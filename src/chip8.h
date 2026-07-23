#include <array>
#include <cstdint>
#include <string>

class Chip8 {
  public:
    bool loadROM(std::string filepath); // load a rom file into memory, return true if successful, else false

    std::array<uint8_t, 4096> memory{0}; // memory

    const int romMemoryStartIndex{0x200}; // where the rom starts to be loaded in memory

  private:
    std::array<uint16_t, 16> V{0};           // general purpose registers
    uint16_t indexRegister{0};               // index register
    uint16_t PC{0x200};                      // program counter, initialised to 0x200 since thats where most programs start
    std::array<uint16_t, 16> stack{0};       // stack
    uint8_t stackerPointer{0};               // pointer for stack
    std::array<bool, 16> keypad{0};          // keypad keys
    uint8_t delayTimer{0};                   // delay timer
    uint8_t soundTimer{0};                   // sound timer
    std::array<uint8_t, 64 * 32> display{0}; // display array
};
