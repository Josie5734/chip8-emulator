#include "chip8.h"
#include <fstream>
#include <iostream>
#include <string>

bool Chip8::loadROM(std::string filepath) {
    // plan:
    // open filepath in ifstream binary mode
    // use ate thingy to start at end of file
    // if !file.open return false
    // read the size of the file
    // if file bigger than memory left,
    //  exit
    // else go back to start and load into memory
    // return true
    std::ifstream file{filepath, std::ios::binary | std::ios::ate};

    if (!file.is_open()) {
        // file couldnt be opened
        std::cerr << "couldnt open rom file\n";
        return false;
    }

    // get end positin/length
    int size = file.tellg();

    // TODO: set this value to a variable or something not hardcoded
    if (size > 4096 - 0x200) {
        // filesize is larger than memory size
        std::cerr << "file is too large to put in memory\n";
        return false;
    }

    std::cout << "size: " << size << std::endl;

    // go back to start
    file.seekg(0);

    // this reads in 1byte chunks until the end of the file
    /*
    uint8_t data;
    while (file.read(reinterpret_cast<char *>(&data), sizeof(data))) {
        std::cout << static_cast<int>(data) << std::endl;
    }
    */

    // read entire file in one pass, directly into memory
    file.read(reinterpret_cast<char *>(memory.data() + 0x200), size);

    file.close();

    /* read out the memory to confirm
    for (int i = 0x200; i < 0x200 + size; i++) {
        std::cout << static_cast<int>(memory[i]) << std::endl;
    }
    */

    std::cout << std::hex << static_cast<int>(memory[0x200]) << std::endl;
    std::cout << std::hex << static_cast<int>(memory[0x201]) << std::endl;
    std::cout << std::hex << static_cast<int>(memory[0x202]) << std::endl;

    return true;
}
