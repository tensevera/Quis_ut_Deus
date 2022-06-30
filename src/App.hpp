#pragma once

#include "Game.hpp"
#include <filesystem>

namespace fs = std::filesystem;

enum AppState {mainMenu,newGameMenu,playing};

class App {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    bool running;
    const Uint8* keyboardState;

    std::string name;
    int hp,speed,power;
    Game* game;

    AppState state;

    Menu* menu;

    void drawOpeningMenu();
public:
    App();
    ~App();
    void loop();
    void render();
    void input();
    void loadMainMenu();
    void loadNewGameMenu();
};