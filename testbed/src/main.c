#include <core/logger.h>
#include <core/asserts.h>

#include <platform/platform.h>

int main(int argc, char *argv[]) {
    PFATAL("A test error: %2.3f", 69.420f);
    PWARN("A test error: %2.3f", 69.420f);


    p_state* state;

    state = p_init("Phlp Engine Testbed", 100, 100, 1280, 720);

    while(TRUE) {
        p_pump_messages(state);

    }

    p_kill(state);

    return 0;
}
