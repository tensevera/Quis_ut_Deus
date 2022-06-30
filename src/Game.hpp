#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "Player.hpp"
#include "Monster.hpp"
#include "Map.hpp"
#include "Menu.hpp"

enum GameState {ingame,dead,won, paused};

class Game
{
private:
    bool running;
    SDL_Window* window;
    int level;
    Menu* menu;

    Map* map;
    Player* player;

    GameState state;
    const Uint8* keyboardState;
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_Font* bigFont;

    void saveGame();

    void cleanGraphics();
    void cleanFont();
    void cleanEntities();
    bool loadLevel();

    void loadWinningScene();
    void loadPausedScene();
    void loadDeathScene();

    void showWinningScene();
    void showPausedScene();
    void showDeathScene();
public:
    Game(SDL_Renderer* ren);
    ~Game();

    bool init(const char* playerName, int health, int speed, int power);
    bool loadSaved();
    void clean();

    void render();
    void update();
    void handleInput();

    bool isRunning() const;
};
