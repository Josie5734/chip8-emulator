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

    // TODO:
    //  make better setup for setting display size
    //  atm GUI is passed the size of the Chip8 screen (in pixels) and a pixel scale(for how big to draw each pixel) then calculates a padding value as (scale*scale)
    // things to ideally define:
    //      there should be a 1cell padding border all the way around the edge of the window
    //      all ui elements should be separated from eachother by atleast 1cell (can just use defined padding for this)
    //      maybe split off drawScreen(x,y) and drawUI(x,y) into separate functions that are then called by draw() which calculates their x,y

    GUI gui{chip8, pixelScale}; // create gui

    InitWindow(gui.windowWidth, gui.windowHeight, "CHIP-8"); // create raylib window
    SetTargetFPS(60);                                        // aim for 60fps for the 60hz timers

    while (!WindowShouldClose()) {
        if (chip8.isRunning()) { // if emulator running
            chip8.timerUpdate(); // update timers once per frame
            // do cpu cycles multiple times per second/frame
            for (int i = 0; i < chip8.clockSpeed; i++) {
                gui.updateKeypad(); // update keypad state
                chip8.cycle();      // cpu fetch/decode/execute cycle
            }
        }

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
