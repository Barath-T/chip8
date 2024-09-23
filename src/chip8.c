#include <chip8.h>
#include <stdio.h>

const unsigned int START_ADDRESS = 0x200;

void chip8_init(struct Chip8 *chip)
{
    chip->pc = START_ADDRESS;
}

// void load_rom(const char* filename){ }
