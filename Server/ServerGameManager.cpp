#include "ServerGameManager.h"
#include <glm/gtx/string_cast.hpp>

ServerGameManager::ServerGameManager() {
	// Creates box collider for the world and inits quadtree 
	Collider worldBox = Collider(glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(200.0f, 200.0f, 200.0f));
	vector<Collider*> objects;
	//qt = new Quadtree(worldBox, 8, objects);

	// TODO: remove, hardcoded initPos
	players.push_back(Player(glm::vec3(0.0f, 15.0f, 0.0f)));
	players.push_back(Player(glm::vec3(0.0f, -15.0f, 0.0f)));
	//players.push_back(Player(glm::vec3(5.0f, 0.0f, 0.0f)));
	//players.push_back(Player(glm::vec3(5.0f, 0.0f, 0.0f)));
	for (Player p : players) {
		allColliders.push_back(p.hitbox);
	}
	//buildQuadtree();
}

MapState ServerGameManager::generateMap() {

	// Add one new box object into world
	allColliders.push_back(new Collider(glm::vec3(0.0f), glm::vec3(1.0f)));

	// Add all colliders to MapState
	MapState ms;

	Collider* c = allColliders[0];
	glm::mat4 transform1 = glm::mat4(1);
	transform1 = glm::scale(transform1, c->dim);
	transform1 = glm::translate(transform1, c->center);

	c = allColliders[1];
	glm::mat4 transform2 = glm::mat4(1);
	transform2 = glm::scale(transform2, c->dim);
	transform2 = glm::translate(transform2, c->center);

	c = allColliders[2];
	glm::mat4 transform3 = glm::mat4(1);
	transform3 = glm::scale(transform3, c->dim);
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

	/*
	for (Collider* c : allColliders) {
		glm::mat4 transform = glm::mat4(1);
		transform = glm::scale(transform, glm::vec3(c->length, c->width, c->height));
		transform = glm::translate(transform, c->center);
		
		ms.add(transform);
	}
	*/
	return ms;
}

void ServerGameManager::handleEvent(Event& e, int playerId) {
	//std::cerr << "size of allColliders " << allColliders.size() << std::endl;
	// TODO: colliders should stop at border 
	// Calculate where player wants to be
	players[playerId].update(e.pos, e.yaw, e.pitch);
	cerr << glm::to_string(players[playerId].pos) << endl;

	// Remove and readd player collider
	//buildQuadtree();

	// Naive collision (for now)
	for (int i = 0; i < allColliders.size(); i++) {
		Collider* collider = allColliders[i];

		// Actual collision 
		for (int j = i + 1; j < allColliders.size(); j++) {
			Collider* otherCollider = allColliders[j];
			// Collision happens
			if (collider != otherCollider && collider->check_collision(otherCollider)) {
				// Actual collision --> don't update player position 
				glm::vec3 center = players[playerId].hitbox->center;
				Collider* hitbox = players[playerId].hitbox;
				std::cout << "Collided" << std::endl;
				players[playerId].update(-e.pos, 0.0f, 0.0f);
				// Collided with first thing 
				break;
			}
		}
		break;
	}
}

// Function that builds/rebuilds quadtree 
void ServerGameManager::buildQuadtree() {
	Collider worldBox = Collider(glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(420.0f, 200.0f, 200.0f));
	vector<Collider*> objects;
	//qt = new Quadtree(worldBox, 4, objects);
	// TODO: might run into issue if we constnatly remove things 
	// idea: hashtables 
	for (Collider* bc : allColliders) {
		//qt->insert(bc);
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
