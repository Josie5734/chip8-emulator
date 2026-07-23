#include "chip8.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "no ch8 file given" << std::endl;
        return 1;
    }

    Chip8 chip8{};

    chip8.loadROM(argv[1]);

    std::cout << "ROM loading finished\n";

    return 0;
}
