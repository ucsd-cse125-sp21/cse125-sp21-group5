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
#include "AudioManager.h"

class Player : public Node
{
private:
	bool mustLoadModels;

public:
	Camera* cam;
	Transform* transform;

	Transform* teamIndicatorTransform;

	Model* model;
	std::vector<Model*> modelsPistol; //pistol
	std::vector<Model*> modelsShotgun; //shotgun
	std::vector<Model*> modelsRifle; //rifle

	// SHEILA 
	// Index 0 - pistol
	// Index 1 - shotgun 
	// Index 2 - rifle 
	std::vector<vector<Model*>> gunTypeModels;

	int playerId;

	float health;
	int isDead;

	int hasLimitedFOV, isFogged, isFrozen;
	
	bool isShooting;
	bool isGrounded;
	bool isCarryingCatFlag;
	bool isCarryingDogFlag;

	unsigned int kills, deaths, captures;

	int gun_idx;
	Gun curr_gun;

	int playerClass;

	Player(Transform* transform, int playerId, Transform* teamIndicatorTransform);
	~Player();

	void draw(const glm::mat4& parent_transform, const glm::mat4& view);
	void update(float deltaTime);

	void loadModels();
	void updatePlayer(PlayerState ps);

	void displayHUD();
};