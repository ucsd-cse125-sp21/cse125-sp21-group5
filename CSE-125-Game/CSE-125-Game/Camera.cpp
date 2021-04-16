#include "Camera.h"

/*
 * TODO: This will need to be changed to support 1st person view
 * Starts off facing the 'center' of the screen at (0,0,0) in world coord
 * 
 */

#include "glm/gtx/euler_angles.hpp"

Camera::Camera() {
	reset();
}

void Camera::update() {
	// Compute camera world matrix
	glm::mat4 world(1);
	world[3][2] = distance;
	world = glm::eulerAngleY(glm::radians(-azimuth)) * glm::eulerAngleX(glm::radians(-incline)) * world;

	// Compute view matrix (inverse of world matrix)
	glm::mat4 view = glm::inverse(world);

	// Compute perspective projection matrix
	glm::mat4 project = glm::perspective(glm::radians(fov), aspect, nearClip, farClip);

	// Compute final view-projection matrix
	viewProjectMtx = project * view;
}

void Camera::reset() {
	fov = 45.0f;
	aspect = 1.33f;
	nearClip = 0.1f;
	farClip = 100.0f;

	distance = 10.0f;
	azimuth = 0.0f;
	incline = 20.0f;
}