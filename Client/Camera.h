#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtx/euler_angles.hpp"

using namespace std;

class Camera
{
private:
	
public:
	Camera();
	~Camera();

	// Camera properties
	glm::vec3 pos; // Camera position.
	glm::vec3 front; // The point we are looking at.
	glm::vec3 up; // The up direction of the camera.
	glm::mat4 view;

	// Camera movement
	float speed;
	float sensitivity;
	float yaw;
	float pitch;

	void update(float& deltaTime, float& offsetX, float& offsetY);
	void reset();

	void move(const glm::vec3& vec);
	void move(bool W, bool S, bool A, bool D, bool SPACE, bool L_CTRL);
};

#endif