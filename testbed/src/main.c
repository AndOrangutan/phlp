#include <core/application.h>
#include <core/asserts.h>
#include <core/logger.h>

int main(int argc, char *argv[]) {
    /* ("Phlp Engine Testbed", 100, 100, 1280, 720) */

    app_config config;

    config.name = "Phlp Engine Testbed";
    config.start_pos_x = 100;
    config.start_pos_y = 100;
    config.start_width = 1280;
    config.start_height = 720;

    app_init(&config);

    app_run();

    return 0;
}
