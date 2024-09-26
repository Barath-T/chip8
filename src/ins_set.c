#include <stdlib.h>
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

// Annn: LD I, addr
// set I = addr
void OP_Annn(struct Chip8 *chip)
{
    uint16_t address = chip->opcode & 0x0FFFu;

    chip->index = address;
}

// Bnnn: JP V0, addr
// Jump to loccation nnn + V0
void OP_Bnnn(struct Chip8 *chip)
{
    uint16_t address = chip->opcode & 0x0FFFu;

    chip->pc = address + chip->registers[0];
}

// Cxkk: RND Vx, byte
// set Vx = random byte AND kk
void OP_Cxkk(struct Chip8 *chip)
{
    uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;
    uint8_t byte = chip->opcode & 0x00FFu;

    chip->registers[Vx] = (rand() % 255) & byte;
}

// Dxyn: DRW Vx, Vy, nibble (height)
// display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
void OP_Dxyn(struct Chip8 *chip)
{
    uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (chip->opcode & 0x00F0u) >> 4u;
    uint8_t height = chip->opcode & 0x000F;

    uint8_t x_pos = chip->registers[Vx] % VIDEO_WIDTH;
    uint8_t y_pos = chip->registers[Vy] % VIDEO_HEIGHT;

    chip->registers[0xF] = 0;

    // iterating each byte/ row of sprite
    for (uint8_t row = 0; row < height; row++)
    {
        uint8_t sprite_byte = chip->memory[chip->index + row];

        // iterating each bit/ col of a sprite byte/ row
        for (uint8_t col = 0; col < 8; col++)
        {
            uint8_t sprite_pixel = sprite_byte & (0x80 >> col);
            uint32_t *video_pixel = &chip->video[((y_pos + row) * VIDEO_WIDTH) + (x_pos + col)];

            if (sprite_pixel)
            {
                // collision
                if (*video_pixel == 0xFFFFFFFF)
                {
                    chip->registers[0xF] = 1;
                }
                // XORing
                *video_pixel ^= 0xFFFFFFFF;
            }
        }
    }
}

// Ex9E: SKP Vx
// skip next instruction if key with the value of Vx is pressed
void OP_Ex9E(struct Chip8* chip){
  uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;
  uint8_t key = chip->registers[Vx];

  if(chip->keypad[key]){
    chip->pc += 2;
  }
}

// ExA1: SKNP Vx
// skip next instruction if key with the value of Vx is not pressed
void OP_ExA1(struct Chip8* chip){
  uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;
  uint8_t key = chip->registers[Vx];

  if(!chip->keypad[key]){
    chip->pc += 2;
  }
}


// Fx07: LD Vx, DT
// set Vx = delay timer value
void OP_Fx07(struct Chip8* chip){
  uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;

  chip->registers[Vx] = chip->delay_timer;
}

// Fx0A: LD Vx, K
// wait for a keypress, store the value of the key in Vx
void OP_Fx0A(struct Chip8* chip){
  uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;

  for(uint8_t i = 0; i<16; i++){
    if(chip->keypad[i]){
      chip->registers[i] = i;
      return;
    }
  }
  chip->pc -= 2;
}

// Fx15: LD DT, Vx
// set delay timer = Vx
void OP_Fx15(struct Chip8* chip){
  uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;

  chip->delay_timer = chip->registers[Vx];
}

// Fx18: LD ST, Vx
// set sound timer = Vx
void OP_Fx18(struct Chip8* chip){
  uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;

  chip->sound_timer = chip->registers[Vx];
}

// Fx1E: ADD I, Vx
// set I = I + Vx
void OP_Fx18(struct Chip8* chip){
  uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;

  chip->index += chip->registers[Vx];
}

// Fx29: LD F, Vx
// set I = location of sprite for digit Vx
void OP_Fx29(struct Chip8* chip){
  uint8_t Vx = (chip->opcode & 0x0F00u) >> 8u;
  uint8_t digit = chip->registers[Vx];

  chip->index = FONTSET_START_ADDRESS + (5*digit);
}

