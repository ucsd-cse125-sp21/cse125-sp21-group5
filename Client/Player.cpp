#include "Player.h"


// Used for other players
Player::Player(Transform* transform, int playerId, Transform* teamIndicatorTransform, std::string team)
{
	this->playerId = playerId;
	this->transform = transform;
	this->teamIndicatorTransform = teamIndicatorTransform;

	cam = new Camera();
	mustLoadModels = true;
	this->model = nullptr;
	this->health = -1.0f;
	this->isDead = 1;
	this->isGrounded = false;
	this->isCarryingCatFlag = false;
	this->isCarryingDogFlag = false;
	this->isDisrespecting = false;
	this->kills = 0;
	this->deaths = 0;
	this->captures = 0;
	this->playerClass = 0;
	this->gun_idx = 1;
	this->curr_gun = Gun();
	
	this->team = team;

	// Try something
	//loadModels();
	//this->model = gunTypeModels[playerClass][(int)AnimationID::IDLE];
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
	
	// Need to call draw here since transform is not added to the world.
	teamIndicatorTransform->draw(glm::rotate(parent_transform, -glm::atan(cam->front.z, cam->front.x) - glm::pi<float>() / 2, glm::vec3(0, 1, 0)), view);
}

void Player::update(float deltaTime)
{
	//cout << "\tCurrently updating " << name << endl;
	if (mustLoadModels) {
		cout << "\tLoading Player Models...." << endl;
		loadModels();
		cout << "\tModels Loaded!!" << endl;
		mustLoadModels = false;
	}

	model->update(deltaTime);
}

void Player::updatePlayer(PlayerState ps)
{
	// Update Rendering information
	cam->update(ps.pos + glm::vec3(0.0f, 0.25f, 0.0f), ps.front);
	transform->setTranslate(ps.pos);

	teamIndicatorTransform->setRotate(transform->rotation);
	teamIndicatorTransform->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	teamIndicatorTransform->setTranslate(glm::vec3(0.0f, 0.5f, 0.0f));

	// Update Movement information
	isShooting = ps.isShooting;
	isGrounded = ps.isGrounded;
	isDead = ps.isDead;
	isCarryingCatFlag = ps.carryingCatFlag;
	isCarryingDogFlag = ps.carryingDogFlag;

	// Update Special Attack Effect information
	hasLimitedFOV = ps.isLimitFOV;
	isFogged = ps.isFogged;
	isFrozen = ps.isFrozen;

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
	model = gunTypeModels[(int)ps.playerClass][(int)ps.currentAnimation];
	if (ps.currentAnimation != previousAnimation && ps.currentAnimation == AnimationID::DEATH) {
		model->getAnimationPlayer()->mStartTime = Renderer::get().gameTime - (1.0f / 60.0f);
	}

	previousAnimation = ps.currentAnimation;
}


// loads the model files and sets up shaders
void Player::loadModels() {

	modelsPistol.push_back(new Model("res/models/finalIdle.dae", team));
	modelsPistol.push_back(new Model("res/models/finalWalk.dae", team));
	modelsPistol.push_back(new Model("res/models/finalShoot.dae", team));
	modelsPistol.push_back(new Model("res/models/finalToxicDab.dae", team));
	modelsPistol.push_back(new Model("res/models/finalDeath200.dae", team));
	modelsPistol.push_back(new Model("res/models/finalReload.dae", team));

	modelsPistol[0]->setName("res/models/finalIdle.dae");
	modelsPistol[1]->setName("res/models/finalWalk.dae");
	modelsPistol[2]->setName("res/models/finalShoot.dae");
	modelsPistol[3]->setName("res/models/finalToxicDab.dae");
	modelsPistol[4]->setName("res/models/finalDeath200.dae");
	modelsPistol[5]->setName("res/models/finalReload.dae");

	modelsShotgun.push_back(new Model("res/models/Shotgun-Idle.dae", team));
	modelsShotgun.push_back(new Model("res/models/Shotgun-Walk.dae", team));
	modelsShotgun.push_back(new Model("res/models/Shotgun-Shoot.dae", team));
	modelsShotgun.push_back(new Model("res/models/Shotgun-OrangeJustice.dae", team));
	modelsShotgun.push_back(new Model("res/models/Shotgun-Death.dae", team));
	modelsShotgun.push_back(new Model("res/models/Shotgun-Reload.dae", team));

	modelsShotgun[0]->setName("res/models/Shotgun-Idle.dae");
	modelsShotgun[1]->setName("res/models/Shotgun-Walk.dae");
	modelsShotgun[2]->setName("res/models/Shotgun-Shoot.dae");
	modelsShotgun[3]->setName("res/models/Shotgun-OrangeJustice.dae");
	modelsShotgun[4]->setName("res/models/Shotgun-Death.dae");
	modelsShotgun[5]->setName("res/models/Shotgun-Reload.dae");

	modelsRifle.push_back(new Model("res/models/Rifle-Idle.dae", team));
	modelsRifle.push_back(new Model("res/models/Rifle-Walk.dae", team));
	modelsRifle.push_back(new Model("res/models/Rifle-Shoot.dae", team));
	modelsRifle.push_back(new Model("res/models/Rifle-Floss.dae", team));
	modelsRifle.push_back(new Model("res/models/Rifle-Death.dae", team));
	modelsRifle.push_back(new Model("res/models/Rifle-Reload.dae", team));

	modelsRifle[0]->setName("res/models/Rifle-Idle.dae");
	modelsRifle[1]->setName("res/models/Rifle-Walk.dae");
	modelsRifle[2]->setName("res/models/Rifle-Shoot.dae");
	modelsRifle[3]->setName("res/models/Rifle-Floss.dae");
	modelsRifle[4]->setName("res/models/Rifle-Death.dae");
	modelsRifle[5]->setName("res/models/Rifle-Reload.dae");

	gunTypeModels.push_back(modelsPistol); //push back pistol6
	gunTypeModels.push_back(modelsShotgun); //push back shotgun
	gunTypeModels.push_back(modelsRifle); //push back rifle

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