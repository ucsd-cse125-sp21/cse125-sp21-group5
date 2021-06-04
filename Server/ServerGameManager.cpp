#include "ServerGameManager.h"
#include <glm/gtx/string_cast.hpp>

ServerGameManager::ServerGameManager()
{
	// Set the seed
	tileSeed = (int)time(NULL);

	// Flags are not picked up by anyone
	flagCatCarrierId = -1;
	flagDogCarrierId = -1;

	gameCountdown = GAME_COUNTDOWN_TIMER;

	// Set the game in lobby state
	gameStatus = State::LOBBY_STATE;
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
			int numTrees = rand() % (MAX_NUM_TREES_PER_TILE - MIN_NUM_TREES_PER_TILE) + MIN_NUM_TREES_PER_TILE;
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
				if (p.second->hitbox == closestCollider && p.second->team != player->team)
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

void ServerGameManager::handleMovement(ServerPlayer* player, int playerId, Event& e)
{
	// Not jumping
	if (!e.jumping)
	{
		if (player->vVelocity >= 0.0f)
			player->vVelocity -= 0.3f;
		player->update(e.dPos + glm::vec3(0.0f, player->vVelocity, 0.0f), e.dYaw, e.dPitch);
	}
	// Jumping 
	else {
		// 10 ticks of jumping in total
		player->jumping = 10;
	}

	// Parabolic jumping
	float jumpingSquared = player->jumping * player->jumping;
	// Handle jumping tick by tick
	if (player->jumping > 0)
	{
		player->update(e.dPos + glm::vec3(0.0f, jumpingSquared / 200.0f, 0.0f), e.dYaw, e.dPitch);
		// 5 ticks of jumping in total
		player->jumping--;
	}

	// Rebuild quadtree for collision after player movement is updated
	buildQuadtree();

	// Movement for colliders 
	Collider* queryRange = new Collider(player->hitbox->cen, player->hitbox->dim * 10.0f);
	vector<Collider*> nearbyColliders;
	nearbyColliders = qt->query(queryRange, nearbyColliders);
	player->isGrounded = false;
	for (Collider* otherCollider : nearbyColliders)
	{
		// Ignore collisions with yourself
		if (player->hitbox == otherCollider)
			continue;

		// Determine which plane collision happened on
		glm::vec3 plane = player->hitbox->check_collision(otherCollider);

		// For jumping
		if (plane.y > 0.0f)
		{
			player->isGrounded = true;
		}

		player->update(plane, 0.0f, 0.0f);

		// If it happened on no plane
		if (plane == glm::vec3(0.0f))
			continue;

		// Pick up flag
		if (otherCollider == flagCat && flagCatCarrierId == -1){
			if (player->team == PlayerTeam::CAT_LOVER) {
				flagCatCarrierId = playerId;
				flagCat->isActive = false;
			}
			else
			{
				// Reset Cat Flag
				flagCatCarrierId = -1;
				flagCat->set_center(CAT_FLAG_SPAWN);
				flagCat->isActive = true;
			}
		}
		else if (otherCollider == flagDog && flagDogCarrierId == -1)
		{
			if (player->team == PlayerTeam::DOG_LOVER) {
				flagDogCarrierId = playerId;
				flagDog->isActive = false;
			}
			else {
				// Reset Dog Flag
				flagDogCarrierId = -1;
				flagDog->set_center(DOG_FLAG_SPAWN);
				flagDog->isActive = true;
			}
		}
	}

	// Rebuild quadtree for collision after player movement is updated
	buildQuadtree();
}

// Forces specified team to dab
void ServerGameManager::forceDab(PlayerTeam team)
{
	for (auto player : players)
		if (player.second->team == team)
			player.second->animation = AnimationID::DAB;
}

void ServerGameManager::switchClass(int playerId, int playerClass)
{
	// Do nothing if already right class
	if (players[playerId]->playerClass == playerClass)
		return;

	// Clear out player's gun list and replace with appropriate ones
	if (playerClass == 0) {
		players[playerId]->guns.clear();
		players[playerId]->playerClass = 0;
		players[playerId]->guns.push_back(new Pistol());
		players[playerId]->guns.push_back(new FOV());
	}
	else if (playerClass == 1) {
		players[playerId]->guns.clear();
		players[playerId]->playerClass = 1;
		players[playerId]->guns.push_back(new Shotgun());
		players[playerId]->guns.push_back(new FOG());
	}
	else if (playerClass == 2) {
		players[playerId]->guns.clear();
		players[playerId]->playerClass = 2;
		players[playerId]->guns.push_back(new Rifle());
		players[playerId]->guns.push_back(new Stun());
	}
}

void ServerGameManager::handleDeath(ServerPlayer* player)
{
	// Update animation for death
	player->animation = AnimationID::DEATH;
	player->isDead--;

	// Reset variables when they are alive again
	if (player->isDead == 0)
	{
		// TP to cat base
		if (player->team == PlayerTeam::CAT_LOVER)
			player->respawn(CAT_SPAWN, CAT_YAW, 0.0f);
		else if (player->team == PlayerTeam::DOG_LOVER)
			player->respawn(DOG_SPAWN, DOG_YAW, 0.0f);
	}
}

void ServerGameManager::handleEvent(Event& e, int playerId)
{
	animalSoundTimer++;
	// Get current player
	ServerPlayer* player = players[playerId];

	// Handle events based on what stage in the game we are in
	switch (gameStatus)
	{
	// This is where players wait for all other players to join
	// This is also where players can test out the different classes and roam the map
	case State::LOBBY_STATE:
		gameCountdown = -1;
		gameOverCountdown = GAMEOVER_TIMER;
		winningTeam = PlayerTeam::NOBODY;
		switchClass(playerId, e.playerClass);

		player->isShooting = false;
		//if (e.shooting) handleShoot(player);
		handleMovement(player, playerId, e);

		//Set the flag to be not picked up
		resetFlags();

		if (e.isReady) {
			player->isReady = true;
		}

		// Do nothing if any one player is not ready
		if (players.size() != NUM_PLAYERS)
			return;
		for (auto p : players)
			if (!p.second->isReady)
				return;
		
		
		// All players must be ready, start countdown
		gameStatus = State::READY_STATE;
		gameCountdown = GAME_COUNTDOWN_TIMER;

		// Respawn players 
		resetPlayersToSpawn();

		break;
	case State::READY_STATE:
		// Tick game countdown
		if (gameCountdown > 0)
		{
			// The tick after it hits 0 will be when game switches
			gameCountdown--;
			return;
		}

		// Enter the play state
		gameStatus = State::PLAY_STATE;
		break;
	case State::PLAY_STATE:
		player->isDisrespecting = e.isDisrespecting;
		
		// Movement

		// Check if player has fallen off map
		// TODO: maybe helper function or remove
		if (player->pos.y < -10.0f)
		{
			// Slowly decrement health
			player->decreaseHealth(10.0f);

			// If player dies, return flag back to area
			if (player->isDeadCheck())
			{
				// Set death timer
				player->isDead = DEATH_TICK_TIMER;

				// Disable hitbox
				player->hitbox->isActive = false;

				// Increase death count
				player->deaths++;

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

		// Apply special gun effects, and tick effects
		// TODO: helper function
		if (player->isLimitFOV > 0) {
			player->isLimitFOV--;
		}
		if (player->isFogged > 0) {
			player->isFogged--;
		}
		if (player->isFrozen > 0) {
			player->isFrozen--;
			e.dPos = glm::vec3(0.0f);
			e.dYaw = 0.0f;
			e.dPitch = 0.0f;
			e.shooting = false;
			e.jumping = false;
		}

		// If player is dead/respawning
		if (player->isDead > 0)
		{
			handleDeath(player);
			// If the player is dead, we don't let them move or do anything
			return;
		}

		player->isShooting = false;
		if (e.shooting) handleShoot(player);
		handleMovement(player, playerId, e);

		// Switch weapons
		player->gun_idx = e.gun_idx;

		// Pick animation
		player->updateAnimations(e);

		// Update gun ticks
		for (Gun* g : player->guns)
		{
			g->decrement_fire_rate();
			g->decrement_reload_time();
		}

		// Make user dab
		if (e.dab) players[playerId]->animation = AnimationID::DAB;

		buildQuadtree();

		// Move flags with player
		if (flagCatCarrierId != -1)
		{
			glm::vec3 offsetVec = players[flagCatCarrierId]->pos - 2.0f * glm::normalize(glm::vec3(players[flagCatCarrierId]->front.x, 0, players[flagCatCarrierId]->front.z));
			flagCat->set_center(offsetVec - glm::vec3(0, 0.85f, 0));
		}
		if (flagDogCarrierId != -1)
		{
			glm::vec3 offsetVec = players[flagDogCarrierId]->pos - 2.0f * glm::normalize(glm::vec3(players[flagDogCarrierId]->front.x, 0, players[flagDogCarrierId]->front.z));
			flagDog->set_center(offsetVec - glm::vec3(0, 0.85f, 0));
		}

		// Detect score for Cat team
		// TODO: helper function
		if (flagCatCarrierId == playerId && player->team == PlayerTeam::CAT_LOVER)
		{
			// Can capture flag only if their own flag is in their base.
			if (flagDog->cen == DOG_FLAG_SPAWN) {
				glm::vec3 plane = player->hitbox->check_collision(catWinArea);
				if (plane != glm::vec3(0.0f))
				{
					player->captures++;

					// Reset cat flag
					flagCatCarrierId = -1;
					flagCat->set_center(CAT_FLAG_SPAWN);
					flagCat->isActive = true;
					if (checkWinCondition()) gameStatus = State::GAMEOVER_STATE;
				}
			}
		}
		// Detect score for Dog team
		else if (flagDogCarrierId == playerId && player->team == PlayerTeam::DOG_LOVER)
		{
			// Can capture flag only if their own flag is in their base.
			if (flagCat->cen == CAT_FLAG_SPAWN) {
				glm::vec3 plane = player->hitbox->check_collision(dogWinArea);
				if (plane != glm::vec3(0.0f))
				{
					player->captures++;

					// Reset dog flag
					flagDogCarrierId = -1;
					flagDog->set_center(DOG_FLAG_SPAWN);
					flagDog->isActive = true;
					if (checkWinCondition()) gameStatus = State::GAMEOVER_STATE;
				}
			}
		}

		break;
	case State::GAMEOVER_STATE:
		// Force winning team to dab
		forceDab(winningTeam);

		// Let winners enjoy a 10 second dab tick timer
		if (gameOverCountdown > 0)
		{
			gameOverCountdown--;
			return;
		}

		if (gameOverCountdown == 0)
		{
			gameOverCountdown = GAMEOVER_TIMER;
			resetPlayersToSpawn();
		}

		// Once, timer is over, Reset GameCountdown Timer
		// gameCountdown = GAME_COUNTDOWN_TIMER;

		// Change state
		gameStatus = State::LOBBY_STATE;
		gameCountdown = -1;
		break;
	}
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

// If there is a tie, the Cat Team wins
// Will also transition state to winning state
bool ServerGameManager::checkWinCondition() 
{
	int dogTeamPoints = 0;
	int catTeamPoints = 0;
	
	// Loop to calculate scores 
	for (auto player : players) {

		// For cat players 
		if (player.second->team == PlayerTeam::CAT_LOVER)
			catTeamPoints += player.second->captures;
		// For dog players 
		else if (player.second->team == PlayerTeam::DOG_LOVER)
			dogTeamPoints += player.second->captures;
	}

	// Assign winner
	if (catTeamPoints == NUM_CAPTURES_TO_WIN)
		winningTeam = PlayerTeam::CAT_LOVER;
	else if (dogTeamPoints == NUM_CAPTURES_TO_WIN)
		winningTeam = PlayerTeam::DOG_LOVER;

	return winningTeam != PlayerTeam::NOBODY;
}

GameState ServerGameManager::getGameState(int playerId)
{
	GameState gs;

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
			players[i]->playerClass,
			players[i]->isReady,
			players[i]->isDisrespecting
		);

		gs.addState(ps);
	}

	// cat and dog makes some noise every 5 seconds 
	if (animalSoundTimer % 180 == 0) gs.animalSound = true;
	else gs.animalSound = false;

	// Send back flag locations
	gs.catLocation = flagCat->cen;
	gs.dogLocation = flagDog->cen;

	// Sends back game-winning variables 
	gs.winningTeam = winningTeam;

	gs.gameCountdown = gameCountdown;

	return gs;
}

void ServerGameManager::createNewPlayer(int playerId)
{
	glm::vec3 spawnPos;
	float initYaw;
	if (playerId % 2 == (int)PlayerTeam::CAT_LOVER)
	{
		spawnPos = CAT_SPAWN;
		initYaw = CAT_YAW;
	}
	else
	{
		spawnPos = DOG_SPAWN;
		initYaw = DOG_YAW;
	}
	players[playerId] = new ServerPlayer(spawnPos, initYaw, 0, playerId);

	// Add player hitboxes to all colliders
	allColliders.push_back(players[playerId]->hitbox);
}

void ServerGameManager::resetFlags() {
	flagCatCarrierId = -1;
	flagDogCarrierId = -1;

	flagCat->set_center(CAT_FLAG_SPAWN);
	flagDog->set_center(DOG_FLAG_SPAWN);

	flagCat->isActive = true;
	flagDog->isActive = true;
}

void ServerGameManager::resetPlayersToSpawn()
{
	glm::vec3 spawnPos;
	float initYaw;

	resetFlags();

	for (auto p : players)
	{
		if (p.second->team == PlayerTeam::CAT_LOVER)
		{
			spawnPos = CAT_SPAWN;
			initYaw = CAT_YAW;
		}
		else
		{
			spawnPos = DOG_SPAWN;
			initYaw = DOG_YAW;
		}
		p.second->resetPlayer(spawnPos, initYaw, 0);
	}
}

void ServerGameManager::respawnPlayerWithID(int playerId, glm::vec3 pos, float yaw, float pitch)
{
	players[playerId]->respawn(pos, yaw, pitch);
}