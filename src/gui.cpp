#include "gui.h"
#include "keymap.h"
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
        } else {
            keypad[i] = 0;
        }
    }
}

void GUI::draw() {
    for (int y = 0; y < displayHeight; y++) {
        for (int x = 0; x < displayWidth; x++) {
            Color color = display[y * displayWidth + x] ? WHITE : BLACK;                                          // get pixel state and decide if on/off
            DrawRectangle(padding + (x * pixelScale), padding + (y * pixelScale), pixelScale, pixelScale, color); // draw pixel
        }
    }
}
