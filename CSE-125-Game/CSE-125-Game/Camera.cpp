#include "Camera.h"

/*
 * TODO: This will need to be changed to support 1st person view
 * Starts off facing the 'center' of the screen at (0,0,0) in world coord
 * 
 */

Camera::Camera()
{
	/*
	fov = 45.0f;
	aspect = 1.33f;
	nearClip = 0.1f;
	farClip = 100.0f;

	distance = 10.0f;
	azimuth = 0.0f;
	incline = 20.0f;
	*/

	// Camera properties
	speed = 2.5f;
	sensitivity = 0.1f;
	yaw = 0.0f;
	pitch = 0.0f;
	pos = glm::vec3(0.0f, 0.0f, 20.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(pos, pos + front, up);
}

Camera::~Camera()
{
}

void Camera::update(float& deltaTime, float& offsetX, float& offsetY)
{
	/*
	// Compute camera world matrix
	glm::mat4 world(1);
	world[3][2] = distance;
	world = glm::eulerAngleY(glm::radians(-azimuth)) * glm::eulerAngleX(glm::radians(-incline)) * world;

	// Compute view matrix (inverse of world matrix)
	glm::mat4 view = glm::inverse(world);

	// Compute perspective projection matrix
	glm::mat4 project = glm::perspective(glm::radians(fov), aspect, nearClip, farClip);

	// Compute final view-projection matrix
	viewProjMat = project * view;
	*/

	// Calculate new pitch and yaw
	yaw += sensitivity * offsetX;
	pitch += sensitivity * offsetY;
	pitch = glm::clamp(pitch, -89.0f, 89.0f);

	// Calculate new front
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	// Have camera look at front
	view = glm::lookAt(pos, pos + front, up);

	// Ensure consistent movement despite diff fps
	speed = 2.5f * deltaTime;
}

void Camera::move(bool W, bool S, bool A, bool D, bool SPACE, bool L_CTRL)
{
	// Forward or backward
	if (W) pos += speed * front;
	else if (S) pos -= speed * front;

	// Left or right
	if (A) pos -= speed * glm::normalize(glm::cross(front, up));
	else if (D) pos += speed * glm::normalize(glm::cross(front, up));

	// Up or down
	if (SPACE) pos += speed * up;
	else if (L_CTRL) pos -= speed * up;
}

void Camera::reset()
{
	
}