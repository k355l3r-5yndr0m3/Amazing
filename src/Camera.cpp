#include "Camera.hpp" 
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(int w, int h, glm::vec3 position, glm::vec2 facing) : w(w), h(h), position(position), facing(facing) {

}

Camera::~Camera() {

}



glm::mat4 Camera::getMatrix() {
	glm::vec3 direction = {glm::cos(facing.y) * glm::cos(facing.x), glm::sin(facing.y), glm::cos(facing.y) * glm::sin(facing.x)}; 
	return glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.01f, 800.0f) * glm::lookAt(position, position + direction, glm::vec3(0, 1, 0));
}

glm::vec3 Camera::getFacingDirection() {
	return {glm::cos(facing.y) * glm::cos(facing.x), glm::sin(facing.y), glm::cos(facing.y) * glm::sin(facing.x)};
}

void Camera::translate(const glm::vec3& translation) {
	position += translation;
}

void Camera::rotate(const glm::vec2& angle) {
	facing += angle;
	facing.y = std::fmin(std::fmax(facing.y, -M_PI / 2 + 0.001f), M_PI / 2 - 0.001f);
}

void Camera::setPosition(const glm::vec3 &position) {
	this->position = position;
}

glm::vec3 Camera::getPosition() {
	return position;
}

glm::vec2 Camera::getFacing() {
	return facing;
}
