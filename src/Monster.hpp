#pragma once

#include <cmath>
#include <ctime>

#include "Entity.hpp"

class Monster : public Entity 
{
private:
    void moveToPlayer();
    void attack();
public:
    Monster() {};
    ~Monster() {};

    bool init(  int hp, int pow, int sp,
                const char* idleAnimPath, int idleAnimFrC, int idleAnimSize,
                const char* runAnimPath, int runAnimFrC, int runAnimSize,
                const char* attackAnimPath, int attackAnimFrC, int attackAnimSize,
                SDL_Renderer* ren, const char* name, TTF_Font* font);
    void update();
};