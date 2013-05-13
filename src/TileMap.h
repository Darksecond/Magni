#pragma once

#include "Tile.h"
#include <vector>
#include <iostream>
#include <memory>

class TileMap
{
    public:
        TileMap(int amount, int width, int height);
        virtual ~TileMap();

        Tile::Type getType(int x, int y);
        void setType(int x, int y, Tile::Type type);

        int getMapWidth();
        int getMapHeight();
        int getMapAmount();
    protected:

    private:
        int mapWidth;
        int mapHeight;
        int mapAmount;
        std::vector<std::vector<Tile>> _tilemap;
};
