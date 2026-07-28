#pragma once

#include <array>
#include <cstdint>

class GUI {
  public:
    GUI(const std::array<uint8_t, 64 * 32> &d, std::array<uint8_t, 16> &k, int dWidth, int dHeight, int pxScale);

    int windowWidth{}; // window size
    int windowHeight{};
    int padding{}; // padding around screen

    void updateKeypad();           // checking keypad state
    void drawScreen(int x, int y); // draw the Chip8 screen at screen coords x,y
    int drawUI(int x, int y);      // draw emulator ui at screen coords x,y. returns the y height of the ui section (not including padding at bottom)
    void draw();                   // main draw function

  private:
    int pixelScale{1};                             // ratio of rectangle size to actual pixel size e.g 5x scale means one chip8 pixel = 5x5px rectangle in gui
    std::array<uint8_t, 16> &keypad;               // reference to chip8.keypad for updating keypad state
    const std::array<uint8_t, 64 * 32> &display{}; // reference to chip8.display for drawing
    int displayWidth{};                            // display sizing
    int displayHeight{};
};
