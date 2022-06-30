#pragma once

#include <SDL2/SDL.h>
#include <map>

#include "Animation.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "Vector.hpp"

#define TEXT_OFFSET_X FRAME_SIZE/2
#define TEXT_OFFSET_Y FRAME_SIZE/6

#define REAL_COORDS_X ((coords.x+FRAME_SIZE/2)/TILE_SIZE)
#define REAL_COORDS_Y ((coords.y+FRAME_SIZE/2)/TILE_SIZE)

#define PLAYER_ID 2


enum EntityState {running, attacking, idleing};

class Entity
{
protected:
    SDL_Renderer* renderer;
    TTF_Font* font;

    static int entityIdGenerator;
    int entityID;
    float health,maxHealth, power,speed;
    std::string name;

    SDL_Point coords;
    SDL_RendererFlip flip;

    EntityState state;
    EntityState defaultState;
    Vector movement;

    Animation* idle;
    Animation* run;
    Animation* attack;
    void updateAnimation();
public:
    static std::map<int,Entity*> characters;

    Entity();
    virtual ~Entity();
    bool init(  int hp, int pow, int sp,
                const char* idleAnimPath, int idleAnimFrC, int idleAnimSize,
                const char* runAnimPath, int runAnimFrC, int runAnimSize,
                const char* attackAnimPath, int attackAnimFrC, int attackAnimSize,
                SDL_Renderer* ren, const char* name, TTF_Font* font);
    void draw();
    virtual void update() = 0;

    static int getNewID();

    void takeDamage(int hit);
};