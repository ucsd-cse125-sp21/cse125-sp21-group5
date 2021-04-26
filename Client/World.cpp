#include "World.h"

World::World()
{
	for (int i = 0; i < DEFAULT_WORLD_HEIGHT; i++)
	{
		vector<Tile*> vec;
		for (int j = 0; j < DEFAULT_WORLD_WIDTH; j++)
		{
		}
		tiles.push_back(vec);
	}
}