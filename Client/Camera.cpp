#include "Camera.h"

/*
 * TODO: This will need to be changed to support 1st person view
 * Starts off facing the 'center' of the screen at (0,0,0) in world coord
 * 
 */

Camera::Camera()
{
	// Camera movement
	speed = 2.5f;
	sensitivity = 0.1f;

	// Camera rotation
	yaw = 0.0f;
	pitch = 0.0f;

	// Camera position
	pos = glm::vec3(0.0f, 0.0f, 20.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	// World coordinates to camera coordinates
	view = glm::lookAt(pos, pos + front, up);
}

Camera::~Camera()
{
}

void Camera::update(float& deltaTime, float& offsetX, float& offsetY)
{
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