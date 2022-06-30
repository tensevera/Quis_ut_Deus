#include "Entity.hpp"

#include <iostream>

int Entity::entityIdGenerator = 3;

std::map<int,Entity*> Entity::characters;

Entity::Entity() {
}

Entity::~Entity() {
    delete idle;
    delete run;
    delete attack;
}

void Entity::draw() {
    TextureManager::drawText(name.c_str(),coords.x + TEXT_OFFSET_X,coords.y,255,255,255, font, renderer);
    TextureManager::drawText((std::to_string((int)health) + "/" + std::to_string((int)maxHealth)).c_str(), coords.x+ TEXT_OFFSET_X, coords.y + TEXT_OFFSET_Y, 255,255,255,font,renderer);
    switch (state)
    {
    case idleing:
        TextureManager::drawFrame(idle, coords.x, coords.y, flip, renderer);
        break;
    case running:
        TextureManager::drawFrame(run, coords.x, coords.y, flip, renderer);
        break;
    default:
        TextureManager::drawFrame(attack, coords.x, coords.y, flip, renderer);
        break;
    }
}

bool Entity::init( int hp, int pow, int sp,
    const char* idleAnimPath, int idleAnimFrC, int idleAnimSize,
    const char* runAnimPath, int runAnimFrC, int runAnimSize,
    const char* attackAnimPath, int attackAnimFrC, int attackAnimSize,
    SDL_Renderer* ren, const char* name, TTF_Font* font) {

    renderer = ren;
    this->font = font;

    health = hp * 2;
    maxHealth = health;
    power = pow;
    speed = 0.375*sp+1.225;
    this->name = name;

    idle = new Animation(TextureManager::loadTexture(idleAnimPath, renderer),idleAnimFrC,idleAnimSize);
    if(!idle)
        return false;
    run = new Animation(TextureManager::loadTexture(runAnimPath, renderer),runAnimFrC,runAnimSize);
    if(!run)
    {
        delete idle;
        return false;
    }
    attack = new Animation(TextureManager::loadTexture(attackAnimPath, renderer),attackAnimFrC,attackAnimSize);
    if(!attack)
    {
        delete idle;
        delete run;
        return false;
    }

    movement.x = movement.y = 0;
    flip = SDL_FLIP_NONE;

    state = defaultState;

    idle->begin = SDL_GetTicks64();

    characters.insert(std::pair<int, Entity*>(entityID, this));
    return true;
}

void Entity::updateAnimation() {
    switch (state)
    {
    case attacking:
        attack->frame = ((SDL_GetTicks64()-attack->begin)/ANIMATION_SPEED);
        if(attack->frame >= attack->frameCount)
            state = defaultState;
        break;
    case running:
        run->frame = (SDL_GetTicks64()/ANIMATION_SPEED)%run->frameCount;
        break;
    default:
        idle->frame = (SDL_GetTicks64()/ANIMATION_SPEED)%idle->frameCount;
        break;
    }
}

int Entity::getNewID() {
    return entityIdGenerator++;
}

void Entity::takeDamage(int hit) {
    health -= hit;
}