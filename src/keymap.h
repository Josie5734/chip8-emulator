#pragma once

#include "raylib.h"
#include <array>

// define keymap using RayLib key enums
inline std::array<int, 16> defaultKeymap = {
    KEY_X, KEY_ONE, KEY_TWO, KEY_THREE, // 0, 1, 2, 3
    KEY_Q, KEY_W, KEY_E, KEY_A,         // 4, 5, 6, 7
    KEY_S, KEY_D, KEY_Z, KEY_C,         // 8, 9, A, B
    KEY_FOUR, KEY_R, KEY_F, KEY_V       // C, D, E, F
};
