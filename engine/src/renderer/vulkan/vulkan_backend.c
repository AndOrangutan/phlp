#include "vulkan_backend.h"

#include "vulkan/vulkan_core.h"
#include "vulkan_platform.h"
#include "vulkan_types.inl"

#include "core/logger.h"
#include "core/pstring.h"

#include "containers/darray.h"

#include "platform/platform.h"

static vulkan_context context;

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *user_data);

b8 vulkan_renderer_backend_initialize(renderer_backend *backend,
                                      const char *app_name,
                                      struct plat_state *plat_state) {
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
    PINFO("All required validation layers are present.")
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

    PINFO("Vulkan renderer intialized successfully.");
    return TRUE;
}

void vulkan_renderer_backed_shutdown(renderer_backend *backend) {
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

void vulkan_renderer_backend_on_resized(renderer_backend *backend, u16 width,
                                        u16 height) {}

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
