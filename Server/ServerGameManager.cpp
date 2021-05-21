#include "ServerGameManager.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/random.hpp>
#include <limits> 
#include "../Shared/Global_variables.h"

ServerGameManager::ServerGameManager() {

	// TODO: remove, hardcoded initPos
	// TODO: add new player colliders as players connect
	players.push_back(ServerPlayer(glm::vec3(-10.0f, 3.0f, -10.0f)));
	players.push_back(ServerPlayer(glm::vec3(-10.0f, 15.0f, -5.0f)));

	// Add player hitboxes to all colliders
	for (ServerPlayer p : players) {
		cout << p.health << endl;
		allColliders.push_back(p.hitbox);
	}


	tileSeed = (int)time(NULL);
}

MapState ServerGameManager::generateMap()
{
	srand(tileSeed);

	//Create the tile for the trees to rest on
	Collider* tileC = new Collider(ObjectType::GROUND, glm::vec3(0, -5.0f, 0), glm::vec3(20.0f * NUM_MAP_TILES, 10.0f, 20.0f * NUM_MAP_TILES));
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
				Collider* treeC = new Collider(ObjectType::ENVIRONMENT, glm::vec3(x, 5.0f, z) + glm::vec3(tileCenter.x, 0.0f, tileCenter.z), glm::vec3(1.0f, 10.0f, 1.0f));
				allColliders.push_back(treeC);
			}
		}
	}

	// Set up quadtree 
	Collider boundary = Collider(glm::vec3(0, -5.0f, 0), glm::vec3(110.0f, 50.0f, 110.0f));
	qt = new Quadtree(boundary, 4);
	for (Collider* c : allColliders)
	{
		qt->insert(c);
	}

	// Create map state
	return MapState(tileSeed);
}

void ServerGameManager::handleEvent(Event& e, int playerId)
{
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
		// 5 ticks of jumping in total
		players[playerId].jumping = 10;
	}

	// Parabolic jumping
	float jumpingSquared = players[playerId].jumping* players[playerId].jumping;
	// Handle jumping tick by tick
	if (players[playerId].jumping > 0) {
		players[playerId].update(e.dPos + glm::vec3(0.0f, jumpingSquared/100.0f, 0.0f), e.dYaw, e.dPitch);
		// 5 ticks of jumping in total
		players[playerId].jumping--;
	}

	// Rebuild quadtree for collision after player movement is updated
	buildQuadtree();

	players[playerId].updateAnimations(e);
	players[playerId].isGrounded = false;

	// Naive collision (for now)
	Collider* playerCollider = players[playerId].hitbox;

	// For calculating min distance
	float minHitlength = std::numeric_limits<float>::infinity();
	Collider* closestCollider = NULL;
	// Only run the fat loop when shootin
	if (e.shooting)
	{
		// Check shooting against all other colliders before checking movement 
		for (Collider* otherCollider : allColliders) {

			// Ignore collisions with yourself
			if (playerCollider == otherCollider)
				continue;

			// Check for shooting stuff
			glm::vec3 hitPos;
			if (otherCollider->check_ray_collision(players[playerId].hitbox->cen, players[playerId].front, hitPos))
			{
				float hitLength = glm::length(hitPos - players[playerId].hitbox->cen);
				if (hitLength < minHitlength) {
					minHitlength = hitLength;
					closestCollider = otherCollider;
				}
				std::cout << "hit" << glm::length(hitPos - players[playerId].hitbox->cen) << std::endl;
			}
		}

		// Handle Hit Damage
		if (closestCollider != NULL && closestCollider->type == ObjectType::PLAYER) {
			// TODO: maybe use pointers for players; for loops are pass by value
			for (ServerPlayer& p : players) {
				if (p.hitbox == closestCollider) {
					p.decreaseHealth(10.0f);
				}
			}

		}
	}

	Collider* queryRange = new Collider(players[playerId].hitbox->cen, players[playerId].hitbox->dim * 10.0f);
	vector<Collider*> nearbyColliders;
	nearbyColliders = qt->query(queryRange, nearbyColliders);

	// Movement for colliders 
	for (Collider* otherCollider : nearbyColliders)
	{
		// Ignore collisions with yourself
		if (playerCollider == otherCollider)
			continue;

		// Determine which plane collision happened on
		glm::vec3 plane = playerCollider->check_collision(otherCollider);

		// For jumping
		if (plane.y > 0.0f) {
			players[playerId].isGrounded = true;
		}

		players[playerId].update(plane, 0.0f, 0.0f);

		// If it happened on no plane
		if (plane == glm::vec3(0.0f)) {
			continue;
		}
	}

	buildQuadtree();
}

void ServerGameManager::buildQuadtree() {
	Collider boundary = Collider(glm::vec3(0, -5.0f, 0), glm::vec3(110.0f, 30.0f, 110.0f));
	qt = new Quadtree(boundary, 4);

	for (Collider* c : allColliders)
	{
		qt->insert(c);
	}
}

GameState ServerGameManager::getGameState(int playerId) {
	GameState gs;

	for (int i = 0; i < players.size(); i++) {
		PlayerState ps(i, players[i].pos, players[i].front, players[i].animation, players[i].isGrounded, players[i].health);

		gs.addState(ps);
	}

	return gs;
}
