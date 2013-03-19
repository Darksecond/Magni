#include "TileMap.h"

TileMap::TileMap(int amount, int width, int height)
{
    for(int i = 0; i < amount; i++){
        std::vector<Tile> temp; // create an array, don't work directly on buff yet.
		for(int j = 0; j < amount; j++) {
			temp.emplace_back(Tile::Type::NONE);
        }
        _tilemap.push_back(temp);
	}
}

TileMap::~TileMap()
{

}

Tile::Type TileMap::getType(int x, int y) {
    return (_tilemap[x][y])._type;
}

void TileMap::setType(int x, int y, Tile::Type type) {
    (_tilemap[x][y])._type = type;
}

