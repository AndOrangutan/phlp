#include <core/logger.h>
#include <core/asserts.h>

int main(int argc, char *argv[]) {
    PFATAL("A test error: %2.3f", 69.420f);

    PASSERT(1 < 3);

    return 0;
}
