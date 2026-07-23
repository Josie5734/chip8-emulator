#pragma once

#include <array>
#include <cstdint>

class GUI {
  public:
    GUI(const std::array<uint8_t, 64 * 32> &d, int dWidth, int dHeight, int pxScale);

    int windowWidth{}; // window size
    int windowHeight{};
    int padding{}; // padding around screen

    void draw(); // draw function

  private:
    int pixelScale{1};                             // ratio of rectangle size to actual pixel size e.g 5x scale means one chip8 pixel = 5x5px rectangle in gui
    const std::array<uint8_t, 64 * 32> &display{}; // reference to chip8.display for drawing
    int displayWidth{};                            // display sizing
    int displayHeight{};
};
