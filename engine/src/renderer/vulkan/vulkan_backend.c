#include "vulkan_backend.h"

#include "vulkan/vulkan_core.h"
#include "vulkan_command_buffer.h"
#include "vulkan_device.h"
#include "vulkan_platform.h"
#include "vulkan_renderpass.h"
#include "vulkan_swapchain.h"
#include "vulkan_types.inl"

#include "core/logger.h"
#include "core/pmemory.h"
#include "core/pstring.h"

#include "containers/darray.h"

#include "platform/platform.h"

static vulkan_context context;

VKAPI_ATTR VkBool32 VKAPI_CALL
vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                  VkDebugUtilsMessageTypeFlagsEXT message_type,
                  const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                  void *user_data);

i32 find_memory_index(u32 type_filter, u32 property_flags);

void create_command_buffers(renderer_backend *backend);

b8 vulkan_renderer_backend_initialize(renderer_backend *backend,
                                      const char *app_name,
                                      struct plat_state *plat_state) {
    // Function pointers
    context.find_memory_index = find_memory_index;

    // TODO: Custom allocator
    context.allocator = 0;

    // Setup vulkan instance
    VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app_info.apiVersion = VK_API_VERSION_1_2;
    app_info.pApplicationName = app_name;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "Phlp Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    create_info.pApplicationInfo = &app_info;

    // Obtain a list of required extensions
    const char **required_extensions = darray_create(const char *);

    // Generic surface extension
    darray_push(required_extensions, &VK_KHR_SURFACE_EXTENSION_NAME);
    // Platform-specific extension(s)
    plat_get_required_extension_names(&required_extensions);
    // Debug utils
#if defined(_DEBUG)
    darray_push(required_extensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    PDEBUG("Required extensions:");
    u32 length = darray_length(required_extensions);
    for (u32 i = 0; i < length; ++i) {
        PDEBUG("    %s", required_extensions[i]);
    }
#endif

    create_info.enabledExtensionCount = darray_length(required_extensions);
    create_info.ppEnabledExtensionNames = required_extensions;

    // Validation Layer
    const char **required_validation_layer_names = 0;
    u32 required_validation_layer_count = 0;

#if defined(_DEBUG)
    PINFO("Validation layers enabled. Enumerating...");

    // The list of validation layers to required
    required_validation_layer_names = darray_create(const char *);
    darray_push(required_validation_layer_names,
                &"VK_LAYER_KHRONOS_validation");
    required_validation_layer_count =
        darray_length(required_validation_layer_names);

    // Obtain a list of available validation layers
    u32 available_layer_count = 0;
    VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, 0));
    VkLayerProperties *available_layers =
        darray_reserve(VkLayerProperties, available_layer_count);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count,
                                                available_layers));

    // Verify all required layers are available
    for (u32 i = 0; i < required_validation_layer_count; ++i) {
        PINFO("Searching for layers: %s...",
              required_validation_layer_names[i]);
        b8 found = FALSE;
        for (u32 j = 0; j < available_layer_count; ++j) {
            if (str_equals(required_validation_layer_names[i],
                           available_layers[j].layerName)) {
                found = TRUE;
                PINFO("Found.");
                break;
            }
        }

        if (!found) {
            PFATAL("Required validation layer is missing: %s",
                   required_validation_layer_names[i]);
            return FALSE;
        }
    }
    PINFO("All required validation layers are present.");
#endif

    create_info.enabledLayerCount = required_validation_layer_count;
    create_info.ppEnabledLayerNames = required_validation_layer_names;

    VK_CHECK(
        vkCreateInstance(&create_info, context.allocator, &context.instance));
    PINFO("Vulkan Instance created.");

    // Debugger
#if defined(_DEBUG)
    PDEBUG("Creating Vulkan debugger...");
    u32 log_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {
        VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
    debug_create_info.messageSeverity = log_severity;
    debug_create_info.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debug_create_info.pfnUserCallback = vk_debug_callback;

    PFN_vkCreateDebugUtilsMessengerEXT func =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            context.instance, "vkCreateDebugUtilsMessengerEXT");
    PASSERT_MSG(func, "Failed to create debug messenger!");
    VK_CHECK(func(context.instance, &debug_create_info, context.allocator,
                  &context.debug_messenger))
    PDEBUG("Vulkan debugger created.")
#endif

    // Surface
    PDEBUG("Creating Vulkan surface...");
    if (!plat_create_vulkan_surface(plat_state, &context)) {
        PERROR("Failed to create platform surface!");
        return FALSE;
    }
    PDEBUG("Vulkan surface created.")

    // Device creation
    if (!vulkan_device_create(&context)) {
        PERROR("Failed to create device!");
        return FALSE;
    }

    // Swapchain
    vulkan_swapchain_create(&context, context.framebuffer_width,
                            context.framebuffer_height, &context.swapchain);

    // Renderpass
    vulkan_renderpass_create(
        &context, &context.main_renderpass, 0, 0, context.framebuffer_width,
        context.framebuffer_height, 0.0f, 0.0f, 0.2f, 1.0f, 1.0f, 0);

    // Create command buffers
    create_command_buffers(backend);

    PINFO("Vulkan renderer initialized successfully.");
    return TRUE;
}

void vulkan_renderer_backed_shutdown(renderer_backend *backend) {
    // Destroy in opposite order of creation

    // Command buffers

    for (u32 i = 0; i < context.swapchain.image_count; ++i) {
        if (context.graphics_command_buffers[i].handle) {
            vulkan_command_buffer_free(&context,
                                       context.device.graphics_command_pool,
                                       &context.graphics_command_buffers[i]);
            context.graphics_command_buffers[i].handle = 0;
        }
    }
    darray_destroy(context.graphics_command_buffers);
    context.graphics_command_buffers = 0;

    PDEBUG("Destroying Vulkan renderpass...");
    vulkan_renderpass_destroy(&context, &context.main_renderpass);

    PDEBUG("Destroying Vulkan swapchain...");
    vulkan_swapchain_destroy(&context, &context.swapchain);

    PDEBUG("Destroying Vulkan device...");
    vulkan_device_destroy(&context);

    if (context.surface) {
        PDEBUG("Destroying Vulkan surface...");
        vkDestroySurfaceKHR(context.instance, context.surface,
                            context.allocator);
        context.surface = 0;
    }

#if defined(_DEBUG)
    if (context.debug_messenger) {
        PDEBUG("Destroying Vulkan debugger...");
        PFN_vkDestroyDebugUtilsMessengerEXT func =
            (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
                context.instance, "vkDestroyDebugUtilsMessengerEXT");

        func(context.instance, context.debug_messenger, context.allocator);
    }
#endif

    PDEBUG("Destroying Vulkan instance...");
    vkDestroyInstance(context.instance, context.allocator);
}

void vulkan_renderer_backend_on_resized(renderer_backend *backend,
                                        u16 width,
                                        u16 height) {
}

b8 vulkan_renderer_backend_begin_frame(renderer_backend *backend,
                                       f32 delta_time) {
    return TRUE;
}

b8 vulkan_renderer_backend_end_frame(renderer_backend *backend,
                                     f32 delta_time) {
    return TRUE;
}

VKAPI_ATTR VkBool32 VKAPI_CALL
vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                  VkDebugUtilsMessageTypeFlagsEXT message_type,
                  const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                  void *user_data) {
    switch (message_severity) {
    defualt:
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        PERROR(callback_data->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        PWARN(callback_data->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        PINFO(callback_data->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        PTRACE(callback_data->pMessage);
        break;
    }
    return VK_FALSE;
}

i32 find_memory_index(u32 type_filter, u32 property_flags) {
    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(context.device.physical_device,
                                        &memory_properties);

    for (u32 i = 0; i < memory_properties.memoryTypeCount; ++i) {
        if (type_filter & (1 << i) &&
            (memory_properties.memoryTypes[i].propertyFlags & property_flags) ==
                property_flags) {
            return i;
        }
    }
    PWARN("Unable to find suitable memory type!");
    return -1;
}

void create_command_buffers(renderer_backend *backend) {
    if (!context.graphics_command_buffers) {
        context.graphics_command_buffers = darray_reserve(
            vulkan_command_buffer, context.swapchain.image_count);
        for (u32 i = 0; i < context.swapchain.image_count; ++i) {
            pmemzero(&context.graphics_command_buffers[i],
                     sizeof(vulkan_command_buffer));
        }
    }

    for (u32 i = 0; i < context.swapchain.image_count; ++i) {
        if (context.graphics_command_buffers[i].handle) {
            vulkan_command_buffer_free(&context,
                                       context.device.graphics_command_pool,
                                       &context.graphics_command_buffers[i]);
        }
        pmemzero(&context.graphics_command_buffers[i],
                 sizeof(vulkan_command_buffer));
        vulkan_command_buffer_allocate(
            &context, context.device.graphics_command_pool, TRUE,
            &context.graphics_command_buffers[i]);
    }

    PDEBUG("Vulkan command buffers created.");
}
