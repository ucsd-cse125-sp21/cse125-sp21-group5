#include "Window.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Window Properties
int Window::width;
int Window::height;

// Camera Matrices
glm::mat4 Window::projection;

// Cleanup objects
void Window::cleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

GLFWwindow* Window::create(int width, int height, string title) {

	// Intialize GLFW
	if (!glfwInit()) {
		cerr << "Failed to initalize GLFW" << endl;
		return NULL;
	}

	// Set up GLFW window values
	glfwWindowHint(GLFW_SAMPLES, 4);					// set 4x MSAA
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);		// set GL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use new opengl features

	// Create the window
	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	// Check if window was actually created
	if (window == NULL) {
		cerr << "Failed to open GLFW window." << endl;
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		cerr << "Failed to init GLEW" << endl;
	}

	// set up ImGUI context on the window
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");


	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// don't want to let the mouse clicks interact with the 
	io.ConfigFlags |= ImGuiConfigFlags_NoMouse;

	io.Fonts->AddFontFromFileTTF("res/fonts/ptsans/PTC55F.ttf", 30);

	glEnable(GL_DEPTH_TEST);

	// Set swap interval to 0
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	glViewport(0, 0, width, height);
	Window::projection = 
		glm::perspective(glm::radians(60.0f), (float) width / (float) height, 0.1f, 1000.0f);
}
