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
    protected:

    private:
        std::vector<std::vector<Tile>> _tilemap;
};
