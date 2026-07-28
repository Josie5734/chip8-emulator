#define RAYGUI_IMPLEMENTATIONS
#include "gui.h"
#include "keymap.h"
#include "raygui.h"
#include "raylib.h"
#include <cstddef>
#include <cstdint>

GUI::GUI(const std::array<uint8_t, 64 * 32> &d, std::array<uint8_t, 16> &k, int dWidth, int dHeight, int pxScale) : pixelScale(pxScale), keypad(k), display(d), displayWidth(dWidth), displayHeight(dHeight) {
    padding = pixelScale * 2;                      // generate padding size
    windowWidth = (dWidth * pixelScale) + padding; // generate window size from passed in display size
    windowHeight = (dHeight * pixelScale) + padding;
}

void GUI::updateKeypad() {
    for (std::size_t i = 0; i < defaultKeymap.size(); i++) {
        if (IsKeyDown(defaultKeymap[i])) {
            keypad[i] = 1;
        } else if (IsKeyUp(defaultKeymap[i])) {
            keypad[i] = 0;
        }
    }
}

void GUI::drawScreen(int x, int y) {
    for (int r = 0; r < displayHeight; r++) {                                                 // row
        for (int c = 0; c < displayWidth; c++) {                                              // column
            Color color = display[r * displayWidth + c] ? WHITE : BLACK;                      // get pixel state and decide if on/off
            DrawRectangle((c * pixelScale), (r * pixelScale), pixelScale, pixelScale, color); // draw pixel
        }
    }
}

int GUI::drawUI(int x, int y) {
    // play/pause button
    return 0; // return ui y size
}

void GUI::draw() {
    int x{padding}; // set starting x,y values
    int y{padding}; // starts padded 1cell from edge

    y += drawUI(x, y); // draw UI and get the y size of it
    y += padding;      // add padding below ui

    drawScreen(x, y); // draw screen
}
