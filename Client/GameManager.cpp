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

	/*// TODO: Build Quadtree using DFS
	BoxCollider worldBox = BoxCollider(glm::vec3(0.0f, 0.0f, 10.0f), 
		glm::vec3(500.0f, 500.0f, 500.0f));
	// TODO: need to make this gneric 
	vector<BoxCollider*> objects;
	world = new Quadtree(worldBox, 4, objects);

	// Keeping track of all the colliders in the world
	allColliders.push_back(playerT->collider);
	allColliders.push_back(monkeT->collider);

	// inserting the collider into the quadtree to ensure fast computation
	Node* root = worldT;


	// TODO: node doesn't have necessary functions to perform DFS 

	world->insert(playerT->collider);
	world->insert(monkeT->collider); 
	//string pp = typeid(playerM).name();
	std::cerr << typeid(playerM).name() << std::endl; */
	
	// Temporary "world"
	/*for (int i = 0; i < 3; i++)
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

// TODO: 
// Idea 1: put collider as part of Model class 
// Idea 2: Server doesn't have model, but instead of model on server, the server
//		   just has the collider 
// 
// 
/*void GameManager::buildQuadtreeDFS(Node* root, Quadtree * qt) {
	if (!root) return;
	string objectType = typeid(root).name();

	// Case where it's a model, return, since no collider
	if (objectType.find("Model") != std::string::npos) {
		return;
	}

	// Main for loop
	for (Node* n : root->children) {
		// Leaf node
		if (!n->children) {
			
			//TODO node needs collider too
			qt->insert(n->collider);
		}

	}
	
}*/

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
	handleInput(client);
	
	// Tell server about any movements
	// TODO: how to wait for response?
	//Event e = Event(key, camera);
	//glm::vec3 newCamPos = this->client.callFakeServer();

	// Testing scene graph
	playerT->translate(glm::vec3(-0.001f, 0.0f, 0.0f));
	monkeT->translate(glm::vec3(0.001f, 0.0f, 0.0f));
	
	// TODO: Change this to be a dynamic check of just all the colliders
	// currently in the world 
	/*playerT->collider->check_collision(monkeT->collider);
	
	// This checks ALL current colliders existing in the world, 
	// then it queries the current collider's surroundings through the 
	// quadtree, and then see if it is colliding with any of the 
	// points in its current surrounding. 
	for (auto collider : allColliders) {
		//std::cerr << "outer loop" << std::endl;
		BoxCollider colliderRange = BoxCollider(collider->center,
			glm::vec3(collider->length, collider->width, collider->height) * 2.0f);
		vector<BoxCollider*> nearbyColliders;
		nearbyColliders = world->query(&colliderRange, nearbyColliders);
		for (auto otherCollider : nearbyColliders) {
			if (collider != otherCollider && collider->check_collision(otherCollider)) {
				std::cerr << "Collided" << endl;
			}
		}
	}*/

	// TODO, could put quadtree here to update every frame 


	//playerT->rotate(0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
	//monkeT->rotate(-0.1f, glm::vec3(0.0f, 0.0f, 1.0f));

	// Update camera position
	// TODO: place camera inside of Player class
	offsetX = 0.0f;
	offsetY = 0.0f;
}

// Handle Keyboard Input
void GameManager::handleInput(Client& client)
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

	//Update mouse movements
	float yaw = camera->sensitivity * offsetX;
	float pitch = camera->sensitivity * offsetY;

	Event e(toSend, yaw, pitch);
	client.callServer(e);
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