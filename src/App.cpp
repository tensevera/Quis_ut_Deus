#include "App.hpp"


App::App(){
    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
        std::cout << "Couldn't initialize graphics: " << SDL_GetError() << std::endl;
        return;
    }
    window = SDL_CreateWindow("Quis ut Deus", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, SDL_WINDOW_SHOWN);
    if(!window)
    {
        SDL_Log("Couldn't initialize window: %s", SDL_GetError());
        
        SDL_Quit();

        return;
    }
    renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer)
    {
        SDL_Log("Couldn't initialize renderer: %s", SDL_GetError());

        SDL_DestroyWindow(window);
        SDL_Quit();

        return;
    }

    TTF_Init();
    font = TTF_OpenFont("./assets/font.ttf",24);
    if(!font)
    {
        SDL_Log("Couldn't initiliaze font");
        return;
    }

    menu = new Menu(renderer,font);

    loadMainMenu();
    state = mainMenu;
    running = true;

    game = new Game(renderer);

    loop();
}

void App::loop(){
    while(state!=playing && running){
        render();
        input();
    }
    while(game->isRunning()) {
        game->handleInput();
        game->update();
        game->render();
    }
    game->clean();
    delete game;
}


void App::render(){
    SDL_RenderClear(renderer);

    menu->drawMenu();

    SDL_RenderPresent(renderer);
}


void App::input(){
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT)
    {
        running = false;
    }
    keyboardState = SDL_GetKeyboardState(nullptr);

    if(keyboardState[SDL_SCANCODE_UP] && event.type == SDL_KEYDOWN)
            menu->skipToPreviousWidget();
    if(keyboardState[SDL_SCANCODE_DOWN] && event.type == SDL_KEYDOWN)
        menu->skipToNextWidget();

    switch (state)
    {
    case mainMenu:
        if(event.type == SDL_KEYDOWN)
        {
            if(keyboardState[SDL_SCANCODE_SPACE])
                if (menu->getActiveWidgetName() == std::string("Exit"))
                    running = false;
            if(keyboardState[SDL_SCANCODE_SPACE])
                {
                    if (menu->getActiveWidgetName() == std::string("Start"))
                    {
                        loadNewGameMenu();
                        state = newGameMenu;
                        return;
                    }
                    else if (menu->getActiveWidgetName() == std::string("Load"))
                    {
                        state = playing;
                        game->loadSaved();
                        return;
                    }
                }
        }   
        break;
    case newGameMenu:
    {
        if(event.type == SDL_KEYDOWN)
        {
            char pressed = (char)event.key.keysym.sym;
            if((pressed >= 'a' && pressed <= 'z'))
                if(menu->changeValue(pressed))
                {
                    if(name[0] == ' ')
                        name = pressed-UPPER_CASE_OFFEST;
                    else
                        name += pressed;
                }
            if((int)pressed == BACKSPACE_ASCII)
            {
                menu->changeValue(pressed);
                name.erase(name.size()-1);
            }
            if(keyboardState[SDL_SCANCODE_SPACE])
            {
                if (menu->getActiveWidgetName() == std::string("Play"))
                {
                    state = playing;
                    game->init(name.c_str(),hp,speed,power);
                }
                else if (menu->getActiveWidgetName() == std::string("Back"))
                {
                    loadMainMenu();
                    state = mainMenu;
                }
            }
            if(keyboardState[SDL_SCANCODE_RIGHT])
            {
                if(menu->changeValue(+1))
                {
                    if(menu->getActiveWidgetName() == std::string("Health"))
                    {
                        ++hp;
                    }
                    else if(menu->getActiveWidgetName() == std::string("Speed"))
                    {
                        ++speed;
                    }
                    else if(menu->getActiveWidgetName() == std::string("Power"))
                    {
                        ++power;
                    }
                }
            }
            if(keyboardState[SDL_SCANCODE_LEFT])
            {
                if(menu->changeValue(-1))
                {
                    if(menu->getActiveWidgetName() == std::string("Heatlh"))
                    {
                        --hp;
                    }
                    else if(menu->getActiveWidgetName() == std::string("Speed"))
                    {
                        --speed;
                    }
                    else if(menu->getActiveWidgetName() == std::string("Power"))
                    {
                        --power;
                    }
                }
            }
        }
        break;
    }
    default:
        break;
    }
}



App::~App(){
    delete menu;
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void App::loadMainMenu(){
    menu->clearWidgets();
    menu->addWidget("Start");
    menu->addWidget("Load");
    menu->addWidget("Exit");
}

void App::loadNewGameMenu(){
    name = "Player";
    hp = speed = power = DEFAULT_SLIDER_VALUE;
    menu->clearWidgets();
    menu->addWidget("Name","Player",false);
    menu->addWidget("Health",DEFAULT_SLIDER_VALUE,5);
    menu->addWidget("Speed",DEFAULT_SLIDER_VALUE,5);
    menu->addWidget("Power",DEFAULT_SLIDER_VALUE,5);
    menu->addWidget("Play");
    menu->addWidget("Back");
}
