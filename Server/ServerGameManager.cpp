#include "ServerGameManager.h"
#include <glm/gtx/string_cast.hpp>

ServerGameManager::ServerGameManager() {
	// Creates box collider for the world and inits quadtree 
	BoxCollider worldBox = BoxCollider(glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(200.0f, 200.0f, 200.0f));
	vector<BoxCollider*> objects;
	qt = new Quadtree(worldBox, 4, objects);

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
	for (Collider* c : allColliders) {
		glm::mat4 transform = glm::mat4(1);
		transform = glm::scale(transform, glm::vec3(c->length, c->width, c->height));
		transform = glm::translate(transform, c->center);
		
		ms.add(transform);
	}
	return ms;
}

void ServerGameManager::handleEvent(Event& e, int playerId) {
	//std::cerr << "size of allColliders " << allColliders.size() << std::endl;
	// TODO: colliders should stop at border 
	// Calculate where player wants to be
	players[playerId].update(e.pos, e.yaw, e.pitch);

	// Remove and readd player collider
	buildQuadtree();

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
			if (collider != otherCollider && collider->check_collision(otherCollider)) {
				// Actual collision --> don't update player position 
				glm::vec3 center = players[playerId].hitbox->center;
				BoxCollider* hitbox = players[playerId].hitbox;
				//std::cout << "center of player " << playerId << " at " << to_string(center) << std::endl;
				//std::cout << "Comparing collider at center " << to_string(collider->center)
					//<< " with lwh " << collider->length << " " << collider->width << " " << collider->height;
				//std::cout << " to collider at center " << to_string(otherCollider->center)
					//<< " with lwh " << otherCollider->length << " " << otherCollider->width << " " << otherCollider->height << std::endl;
				std::cout << "Collided" << std::endl;
				players[playerId].update(-e.pos, 0.0f, 0.0f);
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
	BoxCollider worldBox = BoxCollider(glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(420.0f, 200.0f, 200.0f));
	vector<BoxCollider*> objects;
	qt = new Quadtree(worldBox, 4, objects);
	// TODO: might run into issue if we constnatly remove things 
	// idea: hashtables 
	for (BoxCollider* bc : allColliders) {
		qt->insert(bc);
	}
}

GameState ServerGameManager::getGameState(int playerId) {
	return GameState(players[playerId].pos, players[playerId].front);
}
