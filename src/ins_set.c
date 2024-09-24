#include <string.h>

#include "chip8.h"

// 00E0: CLS
// clear the display
void OP_00E0(struct Chip8 *chip)
{
    memset(&chip->video, 0, sizeof(chip->video));
}

// 00EE: RET
// return from a subroutine
void OP_00EE(struct Chip8 *chip)
{
    chip->sp--;
    chip->pc = chip->stack[chip->sp];
}

// 1nnn: JP addr
// jump to location nnn
void OP_1nnn(struct Chip8 *chip)
{
    uint16_t address = chip->opcode & 0x0FFFu;
    chip->pc = address;
}

// 3xkk: SE Vx, byte
// skip next instruction if Vx = kk
void OP_3xkk(struct Chip8 *chip)
{
    uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;
    uint8_t byte = (chip->opcode & 0x00FFu);

    if (chip->registers[Vx] == byte)
    {
        chip->pc += 2;
    }
}

// 5xy0: SE Vx, Vy
// skip next instruction if Vx = Vy
void OP_5xy0(struct Chip8 *chip)
{
    uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (chip->opcode & 0x00F0u) >> 4u;

    if (chip->registers[Vx] == chip->registers[Vy])
    {
        chip->pc += 2;
    }
}

// 6xkk: LD Vx, byte
// set Vx = kk
void OP_6xkk(struct Chip8 *chip)
{
    uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;
    uint8_t byte = (chip->opcode & 0x00FFu);

    chip->registers[Vx] = byte;
}

// 7xkk: ADD Vx, byte
// set Vx = Vx+kk
void OP_7xkk(struct Chip8 *chip)
{
    uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;
    uint8_t byte = (chip->opcode & 0x00FFu);

    chip->registers[Vx] += byte;
}

// 8xy0: LD Vx, Vy
// set Vx = Vy
void OP_8xy0(struct Chip8 *chip)
{
    uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (chip->opcode & 0x00F0u) >> 4u;

    chip->registers[Vx] = chip->registers[Vy];
}

// 8xy1: OR Vx, Vy
// set Vx = Vx OR Vy
void OP_8xy1(struct Chip8 *chip)
{
    uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (chip->opcode & 0x00F0u) >> 4u;

    chip->registers[Vx] |= chip->registers[Vy];
}

// 8xy2: AND Vx, Vy
// set Vx = Vx AND Vy
void OP_8xy2(struct Chip8 *chip)
{
    uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (chip->opcode & 0x00F0u) >> 4u;

    chip->registers[Vx] &= chip->registers[Vy];
}

// 8xy3: XOR Vx, Vy
// set Vx = Vx XOR Vy
void OP_8xy3(struct Chip8 *chip)
{
    uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (chip->opcode & 0x00F0u) >> 4u;

    chip->registers[Vx] ^= chip->registers[Vy];
}

// 8xy4: ADD Vx, Vy
// set Vx = Vx + Vy, set VF = carry
void OP_8xy4(struct Chip8 *chip)
{
    uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (chip->opcode & 0x00F0u) >> 4u;

    uint16_t sum = chip->registers[Vx] + chip->registers[Vy];

    if (sum > 255u)
        chip->registers[0xF] = 1;
    else
        chip->registers[0xF] = 0;

    chip->registers[Vx] = sum & 0xFFu;
}

// 8xy5: SUB Vx, Vy
// set Vx = Vx - Vy, set VF = NOT (borrow)
void OP_8xy5(struct Chip8 *chip)
{
    uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (chip->opcode & 0x00F0u) >> 4u;

    if (chip->registers[Vx] > chip->registers[Vy])
        chip->registers[0xF] = 1;
    else
        chip->registers[0xF] = 0;

    chip->registers[Vx] -= chip->registers[Vy];
}

// 8xy6: SHR Vx
// set Vx = Vx SHR 1
void OP_8xy6(struct Chip8 *chip)
{
    uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;

    // save LSB to VF
    chip->registers[0xF] = (chip->registers[Vx] & 0x1u);

    chip->registers[Vx] >>= 1;
}

// 8xy7: SUBN Vx, Vy
// set Vx = Vy - Vx, set VF = NOT (borrow)
void OP_8xy7(struct Chip8 *chip)
{
    uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (chip->opcode & 0x00F0u) >> 4u;

    if (chip->registers[Vy] > chip->registers[Vx])
        chip->registers[0xF] = 1;
    else
        chip->registers[0xF] = 0;

    chip->registers[Vx] = chip->registers[Vy] - chip->registers[Vx];
}

// 8xyE: SHL Vx
// set Vx = Vx SHL 1
void OP_8xyE(struct Chip8 *chip)
{
    uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;

    // save MSB to VF
    chip->registers[0xF] = (chip->registers[Vx] & 0x80u) >> 7u;

    chip->registers[Vx] <<= 1;
}

// 9xy0: SNE Vx, Vy
// skip next instruction if Vx != Vy
void OP_9xy0(struct Chip8 *chip)
{
    uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (chip->opcode & 0x00F0u) >> 4u;

    if (chip->registers[Vx] != chip->registers[Vy])
    {
        chip->pc += 2;
    }
}
