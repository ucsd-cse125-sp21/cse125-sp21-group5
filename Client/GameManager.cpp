#include "GameManager.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "../Shared/Global_variables.h"

#include "Renderer.h"
#include "AudioManager.h"

// TODO: possibly move these as well
// Track mouse movements
bool firstMouse = true;
float GameManager::offsetX = 0.0f;
float GameManager::offsetY = 0.0f;
float GameManager::lastX = Window::width / 2;
float GameManager::lastY = Window::height / 2;
float GameManager::fov = 60.0f;

GameManager::GameManager(GLFWwindow* window)
{
	// Save pointer to window
	this->window = window;

	// Initialize transforms
	worldT = new Transform();
	worldT->setName("World Transform");
	// Preload models
	// TODO: maybe save this in a map for less variables
	playerModel = new Model("res/models/unitCube.dae");
	tileModel = new Model("res/models/tile.dae");
	treeModels.push_back(new Model("res/models/willowTrunk_old.dae"));
	treeModels.push_back(new Model("res/models/scragglyTrunk.dae"));
	treeModels.push_back(new Model("res/models/basicTree.dae"));
	treeModels.push_back(new Model("res/models/fiboTree.dae"));
	treeModels.push_back(new Model("res/models/deadTree.dae"));

	// Wall around the world
	wallTileModel = new Model("res/models/TrumpWall-20m.dae");
	wallTileModel->setName("Wall Tile Model");
	//Transform* wallT = new Transform(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f, TILE_SIZE/2, 0.0f));
	//wallT->add_child(wallTileModel);
	//worldT->add_child(wallT);

	playerModel->setName("Player Model");
	tileModel->setName("Tile Model");

	treeModels[0]->setName("res/models/willowTrunk_old.dae");
	treeModels[1]->setName("res/models/scragglyTrunk.dae");
	treeModels[2]->setName("res/models/basicTree.dae");
	treeModels[3]->setName("res/models/fiboTree.dae");
	treeModels[4]->setName("res/models/deadTree.dae");

	// Initialize to Empty transform to avoid race condition with MapState vs GameState update.
	// If Game State update reaches first, these need to be initialized or leads to nullptr exception.
	catT = new Transform();
	catT->setName("Uninitialized CatT");
	dogT = new Transform();
	dogT->setName("Uninitialized DogT");

	catModel = new Model("res/models/cat.dae");
	catModel->setName("res/models/cat.dae");
	catTeamIndicator = new Model("res/models/iconCat.dae");
	catTeamIndicator->setName("res/models/iconCat.dae");

	dogModel = new Model("res/models/finalHuskyIdle.dae");
	dogModel->setName("res/models/finalHuskyIdle.dae");
	dogTeamIndicator = new Model("res/models/iconDog.dae");
	dogTeamIndicator->setName("res/models/iconDog.dae");

	// Initialize variables
	showScoreboard = false;

	// Add a test point light
	//Renderer::get().addPointLight(PointLight(glm::vec3(0, 2, -2), glm::vec3(1, 0, 0)));
	//Renderer::get().addPointLight(PointLight(glm::vec3(0, 2, 2), glm::vec3(0, 1, 0)));

	//Renderer::get().addDirectionalLight(DirectionalLight(glm::vec3(1, 2, 0), glm::vec3(0.9)));

	//Renderer::get().addSpotLight(SpotLight(glm::vec3(0, 15, 0), glm::vec3(0, 0, 1), glm::vec3(1), 30));

	AudioManager::get().init();
	//AudioManager::get().playSound(SOUND_WOOF);

	//AudioManager::get().playSound(SOUND_DUB);

	// Initialize time variables
	deltaTime = 0.0f;
	prevTime = (float) glfwGetTime();
	currTime = (float) glfwGetTime();

	// Uninitialized playerID
	localPlayerId = -1;

	// GamecountDown
	gameCountdown = -1;	
} 

GameManager::~GameManager()
{
	// Delete all models
	delete worldT; // Recursively calls destructor for all nodes... hopefully
}

Event GameManager::update()
{
	// Hopefully doesn't break audio
	AudioManager::get().system->update();
	if (localPlayerId == -1) {
		// Client has not yet connected to the server.
		cout << "Local Player ID not received yet... Waiting to connect to Server..." << endl;
		// TODO: Display Loading Screen...
		return Event();
	}
	else {
		//cout << "Player pointer in unordered_map for localPlayerID " << localPlayerId << " is " << players[localPlayerId];
	}

	// Calculate deltaTime
	currTime = (float) glfwGetTime();
	deltaTime = currTime - prevTime;
	prevTime = currTime;

	Renderer::get().update(deltaTime);
	worldT->update(deltaTime);

	// Rendering of objects is done here. (Draw)
	render();

	// Listen for any events (keyboard input, mouse input, etc)
	glfwPollEvents();

	// Process keyboard input
	Event e = handleInput();

	// TODO: necessary?
	offsetX = 0.0f;
	offsetY = 0.0f;
	return e;
}

// Handle Keyboard and Mouse Input
Event GameManager::handleInput()
{
	// Get current mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Special case for first mouse
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	// Calculate offset from prev frame
	offsetX = (float)(xpos - lastX);
	offsetY = (float)(lastY - ypos);

	// Save previous positions
	lastX = (float)xpos;
	lastY = (float)ypos;

	// System Controls
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Get Player Camera
	Camera* camera = players[localPlayerId]->cam;

	// Jumping control
	bool jumping = false;

	// Player Controls
	glm::vec3 dPos = glm::vec3(0.0f);
	glm::vec3 dir(camera->front.x, 0.0f, camera->front.z);
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		dPos += glm::normalize(dir);
	}
	else if (glfwGetKey(window, GLFW_KEY_S))
	{
		//dPos -= camera->front;
		dPos -= glm::normalize(dir);
	}
	if (glfwGetKey(window, GLFW_KEY_A)) 
	{
		//dPos += -glm::normalize(glm::cross(camera->front, camera->up));
		dPos -= glm::normalize(glm::cross(dir, camera->up));
	}
	else if (glfwGetKey(window, GLFW_KEY_D))
	{
		//dPos += glm::normalize(glm::cross(camera->front, camera->up));
		dPos += glm::normalize(glm::cross(dir, camera->up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		if(players[localPlayerId]->isGrounded)
			jumping = 5;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
	{
		dPos -= camera->up;
	}

	// Class / Weapon 1
	if (glfwGetKey(window, GLFW_KEY_1))
	{
		if (gameCountdown != 0) {
			// Players can choose class only as they wait for other players to join.
			players[localPlayerId]->playerClass = 0;
		}
		else {
			players[localPlayerId]->gun_idx = 0;
		}
	}
	// Class / Weapon 2
	else if (glfwGetKey(window, GLFW_KEY_2))
	{
		if (gameCountdown != 0) {
			// Players can choose class only as they wait for other players to join.
			players[localPlayerId]->playerClass = 1;
		}
		else {
			players[localPlayerId]->gun_idx = 1;
		}
	}
	// Class / Weapon 3
	else if (glfwGetKey(window, GLFW_KEY_3))
	{
		// TODO: Uncomment when all rifle models are ready for use and part of the player's loaded models.
		if (gameCountdown != 0) {
			// Players can choose class only as they wait for other players to join.
			players[localPlayerId]->playerClass = 2;
		}
	}

	bool dab = false;
	// Special animation dab 
	if (glfwGetKey(window, GLFW_KEY_F)) {
		dab = true;
	}

	// Show scoreboard
	showScoreboard = glfwGetKey(window, GLFW_KEY_TAB);

	// Accumulate direction, and convert to offset
	if (dPos != glm::vec3(0.0f))
		dPos = glm::normalize(dPos);
	dPos *= camera->speed * deltaTime;

	// Update mouse movements
	float yaw = camera->sensitivity * offsetX;
	float pitch = camera->sensitivity * offsetY;

	// Plays step sound if dPos is 0 // VERY CURSED 
	//if (dPos != glm:: vec3(0.0f)) AudioManager::get().playSound(SOUND_STEP);

	// Detect mouse presses
	bool shooting = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);

	// If the player is dead, yeet
	if (players[localPlayerId]->isDead == DEATH_TICK_TIMER) {
		dPos = glm::vec3(0.0f, 15.0f, 0.0f);
		AudioManager::get().playSound(SOUND_DEATH);
	}

	return Event(dPos, yaw, pitch, shooting, jumping, players[localPlayerId]->playerClass, players[localPlayerId]->gun_idx, dab);
}

// Use for one-time key presses
void GameManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
	{
		Renderer::get().debug = !Renderer::get().debug;
	}

	if (key == GLFW_KEY_UP && action != GLFW_RELEASE)
	{
		AudioManager::get().adjustVolume(0.001f);
	}
	else if (key == GLFW_KEY_DOWN && action != GLFW_PRESS)
	{
		AudioManager::get().adjustVolume(-0.001f);
	}
}

// Detect mouse clicks
void GameManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
}

// Detect mouse position
void GameManager::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
}

// Detect mouse scroll
void GameManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	//cerr << "Fov: " << fov << endl;
	//fov -= yoffset;
	//fov = glm::clamp(fov, 1.0f, 270.0f);
	//Window::projection =
	//	glm::perspective(glm::radians(fov), (float) Window::width / (float) Window::height, 0.1f, 1000.0f);
}

// Draw objects to screen
void GameManager::render()
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render the models
	Renderer::get().setCamera(players[localPlayerId]->cam);
	worldT->draw(glm::mat4(1), Window::projection * players[localPlayerId]->cam->view);

	renderUI();

	// Swap buffers
	glfwSwapBuffers(window);
}

void GameManager::renderUI()
{
	// Make a new imgui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// show an example window
	ImGuiWindowFlags windowFlags = 0;

	windowFlags |= ImGuiWindowFlags_NoTitleBar;
	windowFlags |= ImGuiWindowFlags_NoScrollbar;
	windowFlags |= ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoCollapse;
	windowFlags |= ImGuiWindowFlags_NoNav;
	windowFlags |= ImGuiWindowFlags_NoBackground;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	bool showUI = true;

	if (gameCountdown < 0) {
		float yOffset = 200;
		ImGui::Begin("WaitingForPlayers", &showUI, windowFlags);
		ImVec2 texSize = ImGui::CalcTextSize("Waiting for additional players...");
		ImGui::SetWindowPos(ImVec2(Window::width / 2 - texSize.x / 2, yOffset));
		ImGui::SetWindowSize(ImVec2(texSize.x + 20, texSize.y + 20));
		//ImGui::SetWindowFontScale(2);
		ImGui::Text("Waiting for additional players...");
		//ImGui::SetWindowFontScale(1);
		ImGui::End();

		yOffset += texSize.y + 20;

		ImGui::Begin("PlayerSelectPrompt", &showUI, windowFlags);
		texSize = ImGui::CalcTextSize("Switch between player classes using number keys 1 and 2.");
		ImGui::SetWindowPos(ImVec2(Window::width / 2 - texSize.x / 2, yOffset));
		ImGui::SetWindowSize(ImVec2(texSize.x + 20, texSize.y + 20));
		//ImGui::SetWindowFontScale(2);
		ImGui::Text("Switch between player classes using number keys 1 and 2.");
		//ImGui::SetWindowFontScale(1);
		ImGui::End();

		yOffset += texSize.y + 20;

		float horizotalSpacing = 100.0f;

		ImGui::Begin("SelectedPlayerDetails", &showUI, windowFlags);
		ImGui::SetWindowPos(ImVec2(Window::width / 2 - 300.0f, yOffset));
		ImGui::SetWindowSize(ImVec2(600.0f, 150.0f));
		ImGui::Columns(2, "PlayerInfo");
		ImGui::SetColumnWidth(0, 300.0f);
		ImGui::SetColumnWidth(1, 300.0f);
		ImGui::Text("Selected Class:");
		ImGui::Text("Primary Weapon:");
		ImGui::Text("Secondary Weapon:");

		ImGui::NextColumn();
		switch(players[localPlayerId]->playerClass) {
		case 0:
			ImGui::Text("Player Class 1");
			ImGui::Text("Pistol");
			ImGui::Text("Flashbang Launcher");
			break;
		case 1:
			ImGui::Text("Player Class 2");
			ImGui::Text("Shotgun");
			ImGui::Text("Smoke Grenade Launcher");
			break;
		case 2:
			ImGui::Text("Player Class 3");
			ImGui::Text("Rifle");
			ImGui::Text("Stun Grenade Launcher");
			break;
		}
		ImGui::End();
	}

	if (gameCountdown > 0) {
		ImGui::Begin("StartingGame", &showUI, windowFlags);
		std::string str = (boost::format("Game starts in %i...") % (gameCountdown / 60)).str();
		ImVec2 texSize = ImGui::CalcTextSize(str.c_str());
		ImGui::SetWindowPos(ImVec2(Window::width / 2 - texSize.x / 2, 200));
		ImGui::SetWindowSize(ImVec2(texSize.x + 20, texSize.y + 20));
		//ImGui::SetWindowFontScale(2);
		ImGui::Text(str.c_str());
		//ImGui::SetWindowFontScale(1);
		ImGui::End();
	}

	// Let player know if they are frozen.
	if (players[localPlayerId]->isFrozen > 0)
	{
		ImGui::Begin("FrozenMessage", &showUI, windowFlags);
		std::string str = (boost::format("You've been frozen!!!")).str();
		ImVec2 texSize = ImGui::CalcTextSize(str.c_str());
		ImGui::SetWindowPos(ImVec2(Window::width / 2 - texSize.x / 2, 200));
		ImGui::SetWindowSize(ImVec2(texSize.x + 20, texSize.y + 20));
		//ImGui::SetWindowFontScale(2);
		ImGui::Text(str.c_str());
		//ImGui::SetWindowFontScale(1);
		ImGui::End();
	}

	// Let players know if they are Fogged.
	if (players[localPlayerId]->isFogged > 0)
	{
		ImGui::Begin("FoggedMessage", &showUI, windowFlags);
		std::string str = (boost::format("You think it's dark, but they can see you....")).str();
		ImVec2 texSize = ImGui::CalcTextSize(str.c_str());
		ImGui::SetWindowPos(ImVec2(Window::width / 2 - texSize.x / 2, 200));
		ImGui::SetWindowSize(ImVec2(texSize.x + 20, texSize.y + 20));
		//ImGui::SetWindowFontScale(2);
		ImGui::Text(str.c_str());
		//ImGui::SetWindowFontScale(1);
		ImGui::End();
	}

	// Let players know that they have limited visibilty.
	if (players[localPlayerId]->hasLimitedFOV > 0)
	{
		ImGui::Begin("LimitedFOVMessage", &showUI, windowFlags);
		std::string str = (boost::format("Can you see what's coming?")).str();
		ImVec2 texSize = ImGui::CalcTextSize(str.c_str());
		ImGui::SetWindowPos(ImVec2(Window::width / 2 - texSize.x / 2, 200));
		ImGui::SetWindowSize(ImVec2(texSize.x + 20, texSize.y + 20));
		//ImGui::SetWindowFontScale(2);
		ImGui::Text(str.c_str());
		//ImGui::SetWindowFontScale(1);
		ImGui::End();
	}

	// Health bar for local player's HUD.
	ImGui::Begin("Health UI", &showUI, windowFlags);
	ImGui::SetWindowPos(ImVec2(50, Window::height - 150));
	ImGui::SetWindowSize(ImVec2(300, 100));
	ImGui::Text("Super basic health bar");
	// Change Health Bar color.
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_PlotHistogram, ImVec4(0, 255, 0, 1));
	ImGui::ProgressBar(players[localPlayerId]->health / 100.0f);
	ImGui::PopStyleColor();
	ImGui::End();

	// super basic crosshair, maybe move this somewhere else
	ImGui::Begin("Crosshairs", &showUI, windowFlags);
	ImGui::SetWindowPos(ImVec2(Window::width / 2 - 20, Window::height / 2 - 20));
	ImGui::SetWindowSize(ImVec2(200, 200));

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 p = ImGui::GetCursorScreenPos();
	drawList->AddLine(ImVec2(p.x + 20, p.y), ImVec2(p.x + 20, p.y + 40), ImColor(ImVec4(0, 1, 0, 1)), 1.0);
	drawList->AddLine(ImVec2(p.x, p.y + 20), ImVec2(p.x + 40, p.y + 20), ImColor(ImVec4(0, 1, 0, 1)), 1.0);
	ImGui::End();

	// Gun info
	ImGui::Begin("Gun Info", &showUI, windowFlags);
	string str = players[localPlayerId]->curr_gun.name;
	str.append((boost::format("\n%i") % (players[localPlayerId]->curr_gun.clip_size)).str());
	ImVec2 texSize = ImGui::CalcTextSize(str.c_str());
	ImGui::SetWindowPos(ImVec2(Window::width - 400, Window::height - 200));
	ImGui::SetWindowSize(ImVec2(400, 300));
	//ImGui::SetWindowFontScale(2);
	ImGui::Text(players[localPlayerId]->curr_gun.name.c_str());
	//ImGui::SetWindowFontScale(1);
	if (players[localPlayerId]->curr_gun.reload_time > 0) {
		//ImGui::TextDisabled("%i", players[localPlayerId]->curr_gun.clip_size);
		ImGui::TextDisabled("Reloading...");
	}
	else {
		ImGui::TextColored(ImVec4(255, 0, 0, 1), "%i", players[localPlayerId]->curr_gun.clip_size);
	}
	ImGui::End();

	// Debug UI Information
	if (Renderer::get().debug)
	{
		ImGui::Begin("Debug UI", &showUI, windowFlags);
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(1000, 500));
		Player* p = players[localPlayerId];
		ImGui::Text("Number of Players: %d", players.size());
		ImGui::Text("Player ID: %d", localPlayerId);
		ImGui::Text("Player center position: (%.2f, %.2f, %.2f)", p->cam->pos.x, p->cam->pos.y, p->cam->pos.z);
		ImGui::Text("Player gun name: %s", p->curr_gun.name);
		ImGui::Text("Player isDead: %d", p->isDead);
		ImGui::Text("Player isGrounded: %d", p->isGrounded);
		ImGui::Text("Player isCarryingFlag: %d", p->isCarryingCatFlag || p->isCarryingDogFlag);
		ImGui::Text("Game Volume: %f", AudioManager::get().volume);
		ImGui::Text("Player is shooting %d", p->isShooting);
		ImGui::End();
	}

	// Scoreboard UI
	if (showScoreboard)
	{
		// Show first team (left)
		// TODO: Deal with tabbing
		windowFlags &= ~ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.25));

		ImGui::Begin("Scoreboard", &showUI, windowFlags);
		ImGui::SetWindowPos(ImVec2(Window::width / 2 - 500, Window::height / 2 - 250));
		ImGui::SetWindowSize(ImVec2(1000, 1000));
		ImGui::Columns(2, "Scoreboard1");
		ImGui::SetColumnWidth(0, 500.0f);
		ImGui::SetColumnWidth(1, 500.0f);
		// START OF CAT TEAM UI 
		ImGui::Text("CAT TEAM");
		ImGui::BeginChild("inner1");
			ImGui::Columns(4, "Stats");
			ImGui::SetColumnWidth(0, 125.0f);
			ImGui::SetColumnWidth(1, 125.0f);
			ImGui::SetColumnWidth(2, 125.0f);
			ImGui::SetColumnWidth(3, 125.0f);
			ImGui::Text("PlayerID");
			for (auto& p : players)
			{
				if (p.first % 2 == (int)PlayerTeam::DOG_LOVER) continue;
				ImGui::Text("%d", p.first);
			}
			
			ImGui::NextColumn();
			ImGui::Text("Kills");
			for (auto& p : players)
			{
				if (p.first % 2 == (int)PlayerTeam::DOG_LOVER) continue;
				ImGui::Text("%d", p.second->kills);
			}
			
			ImGui::NextColumn();
			ImGui::Text("Deaths");
			for (auto& p : players)
			{
				if (p.first % 2 == (int)PlayerTeam::DOG_LOVER) continue;
				ImGui::Text("%d", p.second->deaths);
			}
		
			ImGui::NextColumn();
			ImGui::Text("Captures");
			for (auto& p : players)
			{
				if (p.first % 2 == (int)PlayerTeam::DOG_LOVER) continue;
				ImGui::Text("%d", p.second->captures);
			}
			ImGui::EndChild();
			ImGui::NextColumn();

		// START OF DOG TEAM UI
		ImGui::BeginChild("inner2");
		ImGui::Text("DOG TEAM");
			ImGui::Columns(4, "Stats");
			ImGui::SetColumnWidth(0, 125.0f);
			ImGui::SetColumnWidth(1, 125.0f);
			ImGui::SetColumnWidth(2, 125.0f);
			ImGui::SetColumnWidth(3, 125.0f);
			ImGui::Text("PlayerID");
			for (auto& p : players)
			{
				if (p.first % 2 == (int)PlayerTeam::CAT_LOVER) continue;
				ImGui::Text("%d", p.first);
			}

			ImGui::NextColumn();
			ImGui::Text("Kills");
			for (auto& p : players)
			{
				if (p.first % 2 == (int)PlayerTeam::CAT_LOVER) continue;
				ImGui::Text("%d", p.second->kills);
			}

			ImGui::NextColumn();
			ImGui::Text("Deaths");
			for (auto& p : players)
			{
				if (p.first % 2 == (int)PlayerTeam::CAT_LOVER) continue;
				ImGui::Text("%d", p.second->deaths);
			}

			ImGui::NextColumn();
			ImGui::Text("Captures");
			for (auto& p : players)
			{
				if (p.first % 2 == (int)PlayerTeam::CAT_LOVER) continue;
				ImGui::Text("%d", p.second->captures);
			}
			ImGui::EndChild();
		ImGui::End();

		windowFlags |= ImGuiWindowFlags_NoBackground;
		ImGui::PopStyleColor();
	}

	// UI for game end 
	if (catTeamWin) {
		ImGui::Begin("GAME OVER", &showUI, windowFlags);
		ImGui::SetWindowPos(ImVec2(Window::width / 2, Window::height / 2 - 250));
		ImGui::SetWindowSize(ImVec2(500, 500));
		ImGui::Text("Cat Team Won");
		ImGui::End();
	}
	else if (dogTeamWin) {
		ImGui::Begin("GAME OVER", &showUI, windowFlags);
		ImGui::SetWindowPos(ImVec2(Window::width / 2, Window::height / 2 - 250));
		ImGui::SetWindowSize(ImVec2(500, 500));
		ImGui::Text("Dog Team Won");
		ImGui::End();
	}

	// call ImGUI render to actually render the ui to opengl
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// We translate then scale on the server, so we have to match on client
void GameManager::updateMap(MapState& ms)
{
	srand(ms.tileSeed);

	for (int i = 0; i < NUM_MAP_TILES; i++)
	{
		for (int j = 0; j < NUM_MAP_TILES; j++)
		{
			// Define center of tile
			glm::vec3 tileCenter = glm::vec3(TILE_SIZE * (i - NUM_MAP_TILES / 2), 0, TILE_SIZE * (j - NUM_MAP_TILES / 2));

			// Create tile transform
			Transform* tileT = new Transform(glm::vec3(1.0f), glm::vec3(0.0f), tileCenter);
			tileT->setName("Tile ["  + std::to_string(i) + "][" + std::to_string(j) + "] Transform");
			tileT->add_child(tileModel);
			worldT->add_child(tileT);

			// Add Wall Tiles
			if (i == 0)
			{
				Transform* wallT = new Transform(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f));
				wallT->translate(tileCenter - glm::vec3(((float)TILE_SIZE) / 2, 0.0f, 0.0f));
				wallT->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				wallT->setName("Wall [" + std::to_string(i) + "][" + std::to_string(j) + "] Transform");
				wallT->add_child(wallTileModel);
				worldT->add_child(wallT);

				//cout << "Creating wall [" << i << "][" << j << "] at \t" << glm::to_string(wallT->translation) << endl;
			}

			if (i == NUM_MAP_TILES - 1)
			{
				Transform* wallT = new Transform(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f));
				wallT->translate(tileCenter + glm::vec3(((float)TILE_SIZE) / 2, 0.0f, 0.0f));
				wallT->rotate(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				wallT->setName("Wall [" + std::to_string(i) + "][" + std::to_string(j) + "] Transform");
				wallT->add_child(wallTileModel);
				worldT->add_child(wallT);

				//cout << "Creating wall [" << i << "][" << j << "] at \t" << glm::to_string(wallT->translation) << endl;

			}

			if (j == 0)
			{
				Transform* wallT = new Transform(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f));
				wallT->translate(tileCenter - glm::vec3(0.0f, 0.0f, ((float)TILE_SIZE) / 2));
				wallT->rotate(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				wallT->setName("Wall [" + std::to_string(i) + "][" + std::to_string(j) + "] Transform");
				wallT->add_child(wallTileModel);
				worldT->add_child(wallT);

				//cout << "Creating wall [" << i << "][" << j << "] at \t" << glm::to_string(wallT->translation) << endl;

			}

			if (j == NUM_MAP_TILES - 1)
			{
				Transform* wallT = new Transform(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f));
				wallT->translate(tileCenter + glm::vec3(0.0f, 0.0f, ((float)TILE_SIZE) / 2));
				wallT->rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				wallT->setName("Wall [" + std::to_string(i) + "][" + std::to_string(j) + "] Transform");
				wallT->add_child(wallTileModel);
				worldT->add_child(wallT);

				//cout << "Creating wall [" << i << "][" << j << "] at \t" << glm::to_string(wallT->translation) << endl;

			}

			PointLight p = PointLight(tileCenter + glm::vec3(0, 1, 0),glm::vec3(rand()/(float)RAND_MAX, rand()/(float)RAND_MAX, rand()/(float)RAND_MAX));
			Renderer::get().addPointLight(p);


			// Cat flag
			if (i == 0 && j == 0)
			{
				catT = new Transform(glm::vec3(1.0f), glm::vec3(0.0f), tileCenter + glm::vec3(0.0f, 0.75f, 0.0f));
				catT->setName("catT");
				catT->add_child(catModel);
				worldT->add_child(catT);
				continue;
			}
			// Dog flag
			else if (i == NUM_MAP_TILES - 1 && j == NUM_MAP_TILES - 1)
			{
				dogT = new Transform(glm::vec3(1.0f), glm::vec3(0.0f), tileCenter + glm::vec3(0.0f, 0.75f, 0.0f));
				dogT->setName("dogT");
				dogT->add_child(dogModel);
				worldT->add_child(dogT);
				continue;
			}

			// Generate Trees
			int numTrees = rand() % MAX_NUM_TREES_PER_TILE;
			for (int k = 0; k < numTrees; k++) {
				// Pick random tree position
				float x = TILE_SIZE * (rand() / (float)RAND_MAX) - 10.0f;
				float z = TILE_SIZE * (rand() / (float)RAND_MAX) - 10.0f;
			    
				// Generate the position inside the tile
				Transform* treeT = new Transform(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(x, 0.0f, z));
				treeT->rotate(360.0f * (rand() / (float)RAND_MAX), glm::vec3(0.0f, 1.0f, 0.0f));
				treeT->add_child(treeModels[rand() % treeModels.size()]);
				treeT->setName("Tree [" + std::to_string(i) + "][" + std::to_string(j) + "] Transform");
				tileT->add_child(treeT);
			}
		}
	}
}

void GameManager::updateGameState(GameState& gs)
{
	for (const PlayerState& ps : gs.states)
	{
		// TODO: Decide what to do if player doesn't exist but update is received.
		// Ignore update if player doesn't exist
		if (players.find(ps.playerId) == players.end())
			continue;

		players[ps.playerId]->updatePlayer(ps);
		
		// Play shooting for player
		// TODO: change it so that it's at the 3d location
		if (players[ps.playerId]->isShooting)
			AudioManager::get().playSound(SOUND_SHOOT);

	}
	catTeamWin = gs.catTeamWin;
	dogTeamWin = gs.dogTeamWin;
	catT->setTranslate(gs.catLocation);
	dogT->setTranslate(gs.dogLocation);

	gameCountdown = gs.gameCountdown;
}

// TODO: Model* should be a string or int to what kind of model should be used to render player
void GameManager::addPlayer(int playerId, Model* playerModel)
{
	// Check if player already exists
	if (players.find(playerId) != players.end())
	{
		cerr << "Player with ID " << playerId << " already exists!" << endl;
		return;
	}

	// Create new player with model
	Transform* playerT = new Transform();
	playerT->setName("Player " + std::to_string(playerId) + " Transform");

	Transform* teamIndicatorT = new Transform();
	teamIndicatorT->setName("Player " + std::to_string(playerId) + " Team Indicator Transform");

	Player* player = new Player(playerT, playerId, teamIndicatorT);
	player->setName("Player " + std::to_string(playerId));
	
	if (playerId % 2 == (int)PlayerTeam::DOG_LOVER) {
		//Player is part of dog Team
		teamIndicatorT->add_child(dogTeamIndicator);
	}
	else {
		// Player is part of cat Team
		teamIndicatorT->add_child(catTeamIndicator);
	}

	playerT->add_child(player);
	worldT->add_child(playerT);

	// Transform not added to world. Player calls draw on this transform.
	//worldT->add_child(teamIndicatorT);

	//players.insert(make_pair(playerId, player));
	players[playerId] = player;
}

void GameManager::setLocalPlayerID(int playerId)
{
	std::cout << "Local player id is " << playerId << std::endl;
	this->localPlayerId = playerId;
	Renderer::get().localPlayerId = playerId;
}