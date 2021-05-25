#include "ServerGameManager.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/random.hpp>
#include <limits> 

ServerGameManager::ServerGameManager() {

	// TODO: remove, hardcoded initPos
	// TODO: add new player colliders as players connect
	tileSeed = (int)time(NULL);

	//Tile and flags are unintialized
	flagCatCarrierId = -1;
	flagDogCarrierId = -1;
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
			//Add the flag to the colliders
			if ((i == 0 && j == 0)) {
				flagCat = new Collider(ObjectType::FLAG_CAT, glm::vec3(20 * (i - NUM_MAP_TILES / 2), 1, 20 * (j - NUM_MAP_TILES / 2)), glm::vec3(1));
				allColliders.push_back(flagCat);
				continue;
			}
			else if ((i == NUM_MAP_TILES - 1 && j == NUM_MAP_TILES - 1)) {
				flagDog = new Collider(ObjectType::FLAG_DOG, glm::vec3(20 * (i - NUM_MAP_TILES / 2), 1, 20 * (j - NUM_MAP_TILES / 2)), glm::vec3(1));
				allColliders.push_back(flagDog);
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
	// TODO: Varying death timers 
	if (players[playerId]->isDead > 0) {
		players[playerId]->isDead--;
		
		if (players[playerId]->isDead == 0) {
			players[playerId]->health = 100.0f;
			players[playerId]->hitbox->isActive = true;
			cout << "health being reset" << endl;
		}


		//If the player is dead, we don't let them move or do anything
		return;
	}

	// Check if player has fallen off map
	if (players[playerId]->pos.y < -10.0f)
	{
		players[playerId]->decreaseHealth(10.0f);
	}

	// Not jumping
	if (!e.jumping) {
		if (players[playerId]->vVelocity >= 0) {
			players[playerId]->vVelocity -= 0.1f;
		}
		players[playerId]->update(e.dPos + glm::vec3(0,players[playerId]->vVelocity,0), e.dYaw, e.dPitch);
	}
	// Jumping 
	else {
		// 10 ticks of jumping in total
		players[playerId]->jumping = 10;
	}


	// Parabolic jumping
	float jumpingSquared = players[playerId]->jumping* players[playerId]->jumping;
	// Handle jumping tick by tick
	if (players[playerId]->jumping > 0) {
		players[playerId]->update(e.dPos + glm::vec3(0.0f, jumpingSquared/100.0f, 0.0f), e.dYaw, e.dPitch);
		// 5 ticks of jumping in total
		players[playerId]->jumping--;
	}

	// Rebuild quadtree for collision after player movement is updated
	buildQuadtree();

	players[playerId]->updateAnimations(e);
	players[playerId]->isGrounded = false;

	// Naive collision (for now)
	Collider* playerCollider = players[playerId]->hitbox;

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
			if (otherCollider->check_ray_collision(players[playerId]->hitbox->cen, players[playerId]->front, hitPos))
			{
				float hitLength = glm::length(hitPos - players[playerId]->hitbox->cen);
				if (hitLength < minHitlength) {
					minHitlength = hitLength;
					closestCollider = otherCollider;
				}
				std::cout << "hit" << glm::length(hitPos - players[playerId]->hitbox->cen) << std::endl;
			}
		}

		// Handle Hit Damage
		if (closestCollider != NULL && closestCollider->type == ObjectType::PLAYER) {
			// TODO: maybe use pointers for players; for loops are pass by value
			for (auto p : players) {
				if (p.second->hitbox == closestCollider) {
					p.second->decreaseHealth(100.0f);
					
					//Drop flag if the player is dead
					if (p.second->isDead && flagCatCarrierId == p.first ) {
						flagCatCarrierId = -1;
						flagCat->isActive = true;
						flagCat->set_center(p.second->pos + 2.0f * p.second->front);
						buildQuadtree();
					}
					else if (p.second->isDead && flagDogCarrierId == p.first) {
						flagDogCarrierId = -1;
						flagDog->isActive = true;
						flagDog->set_center(p.second->pos + 2.0f * p.second->front);
						buildQuadtree();
					}

					break;
				}
			}

		}
	}

	Collider* queryRange = new Collider(players[playerId]->hitbox->cen, players[playerId]->hitbox->dim * 10.0f);
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
			players[playerId]->isGrounded = true;
		}

		players[playerId]->update(plane, 0.0f, 0.0f);

		// If it happened on no plane
		if (plane == glm::vec3(0.0f)) {
			continue;
		}

		if (otherCollider == flagDog) {
			cout << "Collided with dog " << playerId << " mod is " << playerId % 2 << " curr carrier id is " << flagDogCarrierId << endl;
		}

		// Flag collision
		if (otherCollider == flagCat && flagCatCarrierId == -1 && playerId % 2 == (int)PlayerTeam::CAT_LOVER) {
			flagCatCarrierId = playerId;
			flagCat->isActive = false;
		}
		else if (otherCollider == flagDog && flagDogCarrierId == -1 && playerId % 2 == (int)PlayerTeam::DOG_LOVER) {
			cout << "Setting dog carrier id" << endl;
			flagDogCarrierId = playerId;
			flagDog->isActive = false;
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
		//Update whether or not this player is carrying the flag
		bool carryingCatFlag = false;
		bool carryingDogFlag = false;
		if (flagCatCarrierId == i) {
			carryingCatFlag = true;
		}
		if (flagDogCarrierId == i) {
			carryingDogFlag = true;
		}

		PlayerState ps(i, players[i]->pos, players[i]->front, players[i]->animation, players[i]->isGrounded, players[i]->health, players[i]->isDead, carryingCatFlag, carryingDogFlag);

		gs.addState(ps);
	}

	return gs;
}

void ServerGameManager::createNewPlayer(int playerId) {
	glm::vec3 playerSpawnPos = (playerId % 2) == 0 ? glm::vec3(-10.0f, 3.0f, -10.0f) : glm::vec3(-10.0f, 15.0f, -5.0f);
	players[playerId] = new ServerPlayer(playerSpawnPos, playerId);
	// Add player hitboxes to all colliders
	allColliders.push_back(players[playerId]->hitbox);
}
