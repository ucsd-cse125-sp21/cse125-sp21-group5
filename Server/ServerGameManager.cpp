#include "ServerGameManager.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/random.hpp>

#include "../Shared/Global_variables.h"

ServerGameManager::ServerGameManager() {

	// TODO: remove, hardcoded initPos
	// TODO: add new player colliders as players connect
	players.push_back(ServerPlayer(glm::vec3(-10.0f, 15.0f, -10.0f)));
	players.push_back(ServerPlayer(glm::vec3(-10.0f, 15.0f, -5.0f)));

	// Add player hitboxes to all colliders
	for (ServerPlayer p : players) {
		allColliders.push_back(p.hitbox);
	}

	tileSeed = (int)time(NULL);
}

MapState ServerGameManager::generateMap()
{
	srand(tileSeed);

	//Create the tile for the trees to rest on
	Collider* tileC = new Collider(glm::vec3(0, -5.0f, 0), glm::vec3(20.0f * NUM_MAP_TILES, 10.0f, 20.0f * NUM_MAP_TILES));
	allColliders.push_back(tileC);

	for (int i = 0; i < NUM_MAP_TILES; i++)
	{
		for (int j = 0; j < NUM_MAP_TILES; j++) {

			glm::vec3 tileCenter = glm::vec3(20 * (i - NUM_MAP_TILES / 2), -5.0f, 20 * (j - NUM_MAP_TILES / 2));

			//Skip the two flag tiles
			// TODO:can't skip here
			if ((i == 0 && j == NUM_MAP_TILES / 2) || (i == NUM_MAP_TILES - 1 && j == NUM_MAP_TILES / 2)) {
				continue;
			}

			int numTrees = rand() % MAX_NUM_TREES_PER_TILE;
			cerr << numTrees << endl;

			for (int k = 0; k < numTrees; k++) {
				float x = 20.0f * (rand() / (float)RAND_MAX) - 10.0f;
				float z = 20.0f * (rand() / (float)RAND_MAX) - 10.0f;

				//generate the position inside the tile
				Collider* treeC = new Collider(glm::vec3(x, 5.0f, z) + glm::vec3(tileCenter.x, 0.0f, tileCenter.z), glm::vec3(1.0f, 10.0f, 1.0f));
				allColliders.push_back(treeC);
			}
		}
	}

	for (Collider* c : allColliders)
	{
		cout << to_string(c->cen) << endl;
	}
	// Create map state
	return MapState(tileSeed);
}

void ServerGameManager::handleEvent(Event& e, int playerId)
{
	// TODO: Predict player's intended position instead
	bool reset = false;

	// Calculate where player wants to be
	// Not jumping
	if (!e.jumping) {
		if (players[playerId].vVelocity >= 0) {
			players[playerId].vVelocity -= 0.1f;
		}
		players[playerId].update(e.dPos + glm::vec3(0,players[playerId].vVelocity,0), e.dYaw, e.dPitch);
	}
	// Jumping 
	else {
		if (players[playerId].vVelocity < 0) {
			players[playerId].vVelocity = 0.5;
		}
		// Why hold space when you can shoot your enemies? 
		if (players[playerId].pos.y <= 1.0f)
			players[playerId].update(e.dPos + glm::vec3(0, players[playerId].vVelocity, 0), e.dYaw, e.dPitch);

	}
	players[playerId].updateAnimations(e);

	//bool isColliding = false;
	// Naive collision (for now)
	Collider* playerCollider = players[playerId].hitbox;
	for (Collider* otherCollider : allColliders)
	{
		// Ignore collisions with yourself
		if (playerCollider == otherCollider)
			continue;

		// Check for shooting stuff
		if (e.shooting)
		{
			//std::cout << "shooting" << std::endl;
			glm::vec3 hitPos;
			if (otherCollider->check_ray_collision(players[playerId].hitbox->cen, players[playerId].front, hitPos))
			{
				std::cout << "hit" << glm::length(hitPos - players[playerId].hitbox->cen) << std::endl;
			}
		}

		// Determine which plane collision happened on
		glm::vec3 plane = playerCollider->check_collision(otherCollider);

		players[playerId].update(plane, 0.0f, 0.0f);

		// If it happened on no plane
		if (plane == glm::vec3(0.0f)) {
			continue;
		}
	}
}

GameState ServerGameManager::getGameState(int playerId) {
	GameState gs;

	for (int i = 0; i < players.size(); i++) {
		PlayerState ps(i, players[i].pos, players[i].front, players[i].animation);

		gs.addState(ps);
	}

	return gs;
}
