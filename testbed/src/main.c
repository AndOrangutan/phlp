#include <core/logger.h>
#include <core/asserts.h>

#include <platform/platform.h>

int main(int argc, char *argv[]) {
    PFATAL("A test error: %2.3f", 69.420f);
    PWARN("A test error: %2.3f", 69.420f);


    platform_state state;

    if(platform_init(&state, "Phlp Engine Testbed", 100, 100, 1280, 720)) {
        while(TRUE) {
            platform_pump_messages(&state);

        }
    }

    platform_shutdown(&state);

    return 0;
}
