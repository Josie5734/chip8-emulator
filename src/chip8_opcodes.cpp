#include "chip8.h"
#include <cstdint>
#include <iostream>

// opcode helpers

uint16_t Chip8::get_0NNN() {
    return opcode & 0x0FFF; // mask for right 3 digits
}

uint8_t Chip8::get_00KK() {
    return opcode & 0x00FF; // mask for right 2 digits
}

uint8_t Chip8::get_0X00() {
    return (opcode & 0x0F00) >> 8; // mask for 2d digit and shift into 1st to make it uint8_t rather than uint16_t
}
uint8_t Chip8::get_00Y0() {
    return opcode & 0x00F0; // mask for 2nd digit
}

// opcode functions

// CLS - clear display
void Chip8::OP_00E0() {
    display.fill(0); // fill display array with 0s
}

// RET - return from subroutine
void Chip8::OP_00EE() {
    PC = stack[SP]; // pc set to address at top of stack
    SP--;           // pop stack
}

// JP addr - set PC to nnn
void Chip8::OP_1nnn() {
    // get nnn value by masking
    uint16_t nnn = get_0NNN();
    PC = nnn;
}

// Call nnn - put current PC on stack and jump PC to nnn
void Chip8::OP_2nnn() {
    uint16_t nnn = get_0NNN();
    SP++;
    stack[SP] = PC;
    PC = nnn;
}

// SE - skip next instruction of Vx == kk
void Chip8::OP_3xkk() {
    uint8_t Vx = get_0X00();
    uint8_t kk = get_00KK();
    if (registers[Vx] == kk) {
        PC += PC_INC_VAL;
    }
}

// SNE - skip next instruction if Vx != kk
void Chip8::OP_4xkk() {
    uint8_t Vx = get_0X00();
    uint8_t kk = get_00KK();
    if (registers[Vx] != kk) {
        PC += PC_INC_VAL;
    }
}

// SE - skip next instruction if Vx == Vy
void Chip8::OP_5xy0() {
    uint8_t Vx = get_0X00();
    uint8_t Vy = get_00Y0();
    if (Vx == Vy) {
        PC += PC_INC_VAL;
    }
}

// LD - load kk into register Vx
void Chip8::OP_6xkk() {
    uint8_t Vx = (opcode & 0x0F00) >> 8; // mask for 3rd hex value and shift to go from bit 12 to bit 4
    uint8_t kk = opcode & 0x00FF;
    registers[Vx] = kk;
}

// ADD - set Vx = Vx + kk
void Chip8::OP_7xkk() {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;
    registers[Vx] = registers[Vx] + kk;
}

// LD - load value of Vy into Vx
void Chip8::OP_8xy0() {
    uint8_t Vx = get_0X00();
    uint8_t Vy = get_00Y0();
    registers[Vx] = registers[Vy];
}

// OR - set Vx = Vx OR Vy
void Chip8::OP_8xy1() {
    uint8_t Vx = get_0X00();
    uint8_t Vy = get_00Y0();
    registers[Vx] = registers[Vx] | registers[Vy];
}

// AND - set Vx = Vx AND Vy
void Chip8::OP_8xy2() {
    uint8_t Vx = get_0X00();
    uint8_t Vy = get_00Y0();
    registers[Vx] = registers[Vx] & registers[Vy];
}

// XOR - set Vx = Vx XOR Vy
void Chip8::OP_8xy3() {
    uint8_t Vx = get_0X00();
    uint8_t Vy = get_00Y0();
    registers[Vx] = registers[Vx] ^ registers[Vy];
}

// ADD - set Vx = Vx + Vy. if result > 8bits, set VF = 1, else VF = 0
void Chip8::OP_8xy4() {
    uint8_t Vx = get_0X00();
    uint8_t Vy = get_00Y0();
    uint16_t calc = registers[Vx] + registers[Vy];
    registers[0xf] = 0; // set VF to 0
    if (calc > 255) {
        registers[0xf] = 1; // set VF to 1 if overflow
    }
    registers[Vx] = calc & 0xFF; // set to only the last 8 bits (<256)
}

// SUB - set Vx = Vx - Vy. set VF to 1 if Vx > Vy
void Chip8::OP_8xy5() {
    uint8_t Vx = get_0X00();
    uint8_t Vy = get_00Y0();
    registers[0xf] = 0; // set VF to 0 by default
    if (registers[Vx] > registers[Vy]) {
        registers[0xf] = 1;
    }
    registers[Vx] = registers[Vx] - registers[Vy];
}

// SHR - if LSB of Vx == 1, VF = 1, else VF = 0. then Vx = Vx / 2
void Chip8::OP_8xy6() {
    uint8_t Vx = get_0X00();
    registers[0xf] = registers[Vx] & 0x1; // save least significant bit, either 1 or 0
    registers[Vx] = registers[Vx] >> 1;   // shift right by 1 digit (divide by 2)
}
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
    std::cout << "dxyn drawing\n";
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t n = opcode & 0x000F;

    n = n & 0x0F; // mask n to get just the nibble used for n-bytes of sprite

    registers[0xf] = 0; // set VF to 0, gets changed to 1 if any pixel in the sprite collides

    std::cout << "I: " << static_cast<int>(I) << std::endl;

    // for n bytes
    for (int i = 0; i < n; i++) {     // i is the row of the sprite where each byte is a single row
        uint8_t byte = memory[I + i]; // read byte (row)

        // for each bit in the byte (column)
        for (int b = 7; b >= 0; b--) { // starting from the leftmost bit and working to the right
            // shift the target bit b to position 0 and mask off everything else
            uint8_t spritePixel = (byte >> (7 - b)) & 0x1;

            int wrappedX = (registers[x] + b) % displayWidth; // wrap the pixel and use those values for collision + drawing
            int wrappedY = (registers[y] + i) % displayHeight;

            uint8_t displayPixel = getDisplayPixel(wrappedX, wrappedY);
            bool collide = displayPixel & spritePixel;

            if (collide) { // set collision flag if any collision happens
                registers[0xf] = 1;
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
