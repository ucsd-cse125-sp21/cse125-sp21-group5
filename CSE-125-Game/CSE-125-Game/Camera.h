#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();

	void update();
	void reset();

	void setAspect(float a) { aspect = a; }
	void setDistance(float d) { distance = d; }
	void setAzimuth(float a) { azimuth = a; }
	void setIncline(float i) { incline = i; }

	float getDistance() { return distance; }
	float getAzimuth() { return azimuth; }
	float getIncline() { return incline; }

	glm::mat4 getViewProjMtx() { return viewProjectMtx; }

private:
	// Perspective controls
	float fov;			// Field of View Angle (degrees)
	float aspect;		// Aspect Ratio
	float nearClip;		// Near clipping plane distance
	float farClip;		// Far clipping plane distance

	// Polar controls
	float distance;		// Distance of the camera eye position to the origin (meters)
	float azimuth;		// Rotation of the camera eye position around the Y axis (degrees)
	float incline;		// Angle of the camera eye position over the XZ plane (degrees)

	// Computed data
	glm::mat4 viewProjectMtx;
};
