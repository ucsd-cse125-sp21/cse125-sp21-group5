#ifndef _CAMERA_H_
#define _CAMERA_H_

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

	void update(glm::vec3 pos, glm::vec3 front);

	void move(const glm::vec3& pos);
	void move(bool W, bool S, bool A, bool D, bool SPACE, bool L_CTRL);
};

#endif