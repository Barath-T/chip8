#ifndef PLATFORM_H
#define PLATFORM_H

#include <SDL2/SDL.h>

// initialized in src/chip8.c
extern uint8_t KEYPAD_MAP[128];

struct Platform
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
};

void platform_init(struct Platform *platform, char const *title, int window_width, int window_height, int texture_width,
                   int texture_height);

void update_window(struct Platform *platform, void const *buffer, int pitch);
char process_input(struct Platform *platform, uint8_t *keys);

void platform_destroy(struct Platform *platform);

#endif // PLATFORM_H
