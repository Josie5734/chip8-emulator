#include "chip8.h"
#include <cstddef>
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
    return (opcode & 0x0F00) >> 8; // mask for 2nd digit and shift into 1st to make 0-15
}
uint8_t Chip8::get_00Y0() {
    return (opcode & 0x00F0) >> 4; // mask for 3rd digit and shift into 1st to make 0-15
}

// opcode functions

// CLS - clear display
void Chip8::OP_00E0() {
    display.fill(0); // fill display array with 0s
}

// RET - return from subroutine
void Chip8::OP_00EE() {
    SP--;           // pop stack
    PC = stack[SP]; // pc set to address at top of stack
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
    stack[SP] = PC;
    SP++;
    PC = nnn;
}

// SE - skip next instruction if Vx == kk
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
    if (registers[Vx] == registers[Vy]) {
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
    registers[Vx] = calc & 0xFF;           // set to only the last 8 bits (<256)
    registers[0xf] = (calc > 255) ? 1 : 0; // set VF last so it doesnt overwrite anything
}

// SUB - set Vx = Vx - Vy. set VF to 1 if Vx > Vy
void Chip8::OP_8xy5() {
    uint8_t Vx = get_0X00();
    uint8_t Vy = get_00Y0();

    uint8_t x = registers[Vx]; // store values for checks without changing them
    uint8_t y = registers[Vy];
    uint8_t flag = (x >= y) ? 1 : 0; // compute VF check

    registers[Vx] = x - y; // set calculation in Vx
    registers[0xf] = flag; // set VF
}

// SHR - if LSB of Vx == 1, VF = 1, else VF = 0. then Vx = Vx / 2
void Chip8::OP_8xy6() {
    uint8_t Vx = get_0X00();
    uint8_t x = registers[Vx]; // store Vx value

    registers[Vx] = x >> 1;   // shift right by 1 digit (divide by 2)
    registers[0xf] = x & 0x1; // save least significant bit, either 1 or 0
}

// SUBN - if Vy > Vx, VF = 1 else VF = 0. Vx = Vy - Vx
void Chip8::OP_8xy7() {
    uint8_t Vx = get_0X00();
    uint8_t Vy = get_00Y0();

    uint8_t x = registers[Vx]; // store values for checks without changing them
    uint8_t y = registers[Vy];
    uint8_t flag = (y >= x) ? 1 : 0; // compute VF check

    registers[Vx] = y - x;
    registers[0xf] = flag;
}

// SHL - Shift Left
void Chip8::OP_8xyE() {
    uint8_t Vx = get_0X00();
    uint8_t x = registers[Vx]; // store Vx value

    registers[Vx] = x << 1;           // left shift by 1 digit (x2)
    registers[0xf] = (x & 0x80) >> 7; // extract MSB, shift to just first bit(1 or 0) and set to VF
}

// SNE - skip next instruction if Vx != Vy
void Chip8::OP_9xy0() {
    uint8_t Vx = get_0X00();
    uint8_t Vy = get_00Y0();
    if (registers[Vx] != registers[Vy]) {
        PC += PC_INC_VAL;
    }
}

// LD - set I = nnn
void Chip8::OP_Annn() {
    uint16_t nnn = opcode & 0x0FFF;
    I = nnn;
}

// JP - set PC to nnn + V0
void Chip8::OP_Bnnn() {
    uint16_t nnn = get_0NNN();
    PC = nnn + registers[0x0];
}

// RND - Vx = random byte(number 0 to 255) & kk
void Chip8::OP_Cxkk() {
    uint8_t Vx = get_0X00();
    uint8_t kk = opcode & 0x00FF;
    registers[Vx] = genRandNum() & kk;
}

// DRW - draw n-byte sprite starting at memory location I at Vx,Vy
// set VF if collision
void Chip8::OP_Dxyn() {
    uint8_t Vx = get_0X00();
    uint8_t Vy = get_00Y0();
    uint8_t n = opcode & 0x000F; // get the height of the sprite

    registers[0xf] = 0; // set VF to 0, gets changed to 1 if any pixel in the sprite collides

    // for n bytes
    for (int i = 0; i < n; i++) {     // i is the row of the sprite where each byte is a single row
        uint8_t byte = memory[I + i]; // read byte (row)

        // for each bit in the byte (column)
        for (int b = 7; b >= 0; b--) { // starting from the leftmost bit and working to the right
            // shift the target bit b to position 0 and mask off everything else
            uint8_t spritePixel = (byte >> (7 - b)) & 0x1;
            int wrappedX = (registers[Vx] + b) % displayWidth; // wrap the pixel around display
            int wrappedY = (registers[Vy] + i) % displayHeight;
            uint8_t displayPixel = getDisplayPixel(wrappedX, wrappedY); // place pixel on display
            bool collide = displayPixel & spritePixel;                  // check if pixels collide
            if (collide) {                                              // set collision flag if any collision happens
                registers[0xf] = 1;
            }
            setDisplayPixel(wrappedX, wrappedY, displayPixel ^ spritePixel); // xor pixel onto display
        }
    }
}

// SKP - skip next instruction if key Vx is pressed(down position)
void Chip8::OP_Ex9E() {
    uint8_t Vx = get_0X00();
    if (keypad[registers[Vx]]) { // if key down(key==1)
        PC += PC_INC_VAL;
    }
}

// SKNP - skip next instruction if key Vx is not pressed
void Chip8::OP_ExA1() {
    uint8_t Vx = get_0X00();
    if (!keypad[registers[Vx]]) { // if key down(key==1)
        PC += PC_INC_VAL;
    }
}

// LD - set Vx = delay timer value
void Chip8::OP_Fx07() {
    uint8_t Vx = get_0X00();
    registers[Vx] = delayTimer;
}

// LD - wait for key press, store value of key in Vx
void Chip8::OP_Fx0A() {
    uint8_t Vx = get_0X00();

    if (!waitForKeyRelease) { // if not waiting for any releases
        for (size_t i = 0; i < keypad.size(); i++) {
            if (keypad[i]) {              // if key down
                waitingKey = i;           // store key to wait for being released
                waitForKeyRelease = true; // set waiting state
                break;
            }
        }
        PC -= PC_INC_VAL; // decrement PC to go back to this instruction again
    } else {
        if (!keypad[waitingKey]) {      // if the waiting key is no longer pressed
            registers[Vx] = waitingKey; // store it in the register
            waitForKeyRelease = false;  // set no longer waiting
        } else {
            PC -= PC_INC_VAL; // else key is still down, repeat instruction
        }
    }
    /*
    bool pressed{false}; // key was pressed (default no)

    for (size_t i = 0; i < keypad.size(); i++) { // for every key on keypad
        if (keypad[i]) {                         // if key is down
            registers[Vx] = i;                   // store key number in register
            pressed = true;                      // mark that a key was pressed
            break;                               // exit loop
        }
    }
    if (!pressed) {       // if no key was pressed
        PC -= PC_INC_VAL; // move program counter back one instruction so it repeats
    }
    */
}

// LD DT - set delayTimer = Vx
void Chip8::OP_Fx15() {
    uint8_t Vx = get_0X00();
    delayTimer = registers[Vx];
}

// LD ST - set soundTimer = Vx
void Chip8::OP_Fx18() {
    uint8_t Vx = get_0X00();
    soundTimer = registers[Vx];
}

// ADD I - set I = I + Vx
void Chip8::OP_Fx1E() {
    uint8_t Vx = get_0X00();
    I = I + registers[Vx];
}

// LD F - set I = location of a font sprite for digit Vx
void Chip8::OP_Fx29() {
    uint8_t Vx = get_0X00();
    I = FONTSET_START_ADDRESS + (5 * registers[Vx]); // starting from the start address of the fontset, get digitIndex * 5(bytecount of a single digit sprite)
}

// LD B - take decimal value of Vx,store hundreds in I, tens in I+1 and ones in I+2
void Chip8::OP_Fx33() {
    uint8_t Vx = get_0X00();
    uint8_t value = registers[Vx]; // e.g value = 143

    // ones
    memory[I + 2] = value % 10; // 143 % 10 = 3
    value = value / 10;         // 143 / 10 = 14

    // tens
    memory[I + 1] = value % 10; // 14 % 10 = 4
    value = value / 10;         // 14 / 10 = 1

    // hundreds
    memory[I] = value % 10; // 1 % 10 = 1
}

// LD [I] - copy values from registers V0 to Vx into memory starting at location I
void Chip8::OP_Fx55() {
    uint8_t Vx = get_0X00();
    for (int i = 0; i <= Vx; i++) {
        memory[I + i] = registers[i];
    }
    I += Vx + 1; // increments the I register, only in original Chip8
}

// LD Vx [I] - read into registers V0 to Vx starting from memory location I
void Chip8::OP_Fx65() {
    uint8_t Vx = get_0X00();
    for (int i = 0; i <= Vx; i++) {
        registers[i] = memory[I + i];
    }
    I += Vx + 1; // increments the I register, only in original Chip8
}
