
#define NUM_MAP_TILES 5

#define MAX_NUM_TREES_PER_TILE 5

#define DEATH_TICK_TIMER 100

#define CAT_SPAWN glm::vec3(20 * (0 - NUM_MAP_TILES / 2) + 5, 1, 20 * (0 - NUM_MAP_TILES / 2) + 5)
#define DOG_SPAWN glm::vec3(20 * (-1 / 2) + 5, 1, 20 * (-1 / 2) + 5)

enum class PlayerTeam
{
	CAT_LOVER = 0,
	DOG_LOVER = 1,
};
