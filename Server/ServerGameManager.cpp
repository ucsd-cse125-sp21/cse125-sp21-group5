#include "ServerGameManager.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/random.hpp>

ServerGameManager::ServerGameManager() {

	// TODO: remove, hardcoded initPos
	// TODO: add new player colliders as players connect
	players.push_back(Player(glm::vec3(0.0f, 15.0f, 0.0f)));
	//players.push_back(Player(glm::vec3(0.0f, -15.0f, 0.0f)));

	// Add player hitboxes to all colliders
	for (Player p : players) {
		allColliders.push_back(p.hitbox);
	}

	tileSeed = (int)time(NULL);
}

MapState ServerGameManager::generateMap()
{
	srand(tileSeed);
	for (int i = 0; i < NUM_TILES; i++)
	{
		for (int j = 0; j < NUM_TILES; j++) {
			//Skip the two flag tiles
			if ((i == 0 && j == 1) || (i == 2 && j == 1)) {
				continue;
			}

			//Create the tile for the trees to rest on
			Collider* tileT = new Collider(glm::vec3(10.0f, 10, 10.0f), glm::vec3(10 * i - 10, 0, 10 * j - 10));

			int numTrees = rand() % 10;

			for (int k = 0; k < numTrees; k++) {
				float u1 = (rand() / (float)RAND_MAX) * 10.f - 5.f;
				float u2 = (rand() / (float)RAND_MAX) * 10.f - 5.f;

				//genrate the position inside the tile
				Collider* treeT = new Collider(glm::vec3(1.0f / 10.0f) + tileT->dim, glm::vec3(u1, 6, u2) + tileT->cen);
			}
		}
	}
	// Create map state
	return MapState(tileSeed);
}

void ServerGameManager::handleEvent(Event& e, int playerId)
{
	// TODO: Predict player's intended position instead
	bool reset = false;

	// Calculate where player wants to be
	players[playerId].update(e.dPos, e.dYaw, e.dPitch);

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
			std::cout << "shooting" << std::endl;
			glm::vec3 hitPos;
			if (otherCollider->check_ray_collision(players[playerId].hitbox->cen, players[playerId].front, hitPos))
			{
				std::cout << "hit" << glm::length(hitPos - players[playerId].hitbox->cen) << std::endl;
			}
		}

		// Determine which plane collision happened on
		glm::vec3 plane = playerCollider->check_collision(otherCollider);

		// If it happened on no plane
		if (plane == glm::vec3(0.0f))
			continue;

		// Reset player back to original position
		if (!reset)
		{
			players[playerId].update(-e.dPos, 0.0f, 0.0f);
			reset = true;
		}
			

		// Zero out the dir the plane is in
		glm::vec3 newDir = e.dPos * plane;

		// Edge case where the product is 0 (perfectly perpendicular collision)
		if (newDir != glm::vec3(0.0f))
			newDir = glm::normalize(newDir);

		// calculate projection to determine how much to move in other plane
		glm::vec3 newDelta = glm::length(e.dPos) * newDir;

		// Move player backwards, then into new direction
		// Across multiple collisions, the hope is that the newDeltas will cancel out
		// It is a definite possibilty that simultaneous collisions can grant players speed boost (in-game mechanic?)
		players[playerId].update(newDelta, 0.0f, 0.0f);
	}
}

GameState ServerGameManager::getGameState(int playerId) {
	GameState gs;

	for (int i = 0; i < players.size(); i++) {
		PlayerState ps(i, players[i].pos, players[i].front);

		gs.addState(ps);
	}

	return gs;
}
