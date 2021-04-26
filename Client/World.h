#pragma once
#include "Tile.h"
#include <vector>

#define DEFAULT_WORLD_WIDTH 10
#define DEFAULT_WORLD_HEIGHT 10

using namespace std;

class World : public Node
{
private:
	vector<vector<Tile*>> tiles;
public:
	World();
	World(unsigned int width, unsigned int height);
	~World();
};