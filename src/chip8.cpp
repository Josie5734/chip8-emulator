#include "chip8.h"
#include <fstream>
#include <iostream>
#include <string>

// constructor
Chip8::Chip8() {
    // load fonts into memory
    for (int i = 0; i < FONTSET_SIZE; i++) {
        memory[FONTSET_START_ADDRESS + i] = FONTSET[i];
    }

    // setup opcode function pointer tables
    table[0x0] = &Chip8::Table0;
    table[0x1] = &Chip8::OP_1nnn;
    table[0x2] = &Chip8::OP_2nnn;
    table[0x3] = &Chip8::OP_3xkk;
    table[0x4] = &Chip8::OP_4xkk;
    table[0x5] = &Chip8::OP_5xy0;
    table[0x6] = &Chip8::OP_6xkk;
    table[0x7] = &Chip8::OP_7xkk;
    table[0x8] = &Chip8::Table8;
    table[0x9] = &Chip8::OP_9xy0;
    table[0xA] = &Chip8::OP_Annn;
    table[0xB] = &Chip8::OP_Bnnn;
    table[0xC] = &Chip8::OP_Cxkk;
    table[0xD] = &Chip8::OP_Dxyn;
    table[0xE] = &Chip8::TableE;
    table[0xF] = &Chip8::TableF;

    // fill extra unused opcodes with noop
    for (size_t i = 0; i <= 0xE; i++) {
        table0[i] = &Chip8::OP_NULL;
        table8[i] = &Chip8::OP_NULL;
        tableE[i] = &Chip8::OP_NULL;
    }

    table0[0x0] = &Chip8::OP_00E0;
    table0[0xE] = &Chip8::OP_00EE;

    table8[0x0] = &Chip8::OP_8xy0;
    table8[0x1] = &Chip8::OP_8xy1;
    table8[0x2] = &Chip8::OP_8xy2;
    table8[0x3] = &Chip8::OP_8xy3;
    table8[0x4] = &Chip8::OP_8xy4;
    table8[0x5] = &Chip8::OP_8xy5;
    table8[0x6] = &Chip8::OP_8xy6;
    table8[0x7] = &Chip8::OP_8xy7;
    table8[0xE] = &Chip8::OP_8xyE;

    tableE[0x1] = &Chip8::OP_ExA1;
    tableE[0xE] = &Chip8::OP_Ex9E;

    for (size_t i = 0; i <= 0x65; i++) {
        tableF[i] = &Chip8::OP_NULL;
    }

    tableF[0x07] = &Chip8::OP_Fx07;
    tableF[0x0A] = &Chip8::OP_Fx0A;
    tableF[0x15] = &Chip8::OP_Fx15;
    tableF[0x18] = &Chip8::OP_Fx18;
    tableF[0x1E] = &Chip8::OP_Fx1E;
    tableF[0x29] = &Chip8::OP_Fx29;
    tableF[0x33] = &Chip8::OP_Fx33;
    tableF[0x55] = &Chip8::OP_Fx55;
    tableF[0x65] = &Chip8::OP_Fx65;
}

// display buffer
void Chip8::setDisplayPixel(int x, int y, uint8_t v) {
    display[y * displayWidth + x] = v;
}

uint8_t Chip8::getDisplayPixel(int x, int y) {
    return display[y * displayWidth + x];
}

bool Chip8::loadROM(std::string filepath) {
    // check the file is a .ch8 rom
    // takes substring of everything after the last "." in the file
    // e.g file.name.ch8 = ch8
    if (filepath.substr(filepath.find_last_of(".") + 1) != "ch8") {
        std::cerr << "not a .ch8 file\n";
        return false;
    }
    // open file at filepath in binary mode at the end of the file
    std::ifstream file{filepath, std::ios::binary | std::ios::ate};

    // check if file was opened
    if (!file.is_open()) {
        std::cerr << "couldnt open rom file\n";
        return false;
    }

    // get end positin/length (size of file)
    int size = file.tellg();

    // check if the size of the file is larger than
    // the memory size - the space that would be used for the interpreter
    // (the amount of memory available for the rom to be loaded into)
    if (size > memory.max_size() - ROM_START_ADDRESS) {
        // filesize is larger than memory size
        std::cerr << "file is too large to put in memory\n";
        return false;
    }

    // go back to start
    file.seekg(0);

    // read entire file in one pass, directly into memory
    file.read(reinterpret_cast<char *>(memory.data() + ROM_START_ADDRESS), size);

    // close file
    file.close();

    return true;
}

void Chip8::cycle() {
    // fetch + decode
    opcode = (memory[PC] << 8) | memory[PC + 1]; // fetch first byte, shift into left 8 bits, fetch second byte, OR into right 8 bits

    PC += PC_INC_VAL; // increase program counter

    // execute
    ((*this).*(table[(opcode & 0xF000) >> 12]))(); // calls the table[] with the opcodes first value 0x0 so it can figure out which command to call
}

void Chip8::timerUpdate() {
    // manage timers
    if (delayTimer > 0) {
        delayTimer--;
    }
    if (soundTimer > 0) {
        soundTimer--;
    }
}
