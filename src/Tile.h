#pragma once

class Tile
{
    public:
        enum class Type {
            NONE,
            WATER,
            MOUNTAIN,
            BUILDING,
            UNIT,
            AOE
        };

        Type _type;

        Tile(Type type);
};
