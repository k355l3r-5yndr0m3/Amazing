#include "Enemy.hpp" 
#include <cmath>
#include <glm/common.hpp>
#include <glm/ext/quaternion_common.hpp>

extern float delta;

#define RND_RES 4096

Enemy::Enemy(PrimMaze64* maze, float magnitude) {
	this->maze = maze;

	prev = maze->random_point();
	next = maze->random_point(prev);


	prev_cell = maze->intcoordtofloatcoord(prev);
	next_cell = maze->intcoordtofloatcoord(next);

	prev_cell.y = 2.0f;
	next_cell.y = 2.0f;

	s = 0.0;

	phi_x = static_cast<float>(maze->pseudorandom() % RND_RES) / static_cast<float>(RND_RES);
	phi_y = static_cast<float>(maze->pseudorandom() % RND_RES) / static_cast<float>(RND_RES);
	phi_z = static_cast<float>(maze->pseudorandom() % RND_RES) / static_cast<float>(RND_RES);

	position = prev_cell;
}

glm::vec3 Enemy::getPosition() {
	return position + glm::vec3(glm::cos((s + phi_x) * 2 * M_PI), glm::sin((phi_y + s) * 2 * M_PI), glm::cos(glm::sin((s + phi_z) * 2 * M_PI) * 2 * M_PI));
}

void Enemy::update() {
	if (s > 1.0) {
		auto tmp = maze->request_new_heading(prev, next);
		prev = next;
		next = tmp;

		prev_cell = maze->intcoordtofloatcoord(prev);
		next_cell = maze->intcoordtofloatcoord(next);
		prev_cell.y = 2.0f;
		next_cell.y = 2.0f;
		
		s = 0.0;
	}
		
	position = prev_cell * (1.0f - s) + next_cell * s;

	s += WANDER_SPEED * delta;
}
