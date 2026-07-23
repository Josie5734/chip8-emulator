#include "chip8.h"
#include <cstdint>

// opcodes

// CLS - clear display
void Chip8::OP_00E0() {
}

// RET - return from subroutine
void Chip8::OP_00EE() {
    PC = stack[SP]; // pc set to address at top of stack
    if (SP > 0) {
        SP--; // pop stack
    }
}

// JP addr - set PC to nnn
void Chip8::OP_1nnn() {
    // get nnn value by masking
    uint16_t nnn = opcode & 0x0FFF;
    PC = nnn;
}

void Chip8::OP_2nnn() {}
void Chip8::OP_3xkk() {}
void Chip8::OP_4xkk() {}
void Chip8::OP_5xy0() {}

// LD - load kk into register Vx
void Chip8::OP_6xkk() {
    uint8_t Vx = (opcode & 0x0F00) >> 8; // mask for 3rd hex value and shift to go from bit 12 to bit 4
    uint8_t kk = opcode & 0x00FF;
    V[Vx] = kk;
}

// ADD - set Vx = Vx + kk
void Chip8::OP_7xkk() {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;
    V[Vx] = V[Vx] + kk;
}

void Chip8::OP_8xy0() {}
void Chip8::OP_8xy1() {}
void Chip8::OP_8xy2() {}
void Chip8::OP_8xy3() {}
void Chip8::OP_8xy4() {}
void Chip8::OP_8xy5() {}
void Chip8::OP_8xy6() {}
void Chip8::OP_8xy7() {}
void Chip8::OP_8xyE() {}
void Chip8::OP_9xy0() {}

// LD - set I = nnn
void Chip8::OP_Annn() {
    uint16_t nnn = opcode & 0x0FFF;
    I = nnn;
}

void Chip8::OP_Bnnn() {}
void Chip8::OP_Cxkk() {}

// DRW - draw n-byte sprite starting at memory location I at Vx,Vy
// set VF if collision
void Chip8::OP_Dxyn() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t n = opcode & 0x000F;

    n = n & 0x0F; // mask n to get just the nibble used for n-bytes of sprite

    V[0xf] = 0; // set VF to 0, gets changed to 1 if any pixel in the sprite collides

    // for n bytes
    for (int i = 0; i < n; i++) {     // i is the row of the sprite where each byte is a single row
        uint8_t byte = memory[I + i]; // read byte (row)

        // for each bit in the byte (column)
        for (int b = 7; b >= 0; b--) { // starting from the leftmost bit and working to the right
            // shift the target bit b to position 0 and mask off everything else
            uint8_t spritePixel = (byte >> (7 - b)) & 0x1;

            int wrappedX = (V[x] + b) % DISPLAY_WIDTH; // wrap the pixel and use those values for collision + drawing
            int wrappedY = (V[y] + i) % DISPLAY_HEIGHT;

            uint8_t displayPixel = getDisplayPixel(wrappedX, wrappedY);
            bool collide = displayPixel & spritePixel;

            if (collide) { // set collision flag if any collision happens
                V[0xf] = 1;
            }
            setDisplayPixel(wrappedX, wrappedY, displayPixel ^ spritePixel);
        }
    }
}

void Chip8::OP_Ex9E() {}
void Chip8::OP_ExA1() {}
void Chip8::OP_Fx07() {}
void Chip8::OP_Fx0A() {}
void Chip8::OP_Fx15() {}
void Chip8::OP_Fx18() {}
void Chip8::OP_Fx1E() {}
void Chip8::OP_Fx29() {}
void Chip8::OP_Fx33() {}
void Chip8::OP_Fx55() {}
void Chip8::OP_Fx65() {}
