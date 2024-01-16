#include <core/logger.h>
#include <core/asserts.h>

#include <platform/platform.h>

int main(int argc, char *argv[]) {
    PFATAL("A test error: %2.3f", 69.420f);
    PERROR("A test error: %2.3f", 69.420f);
    PWARN("A test error: %2.3f", 69.420f);
    PINFO("A test error: %2.3f", 69.420f);
    PDEBUG("A test error: %2.3f", 69.420f);
    PTRACE("A test error: %2.3f", 69.420f);


    plat_state* state;

    state = plat_init("Phlp Engine Testbed", 100, 100, 1280, 720);

    while(TRUE) {
        plat_pump_messages(state);

    }

    plat_kill(state);

    return 0;
}
