#include "Player.h"

// TODO: Used for yourself?
/*
Player::Player(Transform* transform)
{
	this->transform = transform;
	this->model = nullptr;
	cam = new Camera();
}
*/

// Used for other players
Player::Player(Transform* transform, int playerId)
{
	this->playerId = playerId;
	this->transform = transform;
	cam = new Camera();
	mustLoadModels = true;
	this->model = NULL;
	this->health = -1.0f;
	this->isDead = 1;
	this->isCarryingCatFlag = false;
	this->isCarryingDogFlag = false;
}

Player::~Player() {
	for (Model* m : models) {
		delete m;
	}
}

void Player::draw(const glm::mat4& parent_transform, const glm::mat4& view)
{
	// don't draw null model anything if model is null
	if (mustLoadModels) return;
	if (playerId == Renderer::get().localPlayerId) return;
	model->draw(glm::rotate(parent_transform, -glm::atan(cam->front.z, cam->front.x) - glm::pi<float>() / 2, glm::vec3(0, 1, 0)), view);
}

void Player::update(float deltaTime)
{
	if (mustLoadModels) {
		loadModels();
		mustLoadModels = false;
	}

	model->update(deltaTime);
}

void Player::updatePlayer(PlayerState ps)
{
	// TODO: players should be able to pick their own name
	// Update Rendering information
	cam->update(ps.pos + glm::vec3(0.0f, 0.25f, 0.0f), ps.front);
	transform->setTranslate(ps.pos);

	// Update Movement information
	isGrounded = ps.isGrounded;
	isDead = ps.isDead;
	isCarryingCatFlag = ps.carryingCatFlag;
	isCarryingDogFlag = ps.carryingDogFlag;

	// Update Score information
	kills = ps.kills;
	deaths = ps.deaths;
	captures = ps.captures;

	// Health update
	health = ps.health;

	// Don't waste time animating
	if (mustLoadModels) return;

	model = models[(int)ps.currentAnimation];
}


// loads the model files and sets up shaders
void Player::loadModels() {

	models.push_back(new Model("res/models/finalIdle.dae"));
	models.push_back(new Model("res/models/finalWalk.dae"));
	models.push_back(new Model("res/models/finalShoot.dae"));
	models.push_back(new Model("res/models/finalToxicDab.dae"));
	models.push_back(new Model("res/models/finalDeath200.dae"));

	model = models[0];
}

void Player::displayHUD()
{
	// TODO: Fill in IMGUI stuff.
	// show an example window
	/*ImGuiWindowFlags windowFlags = 0;

	windowFlags |= ImGuiWindowFlags_NoTitleBar;
	windowFlags |= ImGuiWindowFlags_NoScrollbar;
	windowFlags |= ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoCollapse;
	windowFlags |= ImGuiWindowFlags_NoNav;
	windowFlags |= ImGuiWindowFlags_NoBackground;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	bool showUI = true;
	ImGui::Begin("Health UI", &showUI, windowFlags);
	ImGui::SetWindowPos(ImVec2(50, Window::height - 150));
	ImGui::SetWindowSize(ImVec2(300, 100));*/
}