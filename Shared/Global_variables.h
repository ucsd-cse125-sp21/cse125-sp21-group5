// Control Map Generation
#define TILE_SIZE 20
#define NUM_MAP_TILES 5
#define MAX_NUM_TREES_PER_TILE 5

// Control Player Properties
#define DEATH_TICK_TIMER 100
#define DOG_FLAG_SPAWN glm::vec3(TILE_SIZE * (NUM_MAP_TILES / 2), 1, TILE_SIZE * (NUM_MAP_TILES / 2))
#define CAT_FLAG_SPAWN glm::vec3(TILE_SIZE * (-NUM_MAP_TILES / 2), 1, TILE_SIZE * (-NUM_MAP_TILES / 2))
#define CAT_SPAWN glm::vec3(TILE_SIZE * (NUM_MAP_TILES / 2) + 5, 1, TILE_SIZE * (NUM_MAP_TILES / 2) + 5)
#define DOG_SPAWN glm::vec3(TILE_SIZE * (-NUM_MAP_TILES / 2) - 5, 1, TILE_SIZE * (-NUM_MAP_TILES / 2) - 5)


enum class PlayerTeam
{
	CAT_LOVER = 0,
	DOG_LOVER = 1,
};
