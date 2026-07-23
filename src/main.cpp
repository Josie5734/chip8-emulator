#include "chip8.h"
#include "gui.h"
#include "raylib.h"
#include <iostream>

int main(int argc, char *argv[]) {
    // check launched with correct args
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <ROM file>" << std::endl;
        return 1;
    }

    Chip8 chip8{};          // create chip8 instance
    chip8.loadROM(argv[1]); // load given rom

    // ratio of chip8.display pixels to raylib rectangle size
    // TODO: make this not a randomly placed hardcoded number
    const int pixelScale{20};

    GUI gui{chip8.display, chip8.displayWidth, chip8.displayHeight, pixelScale}; // create gui

    InitWindow(gui.windowWidth, gui.windowHeight, "CHIP-8"); // create raylib window
    SetTargetFPS(60);                                        // aim for 60fps for the timers

    while (!WindowShouldClose()) {
        /*
        // do cpu cycles multiple times per second/frame
        for (int i = 0; i < chip8.cyclesPerFrame; i++) {
            chip8.cycle();
        }
        */
        chip8.cycle();
        chip8.timerUpdate(); // update timers once per frame

        // draw loop
        BeginDrawing();
        ClearBackground(BLACK);
        gui.draw();
        EndDrawing();
        // draw loop
    }

    CloseWindow();

    return 0;
}
