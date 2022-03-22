#pragma once 

#include <cstdint>
#include <random>
#include <vector>
#include <cstdio> 

#include <glm/glm.hpp>

#define MAZE_CELL_SIZE 1.0f

struct MazeCoordinate {
	int32_t i, j;
};

template<int32_t mazeSize>
class PrimMaze {
	public: 
		PrimMaze(int32_t seed) {
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

				// std::printf("%d\n", walls.size());
			} while (walls.size() > 0);

		}
		~PrimMaze() {}

		void printMaze() {
			for (uint32_t i = 0; i < mazeSize; i++) {
				for (uint32_t j = 0; j < mazeSize; j++) {
					std::printf("%c ", mazeMap[i][j] ? 'O' : 'X');
				}
				std::printf("\n");
			}
		}
	private: 
		std::vector<glm::vec3> corners;
		bool mazeMap[mazeSize][mazeSize];		
};
