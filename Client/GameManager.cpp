#include "GameManager.h"

// Track keyboard movements
bool GameManager::W = false;
bool GameManager::S = false;
bool GameManager::A = false;
bool GameManager::D = false;
bool GameManager::SPACE = false;
bool GameManager::L_CTRL = false;

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
	playerT = new Transform();

	// Initialize models to render
	Model* playerM = new Model("res/models/head2.dae");
	Model* playerM1 = new Model("res/models/head2.dae");

	// Build scene graph
	worldT->add_child(playerT);
	playerT->add_child(playerM);

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

void GameManager::update()
{
	// Calculate deltaTime
	currTime = glfwGetTime();
	deltaTime = currTime - prevTime;
	prevTime = currTime;

	// Rendering of objects is done here. (Draw)
	// TODO: maybe have separate class in charge of rendering
	render();

	// Listen for any events (keyboard input, mouse input, etc)
	glfwPollEvents();

	// Testing scene graph
	worldT->rotate(0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
	playerT->translate(glm::vec3(0.0f, 0.01f, 0.0f));

	// Update camera position
	// TODO: place camera inside of Player class
	camera->move(W, S, A, D, SPACE, L_CTRL);
	camera->update(deltaTime, offsetX, offsetY);
	offsetX = 0.0f;
	offsetY = 0.0f;
}

// Detect different keyboard inputs
void GameManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// TODO: Move this to KeyboardInputManager
	switch (key)
	{
	case GLFW_KEY_UNKNOWN:
		fprintf(stderr, "Unknown keypressed\n");
		break;
	case GLFW_KEY_ESCAPE:
		// Close the window. This causes the program to also terminate.
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_W:
		W = action != GLFW_RELEASE;
		break;
	case GLFW_KEY_S:
		S = action != GLFW_RELEASE;
		break;
	case GLFW_KEY_A:
		A = action != GLFW_RELEASE;
		break;
	case GLFW_KEY_D:
		D = action != GLFW_RELEASE;
		break;
	case GLFW_KEY_SPACE:
		SPACE = action != GLFW_RELEASE;
		break;
	case GLFW_KEY_LEFT_CONTROL:
		L_CTRL = action != GLFW_RELEASE;
		break;
	default:
		break;
	}
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
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	// Calculate offset from prev frame
	offsetX = xpos - lastX;
	offsetY = lastY - ypos;

	// Save previous positions
	lastX = xpos;
	lastY = ypos;
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