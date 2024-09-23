#include "chip8.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("enter a filename to load!\n");
        return -1;
    }
    struct Chip8 first;
    chip8_init(&first);
    chip8_load_rom(&first, argv[1]);

    for (int i = 0; 1; i++)
    {
        char curr_byte = first.memory[0x200 + i];
        if (!curr_byte)
            break;
        printf("%x ", curr_byte);
        if((i+1)%4 ==0 ) printf("\n");
    }
    printf("\n");
    return 0;
}
