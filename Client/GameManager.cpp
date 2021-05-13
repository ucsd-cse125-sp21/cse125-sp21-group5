#include "GameManager.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"



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

	// Create camera
	this->camera = new Camera();

	// Initialize transforms
	worldT = new Transform();
	playerT = new Transform(glm::vec3(0.5f), glm::vec3(0, 0, 0), glm::vec3(0.0f, 0.0f, 0.0));
	playerT = new Transform(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(15.0f, 0.0f, 0.0));

	Model* cube = new Model("res/models/toxicGunDab.dae");

	cubeT1 = new Transform();
	cubeT2 = new Transform();
	cubeT3 = new Transform();

	cubeT1->add_child(cube);
	//cubeT2->add_child(cube);
	//cubeT3->add_child(cube);

	worldT->add_child(cubeT1);
	worldT->add_child(cubeT2);
	worldT->add_child(cubeT3);
	
	// Build scene graph
	//worldT->add_child(playerT);
	//playerT->add_child(playerM);
	//cubeT->add_child(cube);
	//worldT->add_child(cubeT);

	// Add a test point light
	Renderer::get().addPointLight(PointLight(glm::vec3(0, 2, -2), glm::vec3(1, 0, 0)));
	Renderer::get().addPointLight(PointLight(glm::vec3(0, 2, 2), glm::vec3(0, 1, 0)));

	Renderer::get().addDirectionalLight(DirectionalLight(glm::vec3(1, 2, 0), glm::vec3(2)));

	// Initialize time variables
	deltaTime = 0.0f;
	prevTime = (float) glfwGetTime();
	currTime = (float) glfwGetTime();

} 

GameManager::~GameManager()
{
	// Delete all models
	delete worldT; // Recursively calls destructor for all nodes... hopefully
}

Event GameManager::update()
{
	// Make a new imgui frame
	// do this here so game objects can make ImGUI calls in their update function
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

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

	//playerT->translate(glm::vec3(-0.001f, 0.0f, 0.0f));

	// Update camera position
	// TODO: place camera inside of Player class
	offsetX = 0.0f;
	offsetY = 0.0f;
	return e;
}

// Handle Keyboard Input
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

	// Player Controls
	glm::vec3 toSend = glm::vec3(0);
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		toSend += camera->front;
	}
	else if (glfwGetKey(window, GLFW_KEY_S))
	{
		toSend -= camera->front;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) 
	{
		toSend += -glm::normalize(glm::cross(camera->front, camera->up));
	}
	else if (glfwGetKey(window, GLFW_KEY_D))
	{
		toSend += glm::normalize(glm::cross(camera->front, camera->up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		toSend += camera->up;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
	{
		toSend += -camera->up;
	}

	toSend *= camera->speed * deltaTime;

	// Update mouse movements
	float yaw = camera->sensitivity * offsetX;
	float pitch = camera->sensitivity * offsetY;

	return Event(toSend, yaw, pitch);
}

// Use for one-time key presses
void GameManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

// Detect mouse clicks
void GameManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (button)
	{
		case GLFW_MOUSE_BUTTON_LEFT:
			switch (action)
			{
			case GLFW_PRESS:
				fprintf(stderr, "Left Mouse Pressed\n");
				break;
			case GLFW_RELEASE:
				fprintf(stderr, "Left Mouse Released\n");
				break;
			default:
				fprintf(stderr, "Left Mouse Default Action?\n");
				break;
			}
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			switch (action)
			{
			case GLFW_PRESS:
				fprintf(stderr, "Right Mouse Pressed\n");
				break;
			case GLFW_RELEASE:
				fprintf(stderr, "Right Mouse Released\n");
				break;
			default:
				fprintf(stderr, "Right Mouse Default Action?\n");
				break;
			}
			break;
		default:
			fprintf(stderr, "Default Mouse Button?\n");
			break;
	}
}

// Detect mouse position
void GameManager::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	
}

// Detect mouse scroll
void GameManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	cerr << "Fov: " << fov << endl;
	fov -= yoffset;
	fov = glm::clamp(fov, 1.0f, 270.0f);
	Window::projection =
		glm::perspective(glm::radians(fov), (float) Window::width / (float) Window::height, 0.1f, 1000.0f);
}

// TODO: Move this to render class
// Draw objects to screen
void GameManager::render()
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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
	ImGui::Begin("Health UI", &showUI, windowFlags);
	ImGui::SetWindowPos(ImVec2(50, Window::height - 150));
	ImGui::SetWindowSize(ImVec2(300, 100));

	static float health = 0;
	static float direction = 1;
	health += 0.001 * direction;

	if (health > 1) direction = -1;
	if (health < 0) direction = 1;

	ImGui::Text("Super basic health bar");
	ImGui::SliderFloat("Health", &health, 0, 1);
	ImGui::End();

	// super basic crosshair, maybe move this somewhere else
	ImGui::Begin("Crosshairs", &showUI, windowFlags);
	ImGui::SetWindowPos(ImVec2(Window::width / 2 - 20, Window::height / 2 - 20));
	ImGui::SetWindowSize(ImVec2(200, 200));

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 p = ImGui::GetCursorScreenPos();
	drawList->AddLine(ImVec2(p.x+20, p.y), ImVec2(p.x+20, p.y+40), ImColor(ImVec4(0, 1, 0, 1)), 1.0);
	drawList->AddLine(ImVec2(p.x, p.y+20), ImVec2(p.x+40, p.y+20), ImColor(ImVec4(0, 1, 0, 1)), 1.0);
	ImGui::End();

	// Render the models
	worldT->draw(glm::mat4(1), Window::projection * camera->view);

	// call ImGUI render to actually render the ui to opengl
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Swap buffers
	glfwSwapBuffers(window);
}

void GameManager::updateMap(MapState& ms) {

	for (float t : ms.transform1)
	{
		//cerr << "CLIENT MAP STATE TRANSFORM" << endl;
		cerr << t << endl;
		/*cerr << t[1] << endl;
		cerr << t[2] << endl;
		cerr << t[3] << endl;
		cerr << t[4] << endl;
		cerr << t[5] << endl;
		cerr << t[6] << endl;
		cerr << t[7] << endl;
		cerr << t[8] << endl;
		cerr << t[9] << endl;
		cerr << t[10] << endl;
		cerr << t[11] << endl;
		cerr << t[12] << endl;
		cerr << t[13] << endl;
		cerr << t[14] << endl;
		cerr << t[15] << endl;*/
	}
	for (float t : ms.transform2)
	{
		cerr << t << endl;
	}    for (float t : ms.transform3)
	{
		cerr << t << endl;
	}

	Transform* newTrans = new Transform(ms.transform1);

	cubeT1->translation = newTrans->translation;
	cubeT2->translation = newTrans->translation;
	cubeT3->translation = newTrans->translation;

	std::cout << glm::to_string(newTrans->transform) << std::endl;

	cubeT1->rotation = newTrans->rotation;
	cubeT2->rotation = newTrans->rotation;
	cubeT3->rotation = newTrans->rotation;

	cubeT1->scale = newTrans->scale;
	cubeT2->scale = newTrans->scale;
	cubeT3->scale = newTrans->scale;

	delete newTrans;
}