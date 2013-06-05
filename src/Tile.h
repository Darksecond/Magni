#pragma once
#include <glm/glm.hpp>
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
    
        Type retType();

        Type _type;
        glm::vec3 centerpoint;
        Tile(Type type);
};
