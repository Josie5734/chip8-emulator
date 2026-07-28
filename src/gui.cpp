#define RAYGUI_IMPLEMENTATION
#include "gui.h"
#include "keymap.h"
#include "raygui.h"
#include "raylib.h"
#include <cstddef>

GUI::GUI(Chip8 &chip8, int pxScale) : chip8(chip8), pixelScale(pxScale) {
    windowPadding = pixelScale; // generate padding size
    uiPadding = 10;             // hardcoded for now, aiming for consistency across pixelScaling. probably be a config option
    buttonSize = uiPadding * 4; // better label for buttons specifically
    uiHeight = buttonSize;      // set uiHeight (no padding)
    // generate window size (padding on all outer edges, vertical padding between ui and screen, ui size, screen size)
    windowWidth = windowPadding + (chip8.displayWidth * pixelScale) + windowPadding;
    windowHeight = windowPadding + uiHeight + uiPadding + (chip8.displayHeight * pixelScale) + windowPadding;

    // raylib stuff
    GuiEnableTooltip();
}

void GUI::updateKeypad() {
    for (std::size_t i = 0; i < defaultKeymap.size(); i++) {
        if (IsKeyDown(defaultKeymap[i])) {
            chip8.keypad[i] = 1;
        } else if (IsKeyUp(defaultKeymap[i])) {
            chip8.keypad[i] = 0;
        }
    }
}

void GUI::drawScreen(int x, int y) {
    for (int r = 0; r < chip8.displayHeight; r++) {                                                   // row
        for (int c = 0; c < chip8.displayWidth; c++) {                                                // column
            Color color = chip8.display[r * chip8.displayWidth + c] ? WHITE : BLACK;                  // get pixel state and decide if on/off
            DrawRectangle(x + (c * pixelScale), y + (r * pixelScale), pixelScale, pixelScale, color); // draw pixel
        }
    }
    // screen border
    DrawRectangleLines(x - 1, y - 1, (chip8.displayWidth * pixelScale) + 2, (chip8.displayHeight * pixelScale) + 2, GRAY);
}

void GUI::drawUI(float x, float y) {
    Rectangle btn{x, y, buttonSize, buttonSize}; // define a button rectangle
    auto newButton = [&](float w = -1.0f) {      // automatically advance the x pos of the buttons, optionally override width per button
        Rectangle r = btn;                       // take snapshot of btn
        r.width = (w < 0) ? buttonSize : w;      // if no width override passed, use default buttonSize else use passed in width
        btn.x += r.width + uiPadding;            // advance x value of btn by its width + uiPadding spacing
        return r;                                // return the current snapshotted btn
    };

    // play/pause
    GuiSetTooltip("Play/Pause");
    if (GuiButton(newButton(), GuiIconText(chip8.isRunning() ? ICON_PLAYER_PAUSE : ICON_PLAYER_PLAY, nullptr))) {
        chip8.togglePause(); // toggle pause state
    }

    // step
    GuiSetTooltip("Do 1 cycle");
    if (GuiButton(newButton(), GuiIconText(ICON_STEP_OVER, nullptr))) {
        if (chip8.isRunning()) { // pause emulator if running
            chip8.togglePause();
        }
        chip8.cycle(); // do a single cpu cycle
    }
}

void GUI::draw() {
    int x{windowPadding}; // set starting x,y values
    int y{windowPadding}; // starts padded 1cell from edge

    drawScreen(x, y);                                    // draw screen
    y += uiPadding + (chip8.displayHeight * pixelScale); // move y down

    drawUI(x, y); // draw UI
}
