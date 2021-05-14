#include "ServerGameManager.h"
#include <glm/gtx/string_cast.hpp>

ServerGameManager::ServerGameManager() {

	// TODO: remove, hardcoded initPos
	players.push_back(Player(glm::vec3(0.0f, 15.0f, 0.0f)));
	players.push_back(Player(glm::vec3(0.0f, -15.0f, 0.0f)));

	// Add player hitboxes to all colliders
	for (Player p : players) {
		allColliders.push_back(p.hitbox);
	}
}

MapState ServerGameManager::generateMap()
{

	// TODO: generate a bunch of colliders for world, but no need to send player colliders
	// Add one new box object into world
	allColliders.push_back(new Collider(glm::vec3(0.0f), glm::vec3(1.0f)));

	// TODO: potentially optimize using pointers
	// Create map state
	MapState ms;

	// Add all colliders to map
	for (Collider* c : allColliders)
	{
		// Convert collider to MapPiece
		// TODO: really no point in sending rotation
		MapPiece mp(c->dim, glm::vec3(0.0f), c->center);
		ms.addPiece(mp);
	}

	return ms;
}

void ServerGameManager::handleEvent(Event& e, int playerId) {
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

		// Zero out the dir the plane is in
		glm::vec3 newDir = e.dPos * plane;

		// Edge case where the product is 0 (perfectly perpendicular collision)
		if (newDir != glm::vec3(0.0f))
			newDir = glm::normalize(newDir);

		// calculate projection to determine how much to move in other plane
		glm::vec3 newDelta = glm::length(e.dPos) * newDir;

		// Move player backwards, then into new direction
		players[playerId].update(newDelta - e.dPos, 0.0f, 0.0f);
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
