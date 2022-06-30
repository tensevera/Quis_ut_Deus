#include "Map.hpp"
#include "TextureManager.hpp"

int Map::map[20][25] = {};
SDL_Point Map::playerPosition = {0,0};

Map::Map() {}


bool Map::init(SDL_Renderer* ren) {
    renderer = ren;

    ground = TextureManager::loadTexture("./assets/ground.png",renderer);
    if(!ground)
        return false;
    wall = TextureManager::loadTexture("./assets/wall.png", renderer);
    if(!wall)
    {
        SDL_DestroyTexture(ground);
        return false;
    }

    srcRect.x = srcRect.y = 0;
    srcRect.w = srcRect.h = TILE_SRC_SIZE;

    dstRect.w = dstRect.h = TILE_SIZE;
    dstRect.x = dstRect.y = 0;

    return true;
}

Map::~Map() {}

void Map::clean() {
    SDL_DestroyTexture(ground);
    SDL_DestroyTexture(wall);
}

void Map::drawMap() {
    int type;
    for(int row = 0; row < MAP_HEIGHT; ++row)
        for(int column = 0; column < MAP_WIDTH; ++column)
        {
            dstRect.x = column * TILE_SIZE;
            dstRect.y = row * TILE_SIZE;

            type = map[row][column];
            switch (type)
            {
            case 1:
                TextureManager::draw(wall,srcRect,dstRect, SDL_FLIP_NONE, renderer);
                break;
            default:
                TextureManager::draw(ground,srcRect,dstRect, SDL_FLIP_NONE, renderer);
                break;
            }
        }

}

bool Map::loadMap(const char* fileName) {
    std::string currRow;
    std::ifstream newMapFile(fileName);

    if(!newMapFile.is_open())
    {
        SDL_Log("Failed to load map: %s",fileName);
        return false;
    }

    int row;

    for(row = 0; getline(newMapFile,currRow);++row)
    {
        currRow.erase(remove_if(currRow.begin(), currRow.end(), isspace), currRow.end());
        if(currRow.size() > MAP_WIDTH)
        {
            newMapFile.close();
            return false;
        }
        for(int column = 0; column < currRow.size(); ++column)
        {
            if(currRow[column] == '1')
                map[row][column] = TILE_WALL;
            else if(currRow[column] == '0')
                map[row][column] = TILE_GROUND;
            else
            {
                newMapFile.close();
                return false;
            }
        }
    }

    if(row!= MAP_HEIGHT)
    {
        newMapFile.close();
        return false;
    }

    newMapFile.close();
    return true;
}

int Map::getTileValue(int x, int y) {
    if(x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return 1;
    return map[y][x];
}

void Map::setTileValue(int x, int y, int value) {
    map[y][x] = value;
    if (value == 2)
    {
        playerPosition.x = x;
        playerPosition.y = y;
    }
}

SDL_Point Map::getPlayerPosition() {
    return {playerPosition.x,playerPosition.y};
}