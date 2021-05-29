#include "Player.h"

// Used for other players
Player::Player(Transform* transform, int playerId)
{
	this->playerId = playerId;
	this->transform = transform;
	cam = new Camera();
	mustLoadModels = true;
	this->model = nullptr;
	this->health = -1.0f;
	this->isDead = 1;
	this->isGrounded = false;
	this->isCarryingCatFlag = false;
	this->isCarryingDogFlag = false;
	this->kills = 0;
	this->deaths = 0;
	this->captures = 0;
	this->playerClass = 0;
	this->gun_idx = 1;
	this->curr_gun = Gun();
}

Player::~Player()
{
	for (Model* m : modelsPistol)
	{
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

	// Update whichever gun you are holding
	gun_idx = ps.gun_idx;
	curr_gun = ps.curr_gun;

	// Don't waste time animating
	if (mustLoadModels) return;

	//model = modelsPistol[(int)ps.currentAnimation];
	model = gunTypeModels[gun_idx][(int)ps.currentAnimation];
}


// loads the model files and sets up shaders
void Player::loadModels() {

	modelsPistol.push_back(new Model("res/models/finalIdle.dae"));
	modelsPistol.push_back(new Model("res/models/finalWalk.dae"));
	modelsPistol.push_back(new Model("res/models/finalShoot.dae"));
	modelsPistol.push_back(new Model("res/models/finalToxicDab.dae"));
	modelsPistol.push_back(new Model("res/models/finalDeath200.dae"));
	//modelsPistol.push_back(new Model("res/models/finalReload.dae"));

	modelsShotgun.push_back(new Model("res/models/Shotgun-Idle.dae"));
	modelsShotgun.push_back(new Model("res/models/Shotgun-Walk.dae"));
	modelsShotgun.push_back(new Model("res/models/Shotgun-Shoot.dae"));
	modelsShotgun.push_back(new Model("res/models/Shotgun-OrangeJustice.dae"));
	modelsShotgun.push_back(new Model("res/models/Shotgun-Death.dae"));
	modelsShotgun.push_back(new Model("res/models/Shotgun-Reload.dae"));

	gunTypeModels.push_back(modelsPistol); //push back pistol
	gunTypeModels.push_back(modelsShotgun); //push back shotgun
	//gunTypeModels.push_back(modelsRifle); //push back rifle

	model = gunTypeModels[0][0]; //get the pistol, get first animation which is IDLE

	//example of how to access the current model, we default at pistol when we start
	//model = gunTypeModels.at(gun_idx).at(IDLE);
	//model = gunTypeModels.at(gun_idx).at(0);
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