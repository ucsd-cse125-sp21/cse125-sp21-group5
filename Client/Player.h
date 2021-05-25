#pragma once

#include <string>
#include "Camera.h"
#include "Model.h"
#include "Transform.h"
#include "Renderer.h"
#include "../Shared/GameState.h"
#include "../Shared/Gun.h"

// Import IMGui for Player HUD UI.
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Player : public Node
{
private:
	bool mustLoadModels;

public:
	Camera* cam;
	Transform* transform;

	Model* model;
	std::vector<Model*> models;

	int playerId;

	float health;
	int isDead;
	
	bool isGrounded;
	bool isCarryingCatFlag;
	bool isCarryingDogFlag;

	unsigned int kills, deaths, captures;

	int gun_idx;
	vector<Gun*> guns;

	Player(Transform* transform, int playerId);
	~Player();

	void draw(const glm::mat4& parent_transform, const glm::mat4& view);
	void update(float deltaTime);

	void loadModels();
	void updatePlayer(PlayerState ps);

	void displayHUD();
};