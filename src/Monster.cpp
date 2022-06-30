#include "Monster.hpp"

#include <iostream>

bool Monster::init(  int hp, int pow, int sp,
                const char* idleAnimPath, int idleAnimFrC, int idleAnimSize,
                const char* runAnimPath, int runAnimFrC, int runAnimSize,
                const char* attackAnimPath, int attackAnimFrC, int attackAnimSize,
                SDL_Renderer* ren, const char* name, TTF_Font* font) {

    entityID = getNewID();
    defaultState = running;

    if(!Entity::init(hp,pow,sp,
                idleAnimPath, idleAnimFrC, idleAnimSize,
                runAnimPath, runAnimFrC, runAnimSize,
                attackAnimPath, attackAnimFrC, attackAnimSize,
                ren, name, font))
        return false;

    srand (time(nullptr));

    do {
        coords.x = rand() % GAME_WIDTH/2 + GAME_WIDTH/2;
        coords.y = rand() % GAME_HEIGHT;
    }while(Map::getTileValue(REAL_COORDS_X,REAL_COORDS_Y) != TILE_GROUND);

    return true;
}

void Monster::update() {
    updateAnimation();
    moveToPlayer();
    if(state==attacking && Entity::attack->frame==Entity::attack->frameCount)
    {
        if(abs(Map::getPlayerPosition().x - REAL_COORDS_X) == 1)
        {
            std::map<int, Entity*>::iterator player = characters.find(PLAYER_ID);
            player->second->takeDamage(power);
        }
        state == running;
    }
    if(health <= 0)
    {
        Map::setTileValue(REAL_COORDS_X,REAL_COORDS_Y,TILE_GROUND);
        Entity* tmp = Entity::characters.find(entityID)->second;
        Entity::characters.erase(entityID);
        delete tmp;
    }
}

void Monster::moveToPlayer() {
    if(state==attacking)
        return;
    Map::setTileValue(REAL_COORDS_X,REAL_COORDS_Y,TILE_GROUND);
    SDL_Point playerPos = Map::getPlayerPosition();
    if(playerPos.x < REAL_COORDS_X)
    {
        flip = SDL_FLIP_HORIZONTAL;
        if(!Map::getTileValue(REAL_COORDS_X - 1,REAL_COORDS_Y))
            coords.x -= speed;
    }
    else 
    {
        flip = SDL_FLIP_NONE;
        if(!Map::getTileValue(REAL_COORDS_X + 1,REAL_COORDS_Y))
            coords.x += speed;
    }
    if(playerPos.y < REAL_COORDS_Y)
    {
        if(!Map::getTileValue(REAL_COORDS_X,REAL_COORDS_Y - 1))
            coords.y -= speed;
    }
    else 
    {
        if(!Map::getTileValue(REAL_COORDS_X,REAL_COORDS_Y + 1))
            coords.y += speed;
    }

    Map::setTileValue(REAL_COORDS_X,REAL_COORDS_Y,entityID);

    if(abs(Map::getPlayerPosition().x - REAL_COORDS_X) <= 1 && Map::getPlayerPosition().y == REAL_COORDS_Y )
        attack();
}

void Monster::attack() {
    state = attacking;
    Entity::attack->begin = SDL_GetTicks64();
}