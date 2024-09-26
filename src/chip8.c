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
