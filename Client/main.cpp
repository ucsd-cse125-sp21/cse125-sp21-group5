// CSE-125-Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "main.h"
#include "Client.h"

// Print errors
void error_callback(int error, const char* description)
{
	// Print error.
	std::cerr << description << std::endl;
}

// Assign the callback functions to the appropriate functions
void setup_callbacks(GLFWwindow* window)
{
	glfwSetErrorCallback(error_callback);
	glfwSetWindowSizeCallback(window, Window::resizeCallback);
	glfwSetKeyCallback(window, GameManager::keyCallback);
	glfwSetMouseButtonCallback(window, GameManager::mouseButtonCallback);
	glfwSetCursorPosCallback(window, GameManager::cursorPositionCallback);
	glfwSetScrollCallback(window, GameManager::scrollCallback);
}

void setup_opengl_settings()
{
	// Enable depth buffering.
	glEnable(GL_DEPTH_TEST);

	// Related to shaders and z value comparisons for the depth buffer.
	glDepthFunc(GL_LEQUAL);

	// Set polygon drawing mode to fill front and back of each polygon.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Set clear color to black.
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions()
{
	// Get info of GPU and supported OpenGL version.
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version supported: " << glGetString(GL_VERSION)
		<< std::endl;

	//If the shading language symbol is defined.
	#ifdef GL_SHADING_LANGUAGE_VERSION
		std::cout << "Supported GLSL version is: " <<
			glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	#endif
}

int main(int argc, char** argv)
{
	// Create GLFW window
	GLFWwindow* window = Window::create(1280, 960, "CSE 125 Game");
	if (window == nullptr)
	{
		fprintf(stderr, "Window could not be created\n");
	}

	// Print OpenGL and GLSL versions.
	print_versions();

	// Setup callbacks.
	setup_callbacks(window);

	// Setup OpenGL settings.
	setup_opengl_settings();

	// TODO: move this somewhere
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	// Forever game loop
	boost::asio::io_context ioContext;
	Client client(ioContext, window);

	boost::thread_group worker_threads;
	worker_threads.create_thread(                            
		boost::bind(&boost::asio::io_service::run, &ioContext)
	);

	float start = glfwGetTime();
	unsigned int frameCount = 0;
	while (!glfwWindowShouldClose(window)) {
		float end = glfwGetTime();
		if (end - start <= (1.0f / 60))
		{
			continue;
		}

		start = glfwGetTime();
		client.callServer();
	}

	// destroy objects created
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Window::cleanUp();

	// Destroy the window.
	glfwDestroyWindow(window);

	// Terminate GLFW.
	glfwTerminate();

	exit(EXIT_SUCCESS);
}