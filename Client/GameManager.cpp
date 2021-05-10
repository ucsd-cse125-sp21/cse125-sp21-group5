#include "GameManager.h"

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
	cubeT = new Transform();
	playerT = new Transform(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(15.0f, 0.0f, 0.0));
	monkeT = new Transform(glm::vec3(0.25f), glm::vec3(0.0f), glm::vec3(-15.0f, 0.0f, 0.0f));

	// Initialize models to render
	Model* playerM = new Model("res/models/head2.dae");
	Model* monkeM = new Model("res/models/head2.dae");
	Model* cube = new Model("res/models/unitCube.dae");
	
	// Build scene graph
	//worldT->add_child(playerT);
	//worldT->add_child(monkeT);
	//playerT->add_child(playerM);
	//monkeT->add_child(monkeM);
	//cubeT->add_child(cube);
	//worldT->add_child(cubeT);


	// Initialize time variables
	deltaTime = 0.0f;
	prevTime = 0.0f;
	currTime = 0.0f;
}

GameManager::~GameManager()
{
	// Delete all models
	delete worldT; // Recursively calls destructor for all nodes... hopefully
}

Event GameManager::update()
{
	// Calculate deltaTime
	currTime = (float) glfwGetTime();
	deltaTime = currTime - prevTime;
	prevTime = currTime;

	// Rendering of objects is done here. (Draw)
	//render();

	// Listen for any events (keyboard input, mouse input, etc)
	glfwPollEvents();

	// Process keyboard input
	Event e = handleInput();

	// Testing scene graph
	//playerT->translate(glm::vec3(-0.001f, 0.0f, 0.0f));
	//monkeT->translate(glm::vec3(0.001f, 0.0f, 0.0f));

	cubeT->setTranslate(camera->pos + 5.0f * camera->front);

	

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

	// Render the models
	worldT->draw(glm::mat4(1), Window::projection * camera->view);

	//tile->draw(camera->view, Window::projection, shader);
	//cube->draw(camera->view, Window::projection, shader);
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

	Transform* cubeT1 = new Transform(ms.transform1);
	Transform* cubeT2 = new Transform(ms.transform2);
	Transform* cubeT3 = new Transform(ms.transform3);

	Model* cubeM = new Model("res/models/head2.dae");

	cubeT1->add_child(cubeM);
	cubeT2->add_child(cubeM);
	cubeT3->add_child(cubeM);

	worldT->add_child(cubeT1);
	worldT->add_child(cubeT2);
	worldT->add_child(cubeT3);
}