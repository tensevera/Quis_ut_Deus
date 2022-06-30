#include "TextureManager.hpp"

SDL_Texture* TextureManager::loadTexture(const char* fileName, SDL_Renderer* renderer)
{
    SDL_Surface* tmpSurface = IMG_Load(fileName);
    if(!tmpSurface)
    {
        SDL_Log("Failed to load texture: %s, %s",fileName, SDL_GetError());
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer,tmpSurface);
    if(!tex)
    {
        SDL_Log("Failed to load texture from surface: %s, %s",fileName, SDL_GetError());
        SDL_FreeSurface(tmpSurface);
        return nullptr;
    }
    SDL_FreeSurface(tmpSurface);

    return tex;
}

void TextureManager::draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dst, SDL_RendererFlip flip, SDL_Renderer* renderer)
{
    SDL_RenderCopyEx(renderer,texture,&src,&dst,0,nullptr, flip);
}

void TextureManager::drawFrame(Animation* animation, int xpos, int ypos, SDL_RendererFlip flip, SDL_Renderer* renderer)
{
    SDL_Rect src = {animation->size*animation->frame,0,animation->size,animation->size};
    SDL_Rect dst = {xpos,ypos,FRAME_SIZE,FRAME_SIZE};
    SDL_RenderCopyEx(renderer, animation->texture, &src, &dst, 0, nullptr, flip);
}

void TextureManager::drawText(const char* msg, int x, int y, int r, int g, int b, TTF_Font* font, SDL_Renderer* renderer){
    SDL_Surface* surf;
    SDL_Texture* tex;
    SDL_Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = 255;
    SDL_Rect rect;
    surf = TTF_RenderText_Solid(font,msg,color);
    tex = SDL_CreateTextureFromSurface(renderer,surf);
    rect.x = x;
    rect.y = y;
    rect.w = surf->w;
    rect.h = surf->h;
    SDL_FreeSurface(surf);
    SDL_RenderCopy(renderer,tex,nullptr,&rect);
    SDL_DestroyTexture(tex);
}

TTF_Font* TextureManager::loadFont(const char* fileName, int fontSize) {
    if(TTF_Init())
    {
        SDL_Log("Failed to initialize fonts library: %s", TTF_GetError() );
        return nullptr;
    }
    TTF_Font* font;
    font = TTF_OpenFont(fileName,fontSize);
    if(!font)
    {
        SDL_Log("Failed to load font: %s, %s",fileName, TTF_GetError() );
        TTF_Quit();
        return nullptr;
    }

    return font;
}

void TextureManager::drawRect(int x, int y, int width, int r, int g, int b, SDL_Renderer *renderer)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = 20;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderFillRect(renderer, &rect);
}