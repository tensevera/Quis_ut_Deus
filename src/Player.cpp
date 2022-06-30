#include "Player.hpp"

#include <iostream>

bool Player::init( int hp, int maxHealth, int pow, float sp,
    const char* idleAnimPath, int idleAnimFrC, int idleAnimSize,
    const char* runAnimPath, int runAnimFrC, int runAnimSize,
    const char* attackAnimPath, int attackAnimFrC, int attackAnimSize,
    SDL_Renderer* ren, const char* name, TTF_Font* font, int healthPotions) {

    entityID = PLAYER_ID;
    defaultState = idleing;
    this->healthPotions = healthPotions % MAX_HEALTH_POTIONS;

    if( !Entity::init(hp, pow, sp,
     idleAnimPath, idleAnimFrC, idleAnimSize,
     runAnimPath, runAnimFrC, runAnimSize,
     attackAnimPath, attackAnimFrC, attackAnimSize,
     ren, name, font) )
        return false;

    this->maxHealth = maxHealth*2;

    do {
        coords.x = rand() % GAME_WIDTH/2;
        coords.y = rand() % GAME_HEIGHT;
    }while(Map::getTileValue(REAL_COORDS_X,REAL_COORDS_Y) != TILE_GROUND);

    return true;
}

void Player::update() {
    updateAnimation();
    move();
}

void Player::handleInput(const Uint8* keyboardState) {
    if(state == attacking)
        return;
    state = idleing;
    movement.x = movement.y = 0;
    if(keyboardState[SDL_SCANCODE_SPACE])
    {
        attack();
        return;
    }
    if(keyboardState[SDL_SCANCODE_A])
    {
        if(!Map::getTileValue(REAL_COORDS_X - 1,REAL_COORDS_Y) && (REAL_COORDS_X - 1 >= 0) )
            movement.x-=speed;
        state = running;
        flip = SDL_FLIP_HORIZONTAL;
    }    
    if(keyboardState[SDL_SCANCODE_D])
    {
        if(!Map::getTileValue(REAL_COORDS_X + 1,REAL_COORDS_Y) && (REAL_COORDS_X + 1 < MAP_WIDTH))
            movement.x+=speed;
        state = running;
        flip = SDL_FLIP_NONE;
    }
    if(keyboardState[SDL_SCANCODE_S])
    {
        if(!Map::getTileValue(REAL_COORDS_X,REAL_COORDS_Y + 1) && (REAL_COORDS_Y + 1 < MAP_HEIGHT))
            movement.y += speed;
        state = running;
    }
    if(keyboardState[SDL_SCANCODE_W])
    {
        if(!Map::getTileValue(REAL_COORDS_X, REAL_COORDS_Y - 1) && (REAL_COORDS_Y - 1 > 0))
            movement.y -= speed;
        state = running;
    }
    if(keyboardState[SDL_SCANCODE_H])
    {
        if(healthPotions > 0 && health < maxHealth)
        {
            health = std::min(health + HEALTH_POTION_EFFECT,maxHealth);
            --healthPotions;
        }
    }
}

void Player::move() {
    Map::setTileValue(REAL_COORDS_X,REAL_COORDS_Y,TILE_GROUND);
    coords.x += movement.x;
    coords.y += movement.y;
    Map::setTileValue(REAL_COORDS_X,REAL_COORDS_Y,PLAYER_ID);
}

void Player::attack() {
    state = attacking;
    Entity::attack->begin = SDL_GetTicks64();
    if(flip==SDL_FLIP_NONE)
    {
        if (Map::getTileValue(REAL_COORDS_X + 1,REAL_COORDS_Y) == 1)
            return;
        if(Map::getTileValue(REAL_COORDS_X + 1,REAL_COORDS_Y) > 2) 
        {
            std::map<int, Entity*>::iterator enemy = characters.find(Map::getTileValue(REAL_COORDS_X + 1,REAL_COORDS_Y));
            enemy->second->takeDamage(power);
        }
        else if(Map::getTileValue(REAL_COORDS_X + 2,REAL_COORDS_Y) > 2)
        {
            std::map<int, Entity*>::iterator enemy = characters.find(Map::getTileValue(REAL_COORDS_X + 2,REAL_COORDS_Y));
            enemy->second->takeDamage(power);
        }
    }
    else
    {
        if (Map::getTileValue(REAL_COORDS_X - 1,REAL_COORDS_Y) == 1)
            return;
        if(Map::getTileValue(REAL_COORDS_X - 1,REAL_COORDS_Y) > 2)
        {
            std::map<int, Entity*>::iterator enemy = characters.find(Map::getTileValue(REAL_COORDS_X - 1,REAL_COORDS_Y));
            enemy->second->takeDamage(power);
        }
        else if(Map::getTileValue(REAL_COORDS_X - 2,REAL_COORDS_Y) > 2)
        {
            std::map<int, Entity*>::iterator enemy = characters.find(Map::getTileValue(REAL_COORDS_X - 2,REAL_COORDS_Y));
            enemy->second->takeDamage(power);
        }
    }
}

int Player::getNumberOfHealthPotions() const {
    return healthPotions;
}

bool Player::isDead() const {
    return health<=0;
}

const char* Player::getName() const {
    return name.c_str();
}

std::array<float,4> Player::getAttributes() const {
    return {health,power,speed,maxHealth};
}

void Player::addHealthPotion() {
    healthPotions = std::min(healthPotions+1,MAX_HEALTH_POTIONS);
    return;
}