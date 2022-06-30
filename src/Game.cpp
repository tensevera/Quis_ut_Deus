#include "Game.hpp"

Game::Game(SDL_Renderer* ren) {
    renderer = ren;
    running = false;
    map = nullptr;
    menu = nullptr;
    font = nullptr;
}

Game::~Game() {}

bool Game::init(const char* playerName, int health, int speed, int power) {

    font = TextureManager::loadFont("./assets/font.ttf",18);
    if(!font)
        return false;
    
    bigFont = TextureManager::loadFont("./assets/font.ttf",24);

    player = new Player();

    if(!player->init(health,health,power,speed, 
                    "./assets/playerIdle.png",10,135,
                    "./assets/playerRun.png", 6, 135,
                    "./assets/playerAttack.png",4,135,
                    renderer, playerName ,font, 1))
    {
        cleanFont();
        delete player;
        return false;
    }

    menu = new Menu(renderer,bigFont);

    level = 1;

    running = loadLevel();

    state = ingame;

    return true;
}

bool Game::loadSaved() {

    std::ifstream input_file("./savedGame.txt");
    if (!input_file.is_open()) 
    {
        return false;
    }

    std::string savedName;
    int savedHealthPotions;
    int savedLevel;
    float health,power,speed,maxHealth;

    input_file >> savedName;
    input_file >> savedHealthPotions;
    input_file >> savedLevel;
    input_file >> health;
    input_file >> power;
    input_file >> speed;
    input_file >> maxHealth;

    input_file.close();

    font = TextureManager::loadFont("./assets/font.ttf",18);
    if(!font)
        return false;
    
    bigFont = TextureManager::loadFont("./assets/font.ttf",24);

    player = new Player();

    if(!player->init(health,maxHealth,power,speed, 
                    "./assets/playerIdle.png",10,135,
                    "./assets/playerRun.png", 6, 135,
                    "./assets/playerAttack.png",4,135,
                    renderer, savedName.c_str() ,font, savedHealthPotions))
    {
        cleanFont();
        delete player;
        return false;
    }

    menu = new Menu(renderer,bigFont);

    level = savedLevel;

    running = loadLevel();

    state = ingame;

    return true;
}

void Game::clean() {
    if(map)
        delete map;
    if(menu)
        delete menu;
    cleanEntities();
    if(font)
        cleanFont();
}

void Game::cleanFont() {
    TTF_CloseFont(font);
    TTF_CloseFont(bigFont);
    TTF_Quit();
}

void Game::render() {
    SDL_RenderClear(renderer);


    switch (state)
    {
    case ingame:
        map->drawMap();
    
        for(auto &e : Entity::characters)
            e.second->draw();

        TextureManager::drawText( ("Health potions: " + std::to_string(player->getNumberOfHealthPotions()) + "/" + std::to_string(MAX_HEALTH_POTIONS)).c_str()
                                    ,TILE_SIZE,TILE_SIZE/4,255,255,255,font,renderer);

        break;
    case won:
        showWinningScene();
        break;
    case dead:
        showDeathScene();
        break;
    case paused:
        showPausedScene();
        break;
    default:
        break;
    }

    SDL_RenderPresent(renderer);
}

void Game::update() {
    switch (state)
    {
    case ingame:
        for(auto &e : Entity::characters)
            e.second->update();
        if(player->isDead())
        {
            loadDeathScene();
            state = dead;
            return;
        }
        if(Entity::characters.size() == 1)
        {   
            ++level;
            loadLevel();
        }
        break;
    
    default:
        break;
    }
}

void Game::handleInput()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT)
    {
        running = false;
    }
    keyboardState = SDL_GetKeyboardState(nullptr);

    switch (state)
    {
    case ingame:
        player->handleInput(keyboardState);
        if(keyboardState[SDL_SCANCODE_ESCAPE])
        {
            loadPausedScene();
            state = paused;
        }
        break;
    case paused:
        if(keyboardState[SDL_SCANCODE_SPACE] && event.type == SDL_KEYDOWN)
            if (menu->getActiveWidgetName() == std::string("Save"))
                saveGame();
        if(keyboardState[SDL_SCANCODE_SPACE] && event.type == SDL_KEYDOWN)
            if (menu->getActiveWidgetName() == std::string("Back"))
                state = ingame;
        if(keyboardState[SDL_SCANCODE_SPACE] && event.type == SDL_KEYDOWN)
            if (menu->getActiveWidgetName() == std::string("Exit"))
                running = false;
        if(keyboardState[SDL_SCANCODE_UP] && event.type == SDL_KEYDOWN)
            menu->skipToPreviousWidget();
        if(keyboardState[SDL_SCANCODE_DOWN] && event.type == SDL_KEYDOWN)
            menu->skipToNextWidget();
        break;
    case won:
        if(keyboardState[SDL_SCANCODE_SPACE] && event.type == SDL_KEYDOWN)
            if (menu->getActiveWidgetName() == std::string("Exit"))
                running = false;
        break;
    case dead:
        if(keyboardState[SDL_SCANCODE_SPACE] && event.type == SDL_KEYDOWN)
            if (menu->getActiveWidgetName() == std::string("Exit"))
                running = false;
        break;
    default:
        break;
    }
}

bool Game::isRunning() const {
    return running;
}

void Game::cleanEntities() {
    for(auto &e : Entity::characters)
        delete e.second;
}

bool Game::loadLevel() {
    std::string levelPath = "./assets/level" + std::to_string(level) + ".txt";
    std::ifstream newLevelFile(levelPath);

    if(!newLevelFile.is_open())
    {
        loadWinningScene();
        state = won;
        return true;
    }

    int numberOfEnemies;
    newLevelFile >> numberOfEnemies;
    if(numberOfEnemies > 10)
        return false;

    if(!map)
        map = new Map();

    if(!map->init(renderer))
    {
        return false;
    }

    std::string mapPath = "./assets/map" + std::to_string(level) + ".txt";
    if(!map->loadMap(mapPath.c_str()))
    {
        map->clean();
        return false;
    }


    while(numberOfEnemies)
    {
        if( !(new Monster())->init(2,1,1,
                    "./assets/monsterIdle.png",4,150,
                    "./assets/monsterRun.png",8,150,
                    "./assets/monsterAttack.png",8,150,
                    renderer, "enemy", font) )
            return false;
        --numberOfEnemies;
    }

    return true;
}

void Game::loadWinningScene() {
    menu->clearWidgets();
    menu->addWidget("Exit");
}

void Game::loadDeathScene() {
    menu->clearWidgets();
    menu->addWidget("Exit");
}

void Game::loadPausedScene() {
    menu->clearWidgets();
    menu->addWidget("Save");
    menu->addWidget("Back");
    menu->addWidget("Exit");
}

void Game::showWinningScene() {
    TextureManager::drawText("That's all, folks!", GAME_WIDTH/2,GAME_HEIGHT/2,0,0,0,bigFont,renderer);
    menu->drawMenu();
}

void Game::showDeathScene() {
    TextureManager::drawText("YOU DIED", GAME_WIDTH/2,GAME_HEIGHT/2,0,0,0,bigFont,renderer);
    menu->drawMenu();
}

void Game::showPausedScene() {
    TextureManager::drawText("Paused", GAME_WIDTH/2,GAME_HEIGHT/2,0,0,0,bigFont,renderer);
    menu->drawMenu();
}

void Game::saveGame() {
    std::ofstream saveFile;
    saveFile.open ("./savedGame.txt");
    if (!saveFile.is_open()) 
    {
        SDL_Log("Failed to save game!");
        return;
    }
    saveFile << player->getName() << "\n";
    saveFile << player->getNumberOfHealthPotions() << "\n";
    saveFile << level << "\n";
    std::array<float,4> attributes = player->getAttributes();
    saveFile << attributes[0] / 2 << "\n";
    saveFile << attributes[1] << "\n";
    saveFile << (attributes[2] - 1.225)/0.375 << "\n";
    saveFile << attributes[3] / 2 << "\n";
    
    saveFile.close();
}
