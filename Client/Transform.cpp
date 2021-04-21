#include "Transform.h"

Transform::Transform() {

}

Transform::~Transform() {

}

glm::mat4 Transform::translate(glm::vec3 t) {
	T = glm::translate(glm::mat4(1.0f), t);
	return T;
}

// in radians
glm::mat4 Transform::rotate(glm::vec3 axis, float angle) {
	R = glm::rotate(glm::mat4(1.0f), angle, axis);
	return R;
}



glm::mat4 Transform::resize(float scaleFactor) {
	S = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor));
	return S;
}