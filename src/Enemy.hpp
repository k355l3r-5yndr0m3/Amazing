#pragma once
#include <glm/vec3.hpp> 

#include "Maze.hpp"

#define WANDER_SPEED 2.8f

class Enemy {
	public: 
		Enemy(PrimMaze64* maze, float magnitude = 0.2f);
		
		glm::vec3 getPosition();
		void update();
	private: 
		glm::vec3 next_cell;
		glm::vec3 prev_cell;

		glm::vec3 position;
		
		MazeCoordinate prev;
		MazeCoordinate next;

		float s = 0.0f;
		float phi_x, phi_y, phi_z;

		PrimMaze64* maze;		
		


};
