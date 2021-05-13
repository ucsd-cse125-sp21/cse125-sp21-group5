#include "ServerGameManager.h"
#include <glm/gtx/string_cast.hpp>

ServerGameManager::ServerGameManager() {
	// Creates box collider for the world and inits quadtree 
	vector<Collider*> objects;
	qt = new Quadtree(this->worldBox, 4, objects);

	// TODO: remove, hardcoded initPos
	players.push_back(Player(glm::vec3(0.0f, 15.0f, 0.0f)));
	//players.push_back(Player(glm::vec3(0.0f, -15.0f, 0.0f)));
	//players.push_back(Player(glm::vec3(5.0f, 0.0f, 0.0f)));
	//players.push_back(Player(glm::vec3(5.0f, 0.0f, 0.0f)));
	for (Player p : players) {
		allColliders.push_back(p.hitbox);
	}

	//Set up flag
	allColliders.push_back(flag1);

	buildQuadtree();
}

MapState ServerGameManager::generateMap() {

	// Add one new box object into world
	//Collider* box = new Collider(glm::vec3(0.0f, -1.0f, 0), glm::vec3(20.0f));

	// Add all colliders to MapState


	Collider* c = allColliders[1];

	MapState ms;
	glm::mat4 transform1 = glm::mat4(1);
	transform1 = glm::scale(transform1, glm::vec3(c->length, c->width, c->height));
	transform1 = glm::translate(transform1, c->center);

	//Set up float vector to send
	vector<float> t1;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			t1.push_back(transform1[i][j]);
		}
	}

	ms.transform1 = t1;

	for (Collider* c : allColliders) {
		for (int i = 0; i < 8; i++)
		{
			std::cerr << "point " << i << ":" << glm::to_string(c->points[i]) << std::endl;
		}
	}

	return ms;
}

void ServerGameManager::handleEvent(Event& e, int playerId) {
	// Calculate where player wants to be
	players[playerId].update(e.pos, e.yaw, e.pitch);

	// Remove and readd player collider
	buildQuadtree();
	
	// if player is colliding
	// don't update player position
	// TODO: Possibly need to ignore ground 
	for (auto collider : allColliders) {
		Collider colliderRange = Collider(collider->center,
			glm::vec3(collider->length, collider->width, collider->height) * 2.0f);
		vector<Collider*> nearbyColliders;
		nearbyColliders = qt->query(&colliderRange, nearbyColliders);
		
		
		// Actual collision 
		for (Collider* otherCollider : nearbyColliders) {
			// Collision happens
			if (collider != otherCollider) {
				// Determine which plane it collided with
				glm::vec3 plane = collider->intersects(otherCollider);
				
				// Did it actually collide?
				if (plane.x == 0.0f && plane.y == 0.0f && plane.z == 0.0f)
				{
					continue;
				}

				// Actual collision --> don't update player position
				glm::vec3 center = players[playerId].hitbox->center;
				Collider* hitbox = players[playerId].hitbox;

				// zero out the dir the plane is in
				glm::vec3 newDir = e.pos * plane;

				// Edge case where the product is 0
				if (newDir.x != 0.0f || newDir.y != 0.0f || newDir.z != 0.0f) {
					newDir = glm::normalize(newDir);
				}
				
				// calculate projection to determine how much to move in other plane
				glm::vec3 newDelta = glm::length(e.pos) * newDir;

				// Move player backwards
				players[playerId].update(newDelta - e.pos, 0.0f, 0.0f);
				
				// update position if the player "snaps" back
				buildQuadtree();

				//If the collider is the flag, have it follow the player
				if (flagCarrierId == -1 && otherCollider == flag1) {
					flagCarrierId = playerId;
				}

				// Collided with first thing 
				break;
			}
		}
		break;
	}
}

// Function that builds/rebuilds quadtree 
void ServerGameManager::buildQuadtree() {
	vector<Collider*> objects;
	qt = new Quadtree(this->worldBox, 4, objects);
	// TODO: might run into issue if we constnatly remove things 
	// idea: hashtables 
	for (Collider* bc : allColliders) {
		qt->insert(bc);
	}
}

GameState ServerGameManager::getGameState(int playerId) {
	if (flagCarrierId != -1) {
		flag1->center = players[playerId].hitbox->center - glm::vec3(0, 3, 0);
	}

	glm::mat4 transform1 = glm::mat4(1);
	transform1 = glm::scale(transform1, glm::vec3(flag1->length, flag1->width, flag1->height));
	transform1 = glm::translate(transform1, flag1->center);

	//Set up float vector to send
	vector<float> flagPosVec;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			flagPosVec.push_back(transform1[i][j]);
		}
	}

	return GameState(players[playerId].pos, players[playerId].front, flagPosVec);
}
