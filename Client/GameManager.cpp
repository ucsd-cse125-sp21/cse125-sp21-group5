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
	playerT = new Transform(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(15.0f, 0.0f, 0.0));
	monkeT = new Transform(glm::vec3(0.25f), glm::vec3(0.0f), glm::vec3(-15.0f, 0.0f, 0.0f));

	// Initialize models to render
	Model* playerM = new Model("res/models/head2.dae");
	Model* monkeM = new Model("res/models/head2.dae");
	
	// Build scene graph
	worldT->add_child(playerT);
	worldT->add_child(monkeT);
	playerT->add_child(playerM);
	monkeT->add_child(monkeM);

	// TODO: Build Quadtree using DFS
	/*
	// Temporary "world"
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Tile* tile = new Tile((float) i, (float) j, 5.0f, 5.0f, 5);
			worldT->add_child(tile->tileT);
		}
	}
	*/

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

void GameManager::update(Client& client)
{
	// Calculate deltaTime
	currTime = (float) glfwGetTime();
	deltaTime = currTime - prevTime;
	prevTime = currTime;

	// Rendering of objects is done here. (Draw)
	// TODO: maybe have separate class in charge of rendering
	render();

	// Listen for any events (keyboard input, mouse input, etc)
	glfwPollEvents();

	// Process keyboard input
	handleKeyboardInput(client);
	
	// Tell server about any movements
	// TODO: how to wait for response?
	//Event e = Event(key, camera);
	//glm::vec3 newCamPos = this->client.callFakeServer();

	// Testing scene graph
	//playerT->translate(glm::vec3(-0.001f, 0.0f, 0.0f));
	//monkeT->translate(glm::vec3(0.001f, 0.0f, 0.0f));

	//playerT->collider->check_collision(monkeT->collider);

	//playerT->rotate(0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
	//monkeT->rotate(-0.1f, glm::vec3(0.0f, 0.0f, 1.0f));

	// Update camera position
	// TODO: place camera inside of Player class
	camera->update(deltaTime, offsetX, offsetY);
	offsetX = 0.0f;
	offsetY = 0.0f;
}

// Handle Keyboard Input
void GameManager::handleKeyboardInput(Client& client)
{
	// System Controls
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Player Controls
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		Event e(camera->front, camera->speed, camera->pos);
		client.callServer(e);
	}
	else if (glfwGetKey(window, GLFW_KEY_S))
		camera->move(-camera->front);
	if (glfwGetKey(window, GLFW_KEY_A))
		camera->move(-glm::normalize(glm::cross(camera->front, camera->up)));
	else if (glfwGetKey(window, GLFW_KEY_D))
		camera->move(glm::normalize(glm::cross(camera->front, camera->up)));
	if (glfwGetKey(window, GLFW_KEY_SPACE))
		camera->move(camera->up);
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
		camera->move(-camera->up);

	// Temporary Player 2 Controls
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
	// Special case for first mouse
	if (firstMouse)
	{
		lastX = (float) xpos;
		lastY = (float) ypos;
		firstMouse = false;
	}

	// Calculate offset from prev frame
	offsetX = (float) (xpos - lastX);
	offsetY = (float) (lastY - ypos);

	// Save previous positions
	lastX = (float) xpos;
	lastY = (float) ypos;
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
	worldT->draw(camera->view, Window::projection);

	//tile->draw(camera->view, Window::projection, shader);
	//cube->draw(camera->view, Window::projection, shader);
	// Swap buffers
	glfwSwapBuffers(window);
}