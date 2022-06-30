#pragma once

#include "Entity.hpp"

#define MAX_HEALTH_POTIONS 3
#define HEALTH_POTION_EFFECT 3

class Player : public Entity
{
private:
    void move();
    int healthPotions;
public:
    Player() {};
    ~Player() {};

    bool init(  int hp, int pow, int maxHealth, float sp,
                const char* idleAnimPath, int idleAnimFrC, int idleAnimSize,
                const char* runAnimPath, int runAnimFrC, int runAnimSize,
                const char* attackAnimPath, int attackAnimFrC, int attackAnimSize,
                SDL_Renderer* ren, const char* name, TTF_Font* font, int healthPotions);

    void update();
    void handleInput(const Uint8* keyboardState);
    void attack();

    int getNumberOfHealthPotions() const;
    const char* getName() const;
    std::array<float,4> getAttributes() const;
    bool isDead() const;
    void addHealthPotion();
};
