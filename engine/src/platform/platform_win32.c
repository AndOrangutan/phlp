#include "platform.h"

#if PPLATFORM_WINDOWS

#include "core/logger.h"

#include <windows.h>
#include <windowsx.h> // param input extraction
#include <stdlib.h>

typedef struct p_state {
    HINSTANCE h_instance;
    HWND hwnd;
} platfrom_state;

static f64 clock_frequency;
static LARGE_INTEGER start_time;

LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param);

p_state* p_init(const char *application_name, i32 x, i32 y, i32 width, i32 height) {
    p_state* state = malloc(sizeof(p_state));

    state->h_instance = GetModuleHandleA(0);

    // Setup and register window class.
    HICON icon = LoadIcon(state->h_instance, IDI_APPLICATION);
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;  // Get double-clicks
    wc.lpfnWndProc = win32_process_message;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = state->h_instance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // NULL; // Manage the cursor manually
    wc.hbrBackground = NULL;                   // Transparent
    wc.lpszClassName = "phlp_window_class";

    if (!RegisterClassA(&wc)) {
        MessageBoxA(0, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
    }

    // Create window
    u32 client_x = x;
    u32 client_y = y;
    u32 client_width = width;
    u32 client_height = height;

    u32 window_x = client_x;
    u32 window_y = client_y;
    u32 window_width = client_width;
    u32 window_height = client_height;

    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 window_ex_style = WS_EX_APPWINDOW;

    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;


    // Obtain the size of the border.
    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

    // In this case, the border rectangle is negative.
    window_x += border_rect.left;
    window_y += border_rect.top;

    // Grow by the size of the OS border.
    window_width += border_rect.right - border_rect.left;
    window_height += border_rect.bottom - border_rect.top;


    HWND handle = CreateWindowExA(
        window_ex_style, "phlp_window_class", application_name,
        window_style, window_x, window_y, window_width, window_height,
        0, 0, state->h_instance, 0);

    if (handle == 0) {
        MessageBoxA(NULL, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        PFATAL("Window creation failed!");
        return FALSE;
    } else {
        state->hwnd = handle;
    }

    // Show the window
    b32 should_activate = 1;  // TODO: if the window should not accept input, this should be false.
    i32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
    // If initially minimized, use SW_MINIMIZE : SW_SHOWMINNOACTIVE;
    // If initially maximized, use SW_SHOWMAXIMIZED : SW_MAXIMIZE
    ShowWindow(state->hwnd, show_window_command_flags);

    // Clock setup
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clock_frequency = 1.0 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&start_time);

    return state;
}

void p_kill(p_state *state) {
    if (state->hwnd) {
        DestroyWindow(state->hwnd);
        state->hwnd = 0;
    } else {
        PFATAL("Window handle is null!");
    }
}

b8 p_pump_messages(p_state *plat_state) {
    MSG msg;
    while(PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
        /* if (msg.message == WM_QUIT) { */
        /*     return FALSE; */
        /* } */
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return TRUE;
}

void *p_alloc(u64 size, b8 aligned) {
    // NOTE: TEMP use stdlib
    return malloc(size);
}

void p_free(void *block, b8 aligned) {
    // NOTE: TEMP use stdlib
    free(block);
}

void *p_zero_memory(void *block, u64 size) {
    // NOTE: TEMP use stdlib
    return memset(block, 0, size);
}

void *p_copy_memory(void *dest, const void *source, u64 size) {
    // NOTE: TEMP use stdlib
    return memcpy(dest, source, size);
}

void *p_set_memory(void *dest, i32 value, u64 size) {
    // NOTE: TEMP use stdlib
    return memset(dest, value, size);
}

void p_console_write(const char *msg, u8 color) {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    // FATAL,ERROR,WARN,INFO,DEBUG,TRACE
    static u8 levels[LOG_LEVELS] = {71, 116, 6, 2, 113, 8};
    SetConsoleTextAttribute(console_handle, levels[color]);

    OutputDebugStringA(msg);
    u64 length = strlen(msg);
    LPDWORD chars_written = 0;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), msg, (DWORD)length, chars_written, 0);
}

void p_console_write_error(const char *msg, u8 color) {
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    // FATAL,ERROR,WARN,INFO,DEBUG,TRACE
    static u8 levels[LOG_LEVELS] = {71, 116, 6, 2, 113, 8};
    SetConsoleTextAttribute(console_handle, levels[color]);

    OutputDebugStringA(msg);
    u64 length = strlen(msg);
    LPDWORD chars_written = 0;
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), msg, (DWORD)length, chars_written, 0);
}


f64 p_get_time() {
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (f64)now_time.QuadPart * clock_frequency;
}

void p_sleep(u64 ms) {
    Sleep(ms);
}


LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
        case WM_ERASEBKGND:
            // Notify the OS that erasing will be handled by the application to prevent flicker.
            return 1;
        case WM_CLOSE:
            // TODO: Fire an event for the application to quit.
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE: {
            // Get the updated size.
            // RECT r;
            // GetClientRect(hwnd, &r);
            // u32 width = r.right - r.left;
            // u32 height = r.bottom - r.top;

            // TODO: Fire an event for window resize.
        } break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            // Key pressed/released
            //b8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            // TODO: input processing

        } break;
        case WM_MOUSEMOVE: {
            // Mouse move
            //i32 x_position = GET_X_LPARAM(l_param);
            //i32 y_position = GET_Y_LPARAM(l_param);
            // TODO: input processing.
        } break;
        case WM_MOUSEWHEEL: {
            // i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
            // if (z_delta != 0) {
            //     // Flatten the input to an OS-independent (-1, 1)
            //     z_delta = (z_delta < 0) ? -1 : 1;
            //     // TODO: input processing.
            // }
        } break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            //b8 pressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
            // TODO: input processing.
        } break;
    }

    return DefWindowProc(hwnd, msg, w_param, l_param);
}

#endif //PPATFORM_WINDOWS
