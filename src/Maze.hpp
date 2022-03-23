#pragma once 

#include "Mesh.hpp"
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
			Assimp::Importer importer; 
			const aiScene* mazeWallMesh = importer.ReadFile("assets/objects/MazeWall_unoptimized.obj", 0);
			mazeWall = new Mesh(mazeWallMesh->mMeshes[0]);

			const aiScene* prizeMesh = importer.ReadFile("assets/objects/prize.obj", 0);
			prize = new Mesh(prizeMesh->mMeshes[0]);
			for (int32_t i = 0; i < mazeSize; i++) {
				for (int32_t j = 0; j < mazeSize; j++) {
					mazeMap[i][j] = false;
				}
			}

			std::mt19937 pseudorandom(seed);
			std::vector<MazeCoordinate> walls;	
			walls.push_back({mazeSize / 2, mazeSize / 2});

			do {
				size_t randomPick = pseudorandom() % walls.size();			
				MazeCoordinate pickedWall = walls.at(randomPick);
				// std::printf("%d\n", walls.size());
				walls.erase(walls.begin() + randomPick);
				// std::printf("%d\n", walls.size());
				
				if (pickedWall.i == 0 || pickedWall.i == (mazeSize - 1) || pickedWall.j == 0 || pickedWall.j == (mazeSize - 1)) {
					continue;
				}

				if (mazeMap[pickedWall.i][pickedWall.j]) continue;

				int32_t adjacentPassageCount = 0;
				for (int32_t di = -1; di <= 1; di++) {
					for (int32_t dj = -1; dj <= 1; dj++) {
						if ((di == 0 && dj == 0) || di * dj != 0) continue;
						adjacentPassageCount += (mazeMap[pickedWall.i + di][pickedWall.j + dj] ? 1 : 0);
						// if (mazeMap[pickedWall.i + di][pickedWall.j + dj]) std::printf("Hello");
					}
				}
				// std::printf("\tAdj : %d\n", adjacentPassageCount); 

				if (adjacentPassageCount > 1) {
					continue;
				} 	
				
				mazeMap[pickedWall.i][pickedWall.j] = true;
				walls.push_back({pickedWall.i - 1, pickedWall.j});
				walls.push_back({pickedWall.i + 1, pickedWall.j});
				walls.push_back({pickedWall.i, pickedWall.j - 1});
				walls.push_back({pickedWall.i, pickedWall.j + 1});

				destination = pickedWall;
				// std::printf("%d\n", walls.size());
			} while (walls.size() > 0);
			
			wallMatrices.reserve(mazeSize * mazeSize);
			for (int32_t i = 0; i < mazeSize; i++) { for (int32_t j = 0; j < mazeSize; j++) {
				if (!mazeMap[i][j]) {
					wallMatrices.push_back(glm::translate(glm::mat4(1.0f), {(float)(i - mazeSize/2) * MAZE_CELL_SIZE , 0.0f, (float)(j - mazeSize/2) * MAZE_CELL_SIZE}));
				}
			}}

			prizeMatrix = glm::translate(glm::mat4(1.0f), {(float)(destination.i - mazeSize/2) * MAZE_CELL_SIZE , 0.0f, (float)(destination.j - mazeSize/2) * MAZE_CELL_SIZE});
			
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

		void drawWalls(glm::mat4 camMatrix, GLuint matrixID) {
			/*
			for (int32_t i = 0; i < mazeSize; i++) { for (int32_t j = 0; j < mazeSize; j++) {
				if (!mazeMap[i][j]) {
					glm::mat4 mvp = camMatrix * glm::translate(glm::mat4(1.0f), {(float)(i - mazeSize/2) * MAZE_CELL_SIZE , 0.0f, (float)(j - mazeSize/2) * MAZE_CELL_SIZE});
					glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
					mazeWall->draw();
				}
			}} */
			for (glm::mat4& transformMatrix : wallMatrices) {
				glm::mat4 mvp = camMatrix * transformMatrix;
				glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
				mazeWall->draw();
			}
			glm::mat4 mvp = camMatrix * prizeMatrix;
			glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
			prize->draw();	
		}

		bool isWall(int32_t i, int32_t j) {
			return !mazeMap[i][j];
		}

		bool isPassage(float x, float z) {
			int32_t i = (int)std::round(x / MAZE_CELL_SIZE + (float)mazeSize / 2), j = (int)std::round(z / MAZE_CELL_SIZE + (float)mazeSize / 2);
			if (i < 0 || i >= mazeSize || j < 0 || j >= mazeSize) return true;
			return mazeMap[i][j];
		}

		bool atDest(float x, float z) {
			int32_t i = (int)std::round(x / MAZE_CELL_SIZE + (float)mazeSize / 2), j = (int)std::round(z / MAZE_CELL_SIZE + (float)mazeSize / 2);
			return destination.i == i && destination.j == j;
		}

	private: 
		Mesh* mazeWall;
		Mesh* prize;

		MazeCoordinate destination;
		std::vector<glm::vec3> corners;
		bool mazeMap[mazeSize][mazeSize];		

		std::vector<glm::mat4> wallMatrices;
		glm::mat4 prizeMatrix;
};
