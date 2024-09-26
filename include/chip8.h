#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
static const uint8_t VIDEO_WIDTH = 64;
static const uint8_t VIDEO_HEIGHT = 32;

struct Chip8
{
    uint8_t registers[16];
    uint8_t memory[4096];
    uint16_t index;
    uint16_t pc;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t keypad[16];
    uint32_t video[64 * 32];
    uint16_t opcode;
};

void chip8_init(struct Chip8 *chip);
void chip8_load_rom(struct Chip8 *chip, const char *filename);

#endif /*CHIP8_H*/
