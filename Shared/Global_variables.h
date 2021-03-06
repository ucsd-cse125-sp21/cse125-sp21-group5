#pragma once

// Control Networking Packet size
#define PACKET_SIZE 2048

// Control number of players in game.
#define NUM_PLAYERS 4

// Control Map Generation
#define TILE_SIZE 20
#define NUM_MAP_TILES 5
#define MIN_NUM_TREES_PER_TILE 15
#define MAX_NUM_TREES_PER_TILE 25

// Control Player Properties
#define DEATH_TICK_TIMER 100
#define DOG_FLAG_SPAWN glm::vec3(TILE_SIZE * (NUM_MAP_TILES / 2), 1, TILE_SIZE * (NUM_MAP_TILES / 2))
#define CAT_FLAG_SPAWN glm::vec3(TILE_SIZE * (-NUM_MAP_TILES / 2), 1, TILE_SIZE * (-NUM_MAP_TILES / 2))
#define CAT_SPAWN glm::vec3(TILE_SIZE * (NUM_MAP_TILES / 2) + 5, 1.5f, TILE_SIZE * (NUM_MAP_TILES / 2) + 5)
#define DOG_SPAWN glm::vec3(TILE_SIZE * (-NUM_MAP_TILES / 2) - 5, 1.5f, TILE_SIZE * (-NUM_MAP_TILES / 2) - 5)

// Game properties 
#define NUM_CAPTURES_TO_WIN 3
#define VISION_IMPAIRMENT_TIME 300
#define FREEZE_TIME 180
#define DEFAULT_FOV 90.0f
#define BIG_FOV -90.0f
#define DEFAULT_FOG_DENSITY 0.05
#define BIG_FOG_DENSITY 0.35
#define WALK_SPEED 6.0f
#define RUN_SPEED 10.0f


// Server properties
#define GAME_COUNTDOWN_TIMER 300
#define GAMEOVER_TIMER 600

#define CAT_YAW 225.0f
#define DOG_YAW 45.0f

enum class PlayerTeam
{
	CAT_LOVER = 0,
	DOG_LOVER = 1,
	NOBODY = 2
};

enum class State
{
	LOBBY_STATE,
	READY_STATE,
	PLAY_STATE,
	GAMEOVER_STATE,
};