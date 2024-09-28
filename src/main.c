#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chip8.h"
#include "platform.h"

extern uint8_t KEYPAD_MAP[128];
int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("args required: scale, delay, rom\n");
        exit(-1);
    }

    unsigned int video_scale = atoi(argv[1]);
    unsigned int cycle_delay = atoi(argv[2]);
    char const *rom_filename = argv[3];

    struct Platform platform;
    platform_init(&platform, "Chip8 Emulator", VIDEO_WIDTH * video_scale, VIDEO_HEIGHT * video_scale, VIDEO_WIDTH,
                  VIDEO_HEIGHT);

    struct Chip8 chip;
    chip8_init(&chip);
    chip8_load_rom(&chip, rom_filename);

    unsigned int video_pitch = sizeof(chip.video[0]) * VIDEO_WIDTH;

    clock_t last_cycle_time = clock();

    char quit = 0;

    while (!quit)
    {
        quit = process_input(&platform, chip.keypad);
        clock_t curr_cycle_time = clock();

        // find time diff and convert it to milli seconds
        double delay = (curr_cycle_time - last_cycle_time) / 5000;

        if (delay > cycle_delay)
        {
            last_cycle_time = curr_cycle_time;
            chip8_cycle(&chip);

            update_window(&platform, chip.video, video_pitch);
        }
    }

    platform_destroy(&platform);
    return 0;
}
