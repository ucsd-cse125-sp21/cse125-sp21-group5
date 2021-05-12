#include "Camera.h"

#include "Renderer.h"

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
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	// World coordinates to camera coordinates
	view = glm::lookAt(pos, pos + front, up);

	// tell the global renderer about this camera
	Renderer::get().setCamera(this);
}

Camera::~Camera()
{
}

void Camera::update(glm::vec3 pos, glm::vec3 front)
{
	// Have camera look at front
	this->pos = pos;
	this->front = front;
	view = glm::lookAt(pos, pos + front, up);
}

void Camera::move(const glm::vec3& pos) {
	this->pos = pos;
}

// TODO: Deprecated
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