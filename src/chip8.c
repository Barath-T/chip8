#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "chip8.h"
#include "fonts.h"

void chip8_init(struct Chip8 *chip)
{
    memset(&chip->registers, 0, sizeof(chip->registers));
    memset(&chip->memory, 0, sizeof(chip->memory));
    chip->index = 0;
    chip->pc = START_ADDRESS;
    memset(&chip->stack, 0, sizeof(chip->stack));
    chip->sp = 0;
    chip->delay_timer = 0;
    chip->sound_timer = 0;
    memset(&chip->keypad, 0, sizeof(chip->keypad));
    memset(&chip->video, 0, sizeof(chip->video));

    // loading fonts into memory
    for (unsigned int i = 0; i < FONTSET_SIZE; i++)
    {
        chip->memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

    // setting seed for rand()
    // !use %255 to contain it into a byte
    srand(time(NULL));

    // function pointer tables
    chip->table[0x0] = &opcode_prefix0;
    chip->table[0x1] = &OP_1nnn;
    chip->table[0x2] = &OP_2nnn;
    chip->table[0x3] = &OP_3xkk;
    chip->table[0x4] = &OP_4xkk;
    chip->table[0x5] = &OP_5xy0;
    chip->table[0x6] = &OP_6xkk;
    chip->table[0x7] = &OP_7xkk;
    chip->table[0x8] = &opcode_prefix8;
    chip->table[0x9] = &OP_9xy0;
    chip->table[0xA] = &OP_Annn;
    chip->table[0xB] = &OP_Bnnn;
    chip->table[0xC] = &OP_Cxkk;
    chip->table[0xD] = &OP_Dxyn;
    chip->table[0xE] = &opcode_prefixE;
    chip->table[0xF] = &opcode_prefixF;

    for (uint8_t i = 0; i <= 0xE; i++)
    {
        chip->table0[i] = &OP_NULL;
        chip->table8[i] = &OP_NULL;
        chip->tableE[i] = &OP_NULL;
    }

    for (uint8_t i = 0; i <= 0x65; i++)
    {
        chip->tableF[i] = &OP_NULL;
    }

    // table 0: opcodes with prefix 0
    chip->table0[0x0] = &OP_00E0;
    chip->table0[0xE] = &OP_00EE;

    // table 8: opcodes with prefix 8
    chip->table8[0x0] = &OP_8xy0;
    chip->table8[0x1] = &OP_8xy1;
    chip->table8[0x2] = &OP_8xy2;
    chip->table8[0x3] = &OP_8xy3;
    chip->table8[0x4] = &OP_8xy4;
    chip->table8[0x5] = &OP_8xy5;
    chip->table8[0x6] = &OP_8xy6;
    chip->table8[0x7] = &OP_8xy7;
    chip->table8[0xE] = &OP_8xyE;

    // table E: opcodes with prefix E
    chip->tableE[0xE] = &OP_Ex9E;
    chip->tableE[0x1] = &OP_ExA1;

    // table F: opcodes with prefix F
    chip->tableF[0x07] = &OP_Fx07;
    chip->tableF[0x0A] = &OP_Fx0A;
    chip->tableF[0x15] = &OP_Fx15;
    chip->tableF[0x18] = &OP_Fx18;
    chip->tableF[0x1E] = &OP_Fx1E;
    chip->tableF[0x29] = &OP_Fx29;
    chip->tableF[0x33] = &OP_Fx33;
    chip->tableF[0x55] = &OP_Fx55;
    chip->tableF[0x65] = &OP_Fx65;
}

void chip8_load_rom(struct Chip8 *chip, const char *filename)
{
    FILE *rom = fopen(filename, "r");

    // if file doesnt exists
    if (rom == NULL)
    {
        printf("file not loaded");
        return;
    }

    fseek(rom, 0, SEEK_END);
    size_t rom_size = ftell(rom) - 1;
    fseek(rom, 0, SEEK_SET);

    // if rom is larger than the memory
    if (rom_size > (int)(0xfff - START_ADDRESS))
    {
        printf("memory full, couldnt load the whole file");
        return;
    }

    // reading from file one byte at a time into the memory of the chip
    int i = 0;
    while (fread(&chip->memory[START_ADDRESS + i], sizeof(uint8_t), 1, rom))
    {
        i++;
    }
    // removing last byte of the file as it is "end of line" character
    chip->memory[START_ADDRESS + i - 1] = '\0';

    fclose(rom);
}

void chip8_cycle(struct Chip8 *chip)
{

    // fetch
    uint16_t instruction = (chip->memory[chip->pc] << 8u) | chip->memory[chip->pc + 1];

    // move pc to next instruction
    chip->pc += 2;

    // execute
    (*chip->table[(instruction & 0xF000u) >> 12u])(chip);

    // decrement delay timer if it has been set
    if (chip->delay_timer > 0)
        chip->delay_timer--;

    // decrement sound timer if it has been set
    if (chip->sound_timer > 0)
        chip->sound_timer--;
}

void opcode_prefix0(struct Chip8 *chip)
{
    (*chip->table0[chip->opcode & 0x000Fu])(chip);
}
void opcode_prefix8(struct Chip8 *chip)
{
    (*chip->table8[chip->opcode & 0x000Fu])(chip);
}
void opcode_prefixE(struct Chip8 *chip)
{
    (*chip->tableE[chip->opcode & 0x000Fu])(chip);
}
void opcode_prefixF(struct Chip8 *chip)
{
    (*chip->tableF[chip->opcode & 0x00FFu])(chip);
}
