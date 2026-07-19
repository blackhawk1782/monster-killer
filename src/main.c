#include "engine/engine.h"
#include "utilities/math_utils.h"
#include <stdio.h>

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    Math_SeedRandom(0);

    if (!Engine_Init(1280, 800, "Monster Killer - Retro Rail Shooter")) {
        fprintf(stderr, "Engine initialization failed!\n");
        return 1;
    }

    Engine_Run();
    Engine_Shutdown();
    return 0;
}
