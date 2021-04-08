// CSE-125-Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Window.h"

#include <glm/glm.hpp>
#include "Cube.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

int main(int argc, char** argv)
{
	GLFWwindow* window = Window::create(1280, 960, "CSE 169 Project 4");
	glClearColor(0.1, 0.1, 0.1, 1);

	Camera camera = Camera();

	GLuint shader = LoadShaders("res/shaders/shader.vert", "res/shaders/shader.frag");
	Cube cube = Cube();

	Model monke = Model("res/models/monke.dae");

	while (!Window::shouldClose(window)) {

		camera.setAzimuth(camera.getAzimuth() + 1.0f);
		camera.update();
		glm::mat4 viewProjectionMtx = camera.getViewProjMtx();

		monke.draw(glm::mat4(1), viewProjectionMtx, shader);

		Window::swapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}


//GOAL: render out a cube. Render/update fnc.