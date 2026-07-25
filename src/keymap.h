#pragma once

#include "raylib.h"
#include <array>

// keymap is defined is order of 0..9, A..F
// not in the actual rows of 4

// define keymap using RayLib key enums
inline std::array<int, 16> defaultKeymap{
    KEY_X, KEY_ONE, KEY_TWO, KEY_THREE, // 0, 1, 2, 3
    KEY_Q, KEY_W, KEY_E,                // 4, 5, 6
    KEY_A, KEY_S, KEY_D,                // 7, 8, 9
    KEY_Z, KEY_C, KEY_FOUR,             // a, b, c
    KEY_R, KEY_F, KEY_V                 // d, e, f
};
