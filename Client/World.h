#pragma once
#include "Tile.h"
#include <vector>

#define DEFAULT_WORLD_WIDTH 10
#define DEFAULT_WORLD_HEIGHT 10

using namespace std;

class World
{
private:
	unsigned int width = DEFAULT_WORLD_WIDTH;
	unsigned int height = DEFAULT_WORLD_HEIGHT;
	vector<vector<Tile*>> tiles;
public:
	World();
	World(unsigned int& width, unsigned int& height);
	~World();

	void draw(const glm::mat4& parent_transform, const glm::mat4& view);
};