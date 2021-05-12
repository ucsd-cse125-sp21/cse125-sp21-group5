#include "ServerGameManager.h"
#include <glm/gtx/string_cast.hpp>

ServerGameManager::ServerGameManager() {
	// Creates box collider for the world and inits quadtree 
	vector<BoxCollider*> objects;
	qt = new Quadtree(this->worldBox, 4, objects);

	// TODO: remove, hardcoded initPos
	players.push_back(Player(glm::vec3(0.0f, 15.0f, 0.0f)));
	players.push_back(Player(glm::vec3(0.0f, -15.0f, 0.0f)));
	//players.push_back(Player(glm::vec3(5.0f, 0.0f, 0.0f)));
	//players.push_back(Player(glm::vec3(5.0f, 0.0f, 0.0f)));
	for (Player p : players) {
		allColliders.push_back(p.hitbox);
	}
	buildQuadtree();
}

MapState ServerGameManager::generateMap() {

	// Add one new box object into world
	allColliders.push_back(new BoxCollider(glm::vec3(0.0f), glm::vec3(1.0f)));

	// Add all colliders to MapState
	MapState ms;

	Collider* c = allColliders[0];
	glm::mat4 transform1 = glm::mat4(1);
	transform1 = glm::scale(transform1, glm::vec3(c->length, c->width, c->height));
	transform1 = glm::translate(transform1, c->center);

	c = allColliders[1];
	glm::mat4 transform2 = glm::mat4(1);
	transform2 = glm::scale(transform2, glm::vec3(c->length, c->width, c->height));
	transform2 = glm::translate(transform2, c->center);

	c = allColliders[2];
	glm::mat4 transform3 = glm::mat4(1);
	transform3 = glm::scale(transform3, glm::vec3(c->length, c->width, c->height));
	transform3 = glm::translate(transform3, c->center);

	vector<float> t1;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			t1.push_back(transform1[i][j]);
		}
	}
	vector<float> t2;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			t2.push_back(transform1[i][j]);
		}
	}
	vector<float> t3;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			t3.push_back(transform1[i][j]);
		}
	}

	ms.transform1 = t1;
	ms.transform2 = t2;
	ms.transform3 = t3;

	for (BoxCollider* c : allColliders) {
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

	cerr << "asdfasd" << endl;
	
	// if player is colliding
	// don't update player position
	// TODO: Possibly need to ignore ground 
	for (auto collider : allColliders) {
		BoxCollider colliderRange = BoxCollider(collider->center,
			glm::vec3(collider->length, collider->width, collider->height) * 2.0f);
		vector<BoxCollider*> nearbyColliders;
		nearbyColliders = qt->query(&colliderRange, nearbyColliders);
		//std::cerr << "Found " << nearbyColliders.size() << " in nearby colliders" << std::endl;
		
		// Actual collision 
		for (auto otherCollider : nearbyColliders) {
			// Collision happens
			if (collider != otherCollider) {
				// Determine which plane it collided with
				glm::vec3 plane = collider->intersects(otherCollider);
				
				// Did it actually collide?
				if (plane.x == 0.0f && plane.y == 0.0f && plane.z == 0.0f)
				{
					continue;
				}

				cerr << "the product is " << to_string(e.pos * plane) << endl;

				// Actual collision --> don't update player position
				glm::vec3 center = players[playerId].hitbox->center;
				BoxCollider* hitbox = players[playerId].hitbox;

				// zero out the dir the plane is in
				glm::vec3 newDir = e.pos * plane;

				// Edge case where the product is 0
				if (newDir.x != 0.0f || newDir.y != 0.0f || newDir.z != 0.0f) {
					newDir = glm::normalize(newDir);
				}
				
				// calculate projection to determine how much to move in other plane
				glm::vec3 newDelta = glm::length(e.pos) * newDir;

				cerr << "newDir is " << to_string(newDir) << endl;
				
				// Move player backwards
				players[playerId].update(newDelta - e.pos, 0.0f, 0.0f);
				
				// update position if the player "snaps" back
				buildQuadtree();

				// Collided with first thing 
				break;
			}
		}
		break;
	}
}

// Function that builds/rebuilds quadtree 
void ServerGameManager::buildQuadtree() {
	vector<BoxCollider*> objects;
	qt = new Quadtree(this->worldBox, 4, objects);
	// TODO: might run into issue if we constnatly remove things 
	// idea: hashtables 
	for (BoxCollider* bc : allColliders) {
		qt->insert(bc);
	}
}

GameState ServerGameManager::getGameState(int playerId) {
	return GameState(players[playerId].pos, players[playerId].front);
}
