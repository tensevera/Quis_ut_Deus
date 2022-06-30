#pragma once

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Animation.hpp"

#define FRAME_SIZE 128

class TextureManager
{
public:
    static SDL_Texture* loadTexture(const char* fileName, SDL_Renderer* ren);
    static void draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dst, SDL_RendererFlip flip, SDL_Renderer* ren);
    static void drawFrame(Animation* animation, int xpos, int ypos, SDL_RendererFlip flip, SDL_Renderer* ren);
    static void drawText(const char* msg, int x, int y, int r, int g, int b, TTF_Font* font, SDL_Renderer* renderer);
    static void drawRect(int x, int y, int width, int r, int g, int b, SDL_Renderer* renderer);
    static TTF_Font* loadFont(const char* fileName, int fontSize);
};
