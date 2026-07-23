#pragma once

#include <array>
#include <cstdint>
#include <string>

// address where rom starts to be loaded into memory
const int ROM_START_ADDRESS{0x200};

// program counter increment value
const int PC_INC_VAL{2};

// display dimensions
const int DISPLAY_WIDTH{64};
const int DISPLAY_HEIGHT{32};

// fontset taken from https://austinmorlan.com/posts/chip8_emulator/
// size of fontset in bytes
const int FONTSET_SIZE{80};
// memory address that fontset starts being loaded to
const int FONTSET_START_ADDRESS{0x00};

// array of bytes to load in
const uint8_t FONTSET[FONTSET_SIZE] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

class Chip8 {
  public:
    Chip8();                            // constructor
    bool loadROM(std::string filepath); // load a rom file into memory, return true if successful, else false
    void cycle();                       // a cpu cycle: fetch,decode,execute

    void setDisplayPixel(int x, int y, uint8_t v); // set v at the pixel at x,y in display
    uint8_t getDisplayPixel(int x, int y);         // get value of pixel at x,y

    std::array<uint8_t, 4096> memory{0}; // memory

    std::array<uint16_t, 16> V{0};           // general purpose registers
    uint16_t I{0};                           // index register
    uint16_t PC{0x200};                      // program counter, initialised to 0x200 since thats where most programs start
    std::array<uint16_t, 16> stack{0};       // stack
    uint8_t SP{0};                           // pointer for stack
    std::array<bool, 16> keypad{0};          // keypad keys
    uint8_t delayTimer{0};                   // delay timer
    uint8_t soundTimer{0};                   // sound timer
    std::array<uint8_t, 64 * 32> display{0}; // display array. stored as uint8_t but functionally only 1 or 0
    uint16_t opcode;                         // the current opcode during cycle

    // opcodes
    void OP_00E0();
    void OP_00EE();
    void OP_1nnn();
    void OP_2nnn();
    void OP_3xkk();
    void OP_4xkk();
    void OP_5xy0();
    void OP_6xkk();
    void OP_7xkk();
    void OP_8xy0();
    void OP_8xy1();
    void OP_8xy2();
    void OP_8xy3();
    void OP_8xy4();
    void OP_8xy5();
    void OP_8xy6();
    void OP_8xy7();
    void OP_8xyE();
    void OP_9xy0();
    void OP_Annn();
    void OP_Bnnn();
    void OP_Cxkk();
    void OP_Dxyn();
    void OP_Ex9E();
    void OP_ExA1();
    void OP_Fx07();
    void OP_Fx0A();
    void OP_Fx15();
    void OP_Fx18();
    void OP_Fx1E();
    void OP_Fx29();
    void OP_Fx33();
    void OP_Fx55();
    void OP_Fx65();

    // function pointer table for opcodes
    // taken from https://austinmorlan.com/posts/chip8_emulator/#the-instructions
    // as it seems to be the best example. i spent a long time trying to figure out what its doing and i couldnt do it better myself

    // define type shortcut for the tables holding functions
    typedef void (Chip8::*Chip8Func)();

    // define tables that hold the opcode functions
    Chip8Func table[0xF + 1];
    Chip8Func table0[0xE + 1];
    Chip8Func table8[0xE + 1];
    Chip8Func tableE[0xE + 1];
    Chip8Func tableF[0x65 + 1];

    // functions to run the functions in the nested opcode function tables
    void Table0() {
        ((*this).*(table0[opcode & 0x000Fu]))();
    };

    void Table8() {
        ((*this).*(table8[opcode & 0x000Fu]))();
    };

    void TableE() {
        ((*this).*(tableE[opcode & 0x000Fu]))();
    };

    void TableF() {
        ((*this).*(tableF[opcode & 0x00FFu]))();
    };

    void OP_NULL() {};
};
