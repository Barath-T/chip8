#include <chip8.h>
#include <stdio.h>

int main()
{
    struct Chip8 first;
    chip8_init(&first);

    printf("hello, world!\n");
    return 0;
}
