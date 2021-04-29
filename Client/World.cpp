#include "World.h"

World::World()
{
	for (int i = 0; i < DEFAULT_WORLD_HEIGHT; i++)
	{
		vector<Tile*> vec;
		for (int j = 0; j < DEFAULT_WORLD_WIDTH; j++)
		{
			Tile* tile = new Tile(i, j, 10.0f, 10.0f, 5);
			vec.push_back(tile);
		}
		tiles.push_back(vec);
	}
}

World::World(unsigned int& width, unsigned int& height)
{
	this->width = width;
	this->height = height;
	for (int i = 0; i < height; i++)
	{
		vector<Tile*> vec;
		for (int j = 0; j < width; j++)
		{
			Tile* tile = new Tile(i, j, 10.0f, 10.0f, 5);
			vec.push_back(tile);
		}
		tiles.push_back(vec);
	}
}

void World::draw(const glm::mat4& parent_transform, const glm::mat4& view)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			tiles[i][j]->draw(parent_transform, view);
		}
	}
}
