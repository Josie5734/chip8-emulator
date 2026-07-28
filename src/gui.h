#pragma once

#include "chip8.h"

class GUI {
  public:
    GUI(Chip8 &chip8, int pxScale);

    int windowWidth{}; // window size
    int windowHeight{};
    int windowPadding{}; // padding around screen
    int uiPadding{};     // padding for around ui elements
    float buttonSize{};  // same value as padding but better labelled and a float
                         //  specifically for raygui buttons

    void updateKeypad();           // checking keypad state
    void drawScreen(int x, int y); // draw the Chip8 screen at screen coords x,y
    void drawUI(float x, float y); // draw emulator ui at screen coords x,y. returns the y height of the ui section (not including padding at bottom)
    void draw();                   // main draw function

  private:
    Chip8 &chip8;     // reference to emulator
    int pixelScale{}; // ratio of rectangle size to actual pixel size e.g 5x scale means one chip8 pixel = 5x5px rectangle in gui
    int uiHeight{};   // height of the ui section (no padding)
};
