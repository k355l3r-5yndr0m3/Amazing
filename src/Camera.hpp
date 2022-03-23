#pragma once 

#include <glm/glm.hpp>

class Camera {
	public:
		Camera(int w, int h, glm::vec3 position = {0.0f, 0.0f, 0.0f}, glm::vec2 facing = {0.0f, 0.0f});
		~Camera();

		glm::mat4 getMatrix();
		void translate(const glm::vec3& translation);
		void setPosition(const glm::vec3& position);
		void rotate(const glm::vec2& angle);

		glm::vec3 getFacingDirection();
		glm::vec3 getPosition();
		glm::vec2 getFacing();
	private: 
		int w, h;

		glm::vec3 position;
		glm::vec2 facing;
};
