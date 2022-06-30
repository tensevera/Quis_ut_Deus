#pragma once

#include <SDL2/SDL_image.h>
#include <fstream>
#include <algorithm>

#define TILE_SIZE 32
#define TILE_SRC_SIZE 16
#define MAP_WIDTH 25
#define MAP_HEIGHT 20
#define TILE_WALL 1
#define TILE_GROUND 0
#define GAME_WIDTH TILE_SIZE*MAP_WIDTH
#define GAME_HEIGHT TILE_SIZE*MAP_HEIGHT

class Map
{
private:
    SDL_Renderer* renderer;
    SDL_Rect srcRect, dstRect;
    SDL_Texture* ground;
    SDL_Texture* wall;
    static int map[MAP_HEIGHT][MAP_WIDTH];
    static SDL_Point playerPosition;
public:
    Map();
    ~Map();

    bool init(SDL_Renderer* ren);
    void clean();

    bool loadMap(const char* fileName);
    void drawMap();

    static int getTileValue(int x, int y);
    static void setTileValue(int x, int y, int value);

    static SDL_Point getPlayerPosition();
};
