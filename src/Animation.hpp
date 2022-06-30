#pragma once

#include <SDL2/SDL.h>

#define ANIMATION_SPEED 75

struct Animation
{
    SDL_Texture* texture;
    int frame, frameCount;
    int size;
    Uint64 begin;

    Animation(SDL_Texture* tex, int frC, int s) : texture(tex), frameCount(frC), size(s), frame(0) {}
    ~Animation() {
        SDL_DestroyTexture(texture);
    }
};
