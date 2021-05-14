#include "ServerGameManager.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/random.hpp>

ServerGameManager::ServerGameManager() {

	// TODO: remove, hardcoded initPos
	players.push_back(Player(glm::vec3(0.0f, 15.0f, 0.0f)));
	//players.push_back(Player(glm::vec3(0.0f, -15.0f, 0.0f)));

	// Add player hitboxes to all colliders
	for (Player p : players) {
		allColliders.push_back(p.hitbox);
	}
}

MapState ServerGameManager::generateMap()
{

	// Generate a bunch of random colliders
	vector<Collider*> mapColliders;
	for (int i = 0; i < 5; i++)
	{
		glm::vec3 center = (glm::vec3(rand(), rand(), rand()) / (float) RAND_MAX) * 10.0f;
		glm::vec3 dim = (glm::vec3(rand()) / (float) RAND_MAX) * 5.0f;
		//glm::vec3 center = glm::vec3(0.0f, 1.0f, 0.0f);
		//glm::vec3 dim = glm::vec3(5.0f);
		Collider* collider = new Collider(center, dim);
		mapColliders.push_back(collider);
		allColliders.push_back(collider);
	}

	// TODO: potentially optimize using pointers
	// Create map state
	MapState ms;

	// Add all colliders to map
	for (Collider* c : mapColliders)
	{
		// Convert collider to MapPiece
		// TODO: really no point in sending rotation
		MapPiece mp(c->dim/2.0f, glm::vec3(0.0f), c->cen);
		ms.addPiece(mp);
	}

	return ms;
}

void ServerGameManager::handleEvent(Event& e, int playerId) {
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
