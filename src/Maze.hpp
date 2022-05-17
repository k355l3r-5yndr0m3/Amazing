#pragma once 

#include "Mesh3D.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cmath>
#include <cstdint>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <random>
#include <vector>
#include <cstdio> 

#include <glm/glm.hpp>

#define MAZE_CELL_SIZE 8.0f

struct MazeCoordinate {
	int32_t i, j;
};

template<int32_t mazeSize>
class PrimMaze {
	public: 
		PrimMaze(int32_t seed) {
			mazeWall = new Mesh3D;
			mazeWall->loadFromFile("assets/objects/MazeWall_unoptimized_uv.obj", "assets/textures/BrickTexture.png");
	
			prize = new Mesh3D;
			prize->loadFromFile("assets/objects/prize.obj", "assets/textures/yellow.jpg");

			for (int32_t i = 0; i < mazeSize; i++) {
				for (int32_t j = 0; j < mazeSize; j++) {
					mazeMap[i][j] = false;
				}
			}

			pseudorandom.seed(seed);
			std::vector<MazeCoordinate> walls;	
			walls.push_back({mazeSize / 2, mazeSize / 2});

			do {
				size_t randomPick = pseudorandom() % walls.size();			
				MazeCoordinate pickedWall = walls.at(randomPick);
				walls.erase(walls.begin() + randomPick);
				
				if (pickedWall.i == 0 || pickedWall.i == (mazeSize - 1) || pickedWall.j == 0 || pickedWall.j == (mazeSize - 1)) {
					continue;
				}

				if (mazeMap[pickedWall.i][pickedWall.j]) continue;

				int32_t adjacentPassageCount = 0;
				for (int32_t di = -1; di <= 1; di++) {
					for (int32_t dj = -1; dj <= 1; dj++) {
						if ((di == 0 && dj == 0) || di * dj != 0) continue;
						adjacentPassageCount += (mazeMap[pickedWall.i + di][pickedWall.j + dj] ? 1 : 0);
					}
				}

				if (adjacentPassageCount > 1) {
					continue;
				} 	
				
				mazeMap[pickedWall.i][pickedWall.j] = true;
				walls.push_back({pickedWall.i - 1, pickedWall.j});
				walls.push_back({pickedWall.i + 1, pickedWall.j});
				walls.push_back({pickedWall.i, pickedWall.j - 1});
				walls.push_back({pickedWall.i, pickedWall.j + 1});

				destination = pickedWall;
			} while (walls.size() > 0);
			
			wallMatrices.reserve(mazeSize * mazeSize);
			for (int32_t i = 0; i < mazeSize; i++) { for (int32_t j = 0; j < mazeSize; j++) {
				if (!mazeMap[i][j]) {
					wallMatrices.push_back(glm::translate(glm::mat4(1.0f), {(float)(i - mazeSize/2) * MAZE_CELL_SIZE , 0.0f, (float)(j - mazeSize/2) * MAZE_CELL_SIZE}));
				}
			}}

			prizeMatrix = glm::translate(glm::mat4(1.0f), {(float)(destination.i - mazeSize/2) * MAZE_CELL_SIZE , 4.0f, (float)(destination.j - mazeSize/2) * MAZE_CELL_SIZE});
			
		}
		~PrimMaze() {
			delete mazeWall;
			delete prize;
		}

		void printMaze() {
			for (uint32_t i = 0; i < mazeSize; i++) {
				for (uint32_t j = 0; j < mazeSize; j++) {
					std::printf("%c ", mazeMap[i][j] ? 'O' : 'X');
				}
				std::printf("\n");
			}
		}

		void drawWalls(glm::mat4 camMatrix, GLuint matrixID, GLuint color) {
			for (glm::mat4& transformMatrix : wallMatrices) {
				glm::mat4 mvp = camMatrix * transformMatrix;
				glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
				mazeWall->draw();
			}
			glm::mat4 mvp = camMatrix * prizeMatrix;
			glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

			glUniform3f(color, 0.1f, 0.8f, 0.9f);
			prize->draw();	
		}

		bool isWall(int32_t i, int32_t j) {
			return !mazeMap[i][j];
		}

		bool isPassage(const glm::vec3& pos) {
			int32_t i = (int)std::round(pos.x / MAZE_CELL_SIZE + (float)mazeSize / 2), j = (int)std::round(pos.z / MAZE_CELL_SIZE + (float)mazeSize / 2);
			if (i < 0 || i >= mazeSize || j < 0 || j >= mazeSize) return true;
			return mazeMap[i][j];
		}

		bool atDest(float x, float z) {
			int32_t i = (int)std::round(x / MAZE_CELL_SIZE + (float)mazeSize / 2), j = (int)std::round(z / MAZE_CELL_SIZE + (float)mazeSize / 2);
			return destination.i == i && destination.j == j;
		}

		glm::vec3 getRandomEmptyPoint() {
			int32_t i, j;
			do {
				i = pseudorandom() % mazeSize;
				j = pseudorandom() % mazeSize;
			} while (!mazeMap[i][j]);
			return {(float)(i - mazeSize/2) * MAZE_CELL_SIZE , 0.0f, (float)(j - mazeSize/2) * MAZE_CELL_SIZE};
		}

		
		std::mt19937 pseudorandom;
		
		bool isReachable(glm::vec3 org, glm::vec3 dst) {
			int32_t i_0 = (int)std::round(org.x / MAZE_CELL_SIZE + (float)mazeSize / 2), j_0 = (int)std::round(org.z / MAZE_CELL_SIZE + (float)mazeSize / 2);
			int32_t i_1 = (int)std::round(dst.x / MAZE_CELL_SIZE + (float)mazeSize / 2), j_1 = (int)std::round(dst.z / MAZE_CELL_SIZE + (float)mazeSize / 2);
			
			int32_t test = (i_0 - i_1) * (j_0 - j_1);
			return test == 0;
		}
		
		// So much spighetty code 
		MazeCoordinate request_new_heading(MazeCoordinate old, MazeCoordinate current) {
			std::vector<MazeCoordinate> temp;

			// This is slightly unsafe, but it ought not to matter 
			if (mazeMap[current.i - 1][current.j] && old.i != current.i - 1) temp.push_back({current.i - 1, current.j});
			if (mazeMap[current.i + 1][current.j] && old.i != current.i + 1) temp.push_back({current.i + 1, current.j});
			if (mazeMap[current.i][current.j - 1] && old.j != current.j - 1) temp.push_back({current.i, current.j - 1});
			if (mazeMap[current.i][current.j + 1] && old.j != current.j + 1) temp.push_back({current.i, current.j + 1});

			if (temp.size() == 0) return old;
			return temp[pseudorandom() % temp.size()];
		}

		MazeCoordinate random_point() {
			while (true) {
				int i = pseudorandom() % mazeSize, j = pseudorandom() % mazeSize;
				if (mazeMap[i][j]) return {i, j};
			}
		}
		MazeCoordinate random_point(MazeCoordinate a) {
			std::vector<MazeCoordinate> temp;

			// This is slightly unsafe, but it ought not to matter 
			if (mazeMap[a.i - 1][a.j]) temp.push_back({a.i - 1, a.j});
			if (mazeMap[a.i + 1][a.j]) temp.push_back({a.i + 1, a.j});
			if (mazeMap[a.i][a.j - 1]) temp.push_back({a.i, a.j - 1});
			if (mazeMap[a.i][a.j + 1]) temp.push_back({a.i, a.j + 1});

			return temp[pseudorandom() % temp.size()];
		}

		inline glm::vec3 intcoordtofloatcoord(MazeCoordinate c) {
			return {(float)(c.i - mazeSize/2) * MAZE_CELL_SIZE , 0.0f, (float)(c.j - mazeSize/2) * MAZE_CELL_SIZE};
		}

	private: 

		Mesh3D* mazeWall;
		Mesh3D* prize;

		MazeCoordinate destination;
		std::vector<glm::vec3> corners;
		bool mazeMap[mazeSize][mazeSize];		

		std::vector<glm::mat4> wallMatrices;
		glm::mat4 prizeMatrix;
};

typedef PrimMaze<24> PrimMaze64;
