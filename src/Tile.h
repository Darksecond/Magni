#pragma once

class Tile
{
    public:
        enum class Type {
            NONE,
            WATER,
            MOUNTAIN,
            BUILDING,
            UNIT
        };

        Type _type;

        Tile(Type type);
};
