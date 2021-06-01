#include "ServerGameManager.h"
#include <glm/gtx/string_cast.hpp>


ServerGameManager::ServerGameManager() {

	// TODO: remove, hardcoded initPos
	// TODO: add new player colliders as players connect
	tileSeed = (int)time(NULL);

	//Tile and flags are unintialized
	flagCatCarrierId = -1;
	flagDogCarrierId = -1;

	gameCountdown = -1;
}

MapState ServerGameManager::generateMap()
{
	// Set the random seed, so thing generate the same way
	srand(tileSeed);

	// Create one massive tile for entirety of map
	Collider* tileC = new Collider(ObjectType::GROUND, glm::vec3(0.0f, -2.5f, 0.0f), glm::vec3(TILE_SIZE * NUM_MAP_TILES, 5.0f, TILE_SIZE * NUM_MAP_TILES));
	allColliders.push_back(tileC);

	for (int i = 0; i < NUM_MAP_TILES; i++)
	{
		for (int j = 0; j < NUM_MAP_TILES; j++)
		{
			// Center point of the tile
			glm::vec3 tileCenter = glm::vec3(TILE_SIZE * (i - NUM_MAP_TILES / 2), 0.0f, TILE_SIZE * (j - NUM_MAP_TILES / 2));

			// Special case for flags
			if (i == 0 && j == 0)
			{
				flagCat = new Collider(ObjectType::FLAG_CAT, tileCenter + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f));
				allColliders.push_back(flagCat);
				continue;
			}
			else if (i == NUM_MAP_TILES - 1 && j == NUM_MAP_TILES - 1)
			{
				flagDog = new Collider(ObjectType::FLAG_DOG, tileCenter + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f));
				allColliders.push_back(flagDog);
				continue;
			}

			// Generate Trees
			int numTrees = rand() % MAX_NUM_TREES_PER_TILE;
			for (int k = 0; k < numTrees; k++)
			{
				// Pick random position within tile
				float x = TILE_SIZE * (rand() / (float)RAND_MAX) - 10.0f;
				float z = TILE_SIZE * (rand() / (float)RAND_MAX) - 10.0f;

				// Make tree collider
				Collider* treeC = new Collider(ObjectType::ENVIRONMENT, glm::vec3(x, 5.0f, z) + tileCenter, glm::vec3(1.0f, 10.0f, 1.0f));
				rand(); // for random tree models on client
				rand(); // for rotations on client
				allColliders.push_back(treeC);
			}
		}
	}

	// Create capture areas at opposite corners
	dogWinArea = new Collider(ObjectType::ENVIRONMENT, flagCat->cen, flagCat->dim + glm::vec3(1.0f));
	catWinArea = new Collider(ObjectType::ENVIRONMENT, flagDog->cen, flagDog->dim + glm::vec3(1.0f));

	// Set up quadtree 
	Collider boundary = Collider(glm::vec3(0.0f), glm::vec3(TILE_SIZE * (NUM_MAP_TILES + 1), 60.0f, TILE_SIZE * (NUM_MAP_TILES + 1)));
	qt = new Quadtree(boundary, 4);
	for (Collider* c : allColliders)
	{
		qt->insert(c);
	}

	// Create map border
	float tmp = TILE_SIZE * NUM_MAP_TILES;
	allColliders.push_back(new Collider(glm::vec3(tmp / 2 + 1.0f, 0.0f, 0.0f), glm::vec3(2.0f, 50.0f, tmp)));
	allColliders.push_back(new Collider(glm::vec3(-(tmp / 2 + 1.0f), 0.0f, 0.0f), glm::vec3(2.0f, 50.0f, tmp)));
	allColliders.push_back(new Collider(glm::vec3(0.0f, 0.0f, tmp / 2 + 1.0f), glm::vec3(tmp, 50.0f, 2.0f)));
	allColliders.push_back(new Collider(glm::vec3(0.0f, 0.0f, -(tmp / 2 + 1.0f)), glm::vec3(tmp, 50.0f, 2.0f)));

	// Create map state
	return MapState(tileSeed);
}

void ServerGameManager::handleShoot(ServerPlayer* player)
{
	// Select current gun
	Gun* gun = player->guns[player->gun_idx];

	// Check gun stuff
	if (!gun->fire())
	{
		return;
	}

	// Player has shot
	player->isShooting = true;

	// Trace each bullet
	for (int i = 0; i < gun->bullets_per_shot; i++)
	{
		// For calculating min distance
		float minHitlength = std::numeric_limits<float>::infinity();
		Collider* closestCollider = nullptr;

		// Calculate Scatter
		float x = (float)rand() / (float)RAND_MAX - 0.5f;
		float y = (float)rand() / (float)RAND_MAX - 0.5f;
		glm::vec3 scatter = glm::vec3(gun->x_spread * x, gun->y_spread * y, 0.0f);
		glm::vec3 trajectory = glm::normalize(player->front + scatter);

		// Check shooting against all other colliders before checking movement 
		for (Collider* otherCollider : allColliders)
		{
			// Ignore collisions with yourself
			if (player->hitbox == otherCollider)
				continue;

			// Check for shooting stuff
			glm::vec3 hitPos;
			if (otherCollider->check_ray_collision(player->hitbox->cen, trajectory, hitPos))
			{
				float hitLength = glm::length(hitPos - player->hitbox->cen);
				if (hitLength < minHitlength)
				{
					minHitlength = hitLength;
					closestCollider = otherCollider;
				}
			}
		}

		// Handle Hit Damage
		if (closestCollider != nullptr && closestCollider->type == ObjectType::PLAYER)
		{
			// TODO: maybe find constant time way to do this
			for (auto p : players)
			{
				if (p.second->hitbox == closestCollider)
				{
					//Set their fov and stuff man
					if (player->gun_idx == 1) {
						switch (player->playerClass) {
							case 0: 
								p.second->isLimitFOV = VISION_IMPAIRMENT_TIME;
								break;
							case 1: 
								p.second->isFogged = VISION_IMPAIRMENT_TIME;
								break;
							case 2:
								p.second->isFrozen = FREEZE_TIME;
								break;
						}
					}

					p.second->decreaseHealth(gun->damage_per_bullet);

					// Check if player died
					if (p.second->isDeadCheck())
					{
						// Set death timer
						p.second->isDead = DEATH_TICK_TIMER;

						// Disable hitbox
						p.second->hitbox->isActive = false;

						// Increase death count
						p.second->deaths++;

						// Drop corresponding flag
						if (flagCatCarrierId == p.first)
						{
							flagCatCarrierId = -1;
							flagCat->isActive = true;
							glm::vec3 offsetVec = p.second->pos - (2.0f * glm::normalize(glm::vec3(p.second->front.x, 0, p.second->front.z)));
							flagCat->set_center(offsetVec - glm::vec3(0, 0.85f, 0));
						}
						else if (flagDogCarrierId == p.first)
						{
							flagDogCarrierId = -1;
							flagDog->isActive = true;
							glm::vec3 offsetVec = p.second->pos - 2.0f * glm::normalize(glm::vec3(p.second->front.x, 0, p.second->front.z));
							flagDog->set_center(offsetVec - glm::vec3(0, 0.85f, 0));
						}
						buildQuadtree();

						// Increment kill count
						player->kills++;
					}
					break;
				}
			}
		}
	}

	// Add recoil
	player->update(glm::vec3(0.0f), 0.0f, gun->recoil);
}

void ServerGameManager::handleEvent(Event& e, int playerId)
{
	// If a team has won
	if (catTeamWin) {
		// Changes win animation
		for (auto player : players) {
			// For cat players 
			if (player.first % 2 == (int)PlayerTeam::CAT_LOVER) {
				player.second->animation = AnimationID::DAB;
			}
		}
		return;
	}
	else if (dogTeamWin) {
		// Changes win animation
		for (auto player : players) {
			// For cat players 
			if (player.first % 2 == (int)PlayerTeam::DOG_LOVER) {
				player.second->animation = AnimationID::DAB;
			}
		}
		return;
	}

	//uint8_t gameOver = 0, 1, 2;
	//if (!gameOver && (catTeamWin || dogTeamWin))
	//{
	//	resetGame();

	//}

	// Get the current player
	ServerPlayer* curr_player = players[playerId];

	curr_player->isShooting = false;

	// If game didn't start yet, allow players to change class but not move
	if (!gameStarted) {
		if (e.playerClass == 0) {
			players[playerId]->guns.clear();
			players[playerId]->playerClass = 0;
			players[playerId]->guns.push_back(new Pistol());
			players[playerId]->guns.push_back(new FOV());
		}
		else if (e.playerClass == 1) {
			players[playerId]->guns.clear();
			players[playerId]->playerClass = 1;
			players[playerId]->guns.push_back(new Shotgun());
			players[playerId]->guns.push_back(new FOG());
		}
		else if (e.playerClass == 2) {
			players[playerId]->guns.clear();
			players[playerId]->playerClass = 2;
			players[playerId]->guns.push_back(new Rifle());
			players[playerId]->guns.push_back(new Stun());
		}
	}

	// Game not started yet
	if (gameCountdown > 0) {
		gameCountdown--;

		// If game did not start, don't let the players move
		if (gameCountdown == 0) {
			// Set game started; will teleport players back to spawn point
			gameStarted = true;
			cout << "Starting the game" << endl;
			startGame();
		}
	}
	// Waiting for all players to connect.
	else if (gameCountdown == -1) {
		// TODO: delete?
		//return;
	}

	// Special gun effects
	if (curr_player->isLimitFOV > 0) {
		curr_player->isLimitFOV--;
	}
	if (curr_player->isFogged > 0) {
		curr_player->isFogged--;
	}
	if (curr_player->isFrozen > 0) {
		curr_player->isFrozen--;
		e.dPos = glm::vec3(0, 0, 0);
		e.dYaw = 0;
		e.dPitch = 0;
		e.shooting = false;
		e.jumping = false;
	}

	// If player is dead/respawning
	if (curr_player->isDead > 0)
	{
		// Update animation for death
		players[playerId]->animation = AnimationID::DEATH;
		players[playerId]->isDead--;
		
		// Reset variables when they are alive again
		if (players[playerId]->isDead == 0)
		{
			// TODO: Reset player (including gun variables)
			players[playerId]->health = 100.0f;
			players[playerId]->hitbox->isActive = true;

			// TP to cat base
			if (players[playerId]->team == PlayerTeam::CAT_LOVER) {
				players[playerId]->pos = CAT_SPAWN;
			}
			else if (players[playerId]->team == PlayerTeam::DOG_LOVER) {
				players[playerId]->pos = DOG_SPAWN;
			}
		}
		// If the player is dead, we don't let them move or do anything
		return;
	}

	// Switch weapons
	curr_player->gun_idx = e.gun_idx;

	// Pick animation
	curr_player->updateAnimations(e);

	// Check if player has fallen off map
	if (curr_player->pos.y < -10.0f)
	{
		// Slowly decrement health
		curr_player->decreaseHealth(10.0f);
	
		// If player dies, return flag back to area
		if (curr_player->isDeadCheck())
		{
			// Set death timer
			curr_player->isDead = DEATH_TICK_TIMER;

			// Disable hitbox
			curr_player->hitbox->isActive = false;

			// Increase death count
			curr_player->deaths++;

			if (flagCatCarrierId == playerId)
			{
				flagCatCarrierId = -1;
				flagCat->set_center(CAT_FLAG_SPAWN);
				flagCat->isActive = true;
			}
			else if (flagDogCarrierId == playerId)
			{
				flagDogCarrierId = -1;
				flagDog->set_center(DOG_FLAG_SPAWN);
				flagDog->isActive = true;
			}

			buildQuadtree();
		}
	}

	// Not jumping
	if (!e.jumping)
	{
		if (curr_player->vVelocity >= 0.0f)
		{
			curr_player->vVelocity -= 0.1f;
		}
		curr_player->update(e.dPos + glm::vec3(0.0f, curr_player->vVelocity, 0.0f), e.dYaw, e.dPitch);
	}
	// Jumping 
	else {
		// 10 ticks of jumping in total
		curr_player->jumping = 10;
	}

	// Parabolic jumping
	float jumpingSquared = curr_player->jumping * curr_player->jumping;
	// Handle jumping tick by tick
	if (curr_player->jumping > 0)
	{
		curr_player->update(e.dPos + glm::vec3(0.0f, jumpingSquared/100.0f, 0.0f), e.dYaw, e.dPitch);
		// 5 ticks of jumping in total
		curr_player->jumping--;
	}

	// Rebuild quadtree for collision after player movement is updated
	buildQuadtree();

	// Update gun ticks
	for (Gun* g : curr_player->guns)
	{
		g->decrement_fire_rate();
		g->decrement_reload_time();
	}

	// Handle shooting
	if (e.shooting)
	{
		handleShoot(curr_player);
	}

	// Movement for colliders 
	Collider* queryRange = new Collider(curr_player->hitbox->cen, curr_player->hitbox->dim * 10.0f);
	vector<Collider*> nearbyColliders;
	nearbyColliders = qt->query(queryRange, nearbyColliders);
	curr_player->isGrounded = false;
	for (Collider* otherCollider : nearbyColliders)
	{
		// Ignore collisions with yourself
		if (curr_player->hitbox == otherCollider)
			continue;

		// Determine which plane collision happened on
		glm::vec3 plane = curr_player->hitbox->check_collision(otherCollider);

		// For jumping
		if (plane.y > 0.0f)
		{
			curr_player->isGrounded = true;
		}

		curr_player->update(plane, 0.0f, 0.0f);

		// If it happened on no plane
		if (plane == glm::vec3(0.0f))
			continue;

		// Pick up flag
		if (otherCollider == flagCat && flagCatCarrierId == -1 && playerId % 2 == (int)PlayerTeam::CAT_LOVER) {
			flagCatCarrierId = playerId;
			flagCat->isActive = false;
		}
		else if (otherCollider == flagDog && flagDogCarrierId == -1 && playerId % 2 == (int)PlayerTeam::DOG_LOVER) {
			flagDogCarrierId = playerId;
			flagDog->isActive = false;
		}
	}

	// Move cat with player
	if (flagCatCarrierId != -1)
	{
		glm::vec3 offsetVec = players[flagCatCarrierId]->pos - 2.0f * glm::normalize(glm::vec3(players[flagCatCarrierId]->front.x, 0, players[flagCatCarrierId]->front.z));
		flagCat->set_center(offsetVec - glm::vec3(0, 0.85f, 0));
	}

	// Move dog with player
	if (flagDogCarrierId != -1)
	{
		glm::vec3 offsetVec = players[flagDogCarrierId]->pos - 2.0f * glm::normalize(glm::vec3(players[flagDogCarrierId]->front.x, 0, players[flagDogCarrierId]->front.z));
		flagDog->set_center(offsetVec - glm::vec3(0, 0.85f, 0));
	}

	// Detect score for Cat team
	if (flagCatCarrierId == playerId && curr_player->team == PlayerTeam::CAT_LOVER)
	{
		glm::vec3 plane = curr_player->hitbox->check_collision(catWinArea);
		if (plane != glm::vec3(0.0f))
		{
			curr_player->captures++;

			// Reset cat flag
			flagCatCarrierId = -1;
			flagCat->set_center(CAT_FLAG_SPAWN);
			flagCat->isActive = true;
			checkWinCondition();
		}
			
	}
	else if(flagDogCarrierId == playerId && curr_player->team == PlayerTeam::DOG_LOVER)
	{
		glm::vec3 plane = curr_player->hitbox->check_collision(dogWinArea);
		if (plane != glm::vec3(0.0f))
		{
			curr_player->captures++;

			// Reset dog flag
			flagDogCarrierId = -1;
			flagDog->set_center(DOG_FLAG_SPAWN);
			flagDog->isActive = true;
			checkWinCondition();
		}
	}

	// Dab god damnit sheila 
	if (e.dab) {
		players[playerId]->animation = AnimationID::DAB;
	}
	buildQuadtree();
}

void ServerGameManager::buildQuadtree()
{
	Collider boundary = Collider(glm::vec3(0.0f), glm::vec3(TILE_SIZE * (NUM_MAP_TILES + 1), 60.0f, TILE_SIZE * (NUM_MAP_TILES + 1)));
	qt = new Quadtree(boundary, 4);

	for (Collider* c : allColliders)
	{
		qt->insert(c);
	}
}

void ServerGameManager::checkWinCondition() 
{
	int dogTeamPoints = 0;
	int catTeamPoints = 0;
	
	// Loop to calculate scores 
	for (auto player : players) {

		// For cat players 
		if (player.first % 2 == (int)PlayerTeam::CAT_LOVER) {
			catTeamPoints += player.second->captures;
		}
		// For dog players 
		else if (player.first % 2 == (int)PlayerTeam::DOG_LOVER) {
			dogTeamPoints += player.second->captures;
		}
	}

	// Check cat team points 
	if (catTeamPoints == NUM_CAPTURES_TO_WIN) {
		// end the game
		catTeamWin = true;
	}
	if (dogTeamPoints == NUM_CAPTURES_TO_WIN) {
		// end the game 
		dogTeamWin = true;
	}

	// Disable winhitboxes
	// TODO: call reset game here

	
}

GameState ServerGameManager::getGameState(int playerId)
{
	GameState gs;

	//cout << "Printing out Game State:" << endl;

	for (int i = 0; i < players.size(); i++)
	{
		PlayerState ps(i,
						players[i]->pos,
						players[i]->front,
						players[i]->animation,
						players[i]->isGrounded,
						players[i]->health,
						players[i]->isDead,
						(flagCatCarrierId == i),
						(flagDogCarrierId == i),
						players[i]->kills,
						players[i]->deaths,
						players[i]->captures,
						players[i]->gun_idx,
						*(players[i]->guns[players[i]->gun_idx]),
						players[i]->isShooting,
						players[i]->isLimitFOV,
						players[i]->isFogged,
						players[i]->isFrozen,
						players[i]->playerClass
		);
		gs.addState(ps);
	}

	// Send back flag locations
	gs.catLocation = flagCat->cen;
	gs.dogLocation = flagDog->cen;

	// Sends back game-winning variables 
	gs.catTeamWin = catTeamWin;
	gs.dogTeamWin = dogTeamWin;

	gs.gameCountdown = gameCountdown;

	return gs;
}

void ServerGameManager::createNewPlayer(int playerId)
{
	glm::vec3 playerSpawnPos;
	float initYaw;
	if (playerId % 2 == (int)PlayerTeam::CAT_LOVER)
	{
		playerSpawnPos = CAT_SPAWN;
		initYaw = 225;
	}
	else
	{
		playerSpawnPos = DOG_SPAWN;
		initYaw = 45;
	}
	players[playerId] = new ServerPlayer(playerSpawnPos, initYaw, 0, playerId);


	// Add player hitboxes to all colliders
	allColliders.push_back(players[playerId]->hitbox);
}

void ServerGameManager::startGame()
{
	for (auto p : players)
	{
		int playerId = p.first;
		glm::vec3 playerSpawnPos;
		float initYaw;
		if (playerId % 2 == (int)PlayerTeam::CAT_LOVER) {
			playerSpawnPos = CAT_SPAWN;
			initYaw = 225;
		}
		else {
			playerSpawnPos = DOG_SPAWN;
			initYaw = 45;
		}
		players[p.first]->resetPlayer(playerSpawnPos, initYaw, 0);
	}
}

void ServerGameManager::respawnPlayerWithID(int playerId, glm::vec3 pos, float yaw, float pitch)
{
	players[playerId]->respawn(pos, yaw, pitch);
}