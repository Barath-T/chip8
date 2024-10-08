#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

// initialised in src/chip8.c
extern const unsigned int START_ADDRESS;
extern const unsigned int FONTSET_START_ADDRESS;

// initialised in src/chip8.c
extern const uint8_t VIDEO_WIDTH;
extern const uint8_t VIDEO_HEIGHT;
extern const uint8_t CHIP8_KEYMAP[16];

struct Chip8;
typedef void (*chip8_ins)(struct Chip8 *);

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

    chip8_ins table[0xF + 1];
    chip8_ins table0[0xE + 1];
    chip8_ins table8[0xE + 1];
    chip8_ins tableE[0xE + 1];
    chip8_ins tableF[0x65 + 1];
};

void chip8_init(struct Chip8 *chip);
void chip8_load_rom(struct Chip8 *chip, const char *filename);
void chip8_cycle(struct Chip8 *chip);

void OP_NULL(struct Chip8 *chip);
void OP_00E0(struct Chip8 *chip);
void OP_00EE(struct Chip8 *chip);
void OP_1nnn(struct Chip8 *chip);
void OP_2nnn(struct Chip8 *chip);
void OP_3xkk(struct Chip8 *chip);
void OP_4xkk(struct Chip8 *chip);
void OP_5xy0(struct Chip8 *chip);
void OP_6xkk(struct Chip8 *chip);
void OP_7xkk(struct Chip8 *chip);
void OP_8xy0(struct Chip8 *chip);
void OP_8xy1(struct Chip8 *chip);
void OP_8xy2(struct Chip8 *chip);
void OP_8xy3(struct Chip8 *chip);
void OP_8xy4(struct Chip8 *chip);
void OP_8xy5(struct Chip8 *chip);
void OP_8xy6(struct Chip8 *chip);
void OP_8xy7(struct Chip8 *chip);
void OP_8xyE(struct Chip8 *chip);
void OP_9xy0(struct Chip8 *chip);
void OP_Annn(struct Chip8 *chip);
void OP_Bnnn(struct Chip8 *chip);
void OP_Cxkk(struct Chip8 *chip);
void OP_Dxyn(struct Chip8 *chip);
void OP_Ex9E(struct Chip8 *chip);
void OP_ExA1(struct Chip8 *chip);
void OP_Fx07(struct Chip8 *chip);
void OP_Fx0A(struct Chip8 *chip);
void OP_Fx15(struct Chip8 *chip);
void OP_Fx18(struct Chip8 *chip);
void OP_Fx1E(struct Chip8 *chip);
void OP_Fx29(struct Chip8 *chip);
void OP_Fx33(struct Chip8 *chip);
void OP_Fx55(struct Chip8 *chip);
void OP_Fx65(struct Chip8 *chip);

void opcode_prefix0(struct Chip8 *chip);
void opcode_prefix8(struct Chip8 *chip);
void opcode_prefixE(struct Chip8 *chip);
void opcode_prefixF(struct Chip8 *chip);

#endif /*CHIP8_H*/
