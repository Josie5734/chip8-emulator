#pragma once

#include "raylib.h"
#include <array>

// define keymap using RayLib key enums
inline std::array<int, 16> defaultKeymap = {
    KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR, // 1, 2, 3, 4
    KEY_Q, KEY_W, KEY_E, KEY_R,            // q, w, e, r
    KEY_A, KEY_S, KEY_D, KEY_F,            // a, s, d, f
    KEY_Z, KEY_X, KEY_C, KEY_V             // z, x, c, v
};
