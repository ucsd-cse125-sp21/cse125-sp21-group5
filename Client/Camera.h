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

	/*
	// Perspective controls
	float fov;			// Field of View Angle (degrees)
	float aspect;		// Aspect Ratio
	float nearClip;		// Near clipping plane distance
	float farClip;		// Far clipping plane distance

	// Polar controls
	float distance;		// Distance of the camera eye position to the origin (meters)
	float azimuth;		// Rotation of the camera eye position around the Y axis (degrees)
	float incline;		// Angle of the camera eye position over the XZ plane (degrees)
	*/

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

	// Computed data
	//glm::mat4 viewProjMat;

	void update(float& deltaTime, float& offsetX, float& offsetY);
	void reset();

	/*
	void setAspect(float a) { aspect = a; }
	void setDistance(float d) { distance = d; }
	void setAzimuth(float a) { azimuth = a; }
	void setIncline(float i) { incline = i; }

	float getDistance() { return distance; }
	float getAzimuth() { return azimuth; }
	float getIncline() { return incline; }

	glm::mat4 getViewProjMtx() { return viewProjectMtx; }
	*/

	void move(bool W, bool S, bool A, bool D, bool SPACE, bool L_CTRL);
};

#endif