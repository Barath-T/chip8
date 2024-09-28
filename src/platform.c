#include <SDL2/SDL.h>
#include <stdint.h>

#include "platform.h"

void platform_init(struct Platform *platform, char const *title, int window_width, int window_height, int texture_width,
                   int texture_height)
{

    SDL_Init(SDL_INIT_VIDEO);

    platform->window = SDL_CreateWindow(title, 100, 100, window_width, window_height, SDL_WINDOW_SHOWN);
    platform->renderer = SDL_CreateRenderer(platform->window, -1, SDL_RENDERER_ACCELERATED);
    platform->texture = SDL_CreateTexture(platform->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
                                          texture_width, texture_height);
}

void platform_destroy(struct Platform *platform)
{

    SDL_DestroyTexture(platform->texture);
    SDL_DestroyRenderer(platform->renderer);
    SDL_DestroyWindow(platform->window);
    SDL_Quit();
}

void update_window(struct Platform *platform, void const *buffer, int pitch)
{

    SDL_UpdateTexture(platform->texture, NULL, buffer, pitch);
    SDL_RenderClear(platform->renderer);
    SDL_RenderCopy(platform->renderer, platform->texture, NULL, NULL);
    SDL_RenderPresent(platform->renderer);
}

char process_input(struct Platform *platform, uint8_t *keys)
{
    char quit = 0;

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT: {
            quit = 1;
        }
        break;
        case SDL_KEYDOWN: {
            uint8_t keycode = event.key.keysym.sym;
            if (keycode == SDLK_ESCAPE)
            {
                quit = 1;
                break;
            }
            if (KEYPAD_MAP[keycode] != 255)
            {
                keys[KEYPAD_MAP[keycode]] = 1;
            }
        }
        break;
        case SDL_KEYUP: {
            uint8_t keycode = event.key.keysym.sym;

            if (KEYPAD_MAP[keycode] != 255)
            {
                keys[KEYPAD_MAP[keycode]] = 0;
            }
        }
        break;
        }
    }
    return quit;
}
