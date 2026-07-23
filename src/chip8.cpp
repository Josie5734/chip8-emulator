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
}

// display buffer
void Chip8::setDisplayPixel(int x, int y, uint8_t v) {
    display[y * DISPLAY_WIDTH + x] = v;
}

uint8_t Chip8::getDisplayPixel(int x, int y) {
    return display[y * DISPLAY_WIDTH + x];
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
    /*plan:
    fetch:
        read the 2 bytes from memory[pc]

    decode:
        put together to get one 16bit opcode

    execute:
        execute the matching opcode function

    cleanup:
        increment pc if needed
        manager timers
    */
}
