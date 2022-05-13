#include "Application.hpp" 

#include "Camera.hpp"
#include "Maze.hpp"
#include "ShaderProgram.hpp"
#include "glad/glad.h"
#include "Mesh3D.hpp"
#include "Enemy.hpp" 

#include <SDL2/SDL.h>
#include <algorithm>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cstdint>
#include <cstdio>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <fstream>
#include <vector>

#define PLAYER_SPEED 18.0f

float delta = 0.0f;

Application::Application() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw "SDL initialization failed"; 
	}
	if ((window = SDL_CreateWindow(applicationName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL)) == nullptr) {
		throw "Failed to create window";
	}
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_SetWindowGrab(window, SDL_TRUE);

	glcontext = SDL_GL_CreateContext(window);
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	glViewport(0, 0, windowWidth, windowHeight);
	
	glClearColor(0.4f, 0.1f, 0.8f, 0.1f);
	glClearDepth(1.0);

	glEnable(GL_DEPTH_TEST);
	
	standardShader = new ShaderProgram("assets/shaders/wall_material_vertex.glsl", "assets/shaders/wall_material_fragment.glsl");

	uint32_t seed;
	std::ifstream entropyPool("/dev/random");
	entropyPool.read(reinterpret_cast<char*>(&seed), sizeof(seed));
	std::printf("Maze seed: %u\n", seed);
	entropyPool.close();

	maze = new PrimMaze64(seed);
}

Application::~Application() {
	delete standardShader;
	delete maze;

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);

}

void Application::mainLoop() {
	std::printf("Find the icosphere\n");
	std::printf("There are floating cubes that will end the game if you touches them. While dumb, they can be quite erratic.\n");
	std::printf("Control using WASD\n");

	bool alive = true;
	Camera mainCam(windowWidth, windowHeight, {0.0f, 4.0f, 0.0f});

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	GLuint MatrixID = standardShader->uniformLocation("MVP");
	GLuint lightSource = standardShader->uniformLocation("lightSource");
	GLuint lightColor = standardShader->uniformLocation("lightColor");
	GLuint eyePos = standardShader->uniformLocation("eyePos");

	Mesh3D /*prize,*/ floor, enemy;
	floor.loadFromFile("assets/objects/floor.obj", "assets/textures/grey.jpg");
	enemy.loadFromFile("assets/objects/defaultCube.obj", "assets/textures/BrickTexture.png");


	std::vector<Enemy> enemies;
	for (int i = 0; i < 4; i++) {
		enemies.push_back(Enemy(maze));
	}

	glm::vec3 enemy_col = {0.5f, 0.1f, 9.0f};

	glm::vec3 velocity = {0.0f, 0.0f, 0.0f};
	glm::vec3 forward = mainCam.getFacingDirection();
	forward.y = 0.0f;
	
	glm::vec3 left = glm::cross({0.0f, 1.0f, 0.0f}, forward);
	float move_left = 0.0, move_forward = 0.0;

	prev_t = SDL_GetTicks();

	while (alive) {
		delta = SDL_GetTicks() - prev_t; 
		delta /= 1000.0f; // Convert to second 
		prev_t = SDL_GetTicks();


		SDL_Event event;
		forward = mainCam.getFacingDirection();
		forward.y = 0.0f; 
		forward = glm::normalize(forward);
		left = glm::cross({0.0f, 1.0f, 0.0f}, forward);

		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
				case SDL_QUIT:
					alive = false;
					break;
				case SDL_KEYDOWN: 
					switch (event.key.keysym.sym) {
						case SDLK_a:
							if (move_left == 0.0f) move_left = 1.0f;
							break;
						case SDLK_d: 
							if (move_left == 0.0f) move_left = -1.0f;
							break;
						case SDLK_w:
							if (move_forward == 0.0f) move_forward = 1.0f;
							break;
						case SDLK_s:
							if (move_forward == 0.0f) move_forward = -1.0f;
							break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym) {
						case SDLK_a:
							if (move_left == 1.0f) move_left = 0.0f;
							break;
						case SDLK_d:
							if (move_left == -1.0f) move_left = 0.0f;
							break;
						case SDLK_w:
							if (move_forward == 1.0f) move_forward = 0.0f;
							break;
						case SDLK_s:
							if (move_forward == -1.0f) move_forward = 0.0f;
							break;
					}
					break;
				case SDL_MOUSEMOTION:
					mainCam.rotate({event.motion.xrel / 120.0f, -event.motion.yrel / 120.0f});
					break;
			}
		}
		glm::vec3 displacement = (left * move_left + forward * move_forward) * PLAYER_SPEED * delta; 
		auto camOrgPos = mainCam.getPosition();
		glm::vec3 testCamPos = camOrgPos + displacement ;
		if (maze->isPassage(testCamPos) && maze->isReachable(camOrgPos, testCamPos)) {
			mainCam.translate(displacement * 0.98f);
		}



		glm::vec3 camPos = mainCam.getPosition();

		glm::mat4 mvp = mainCam.getMatrix() * glm::translate(glm::mat4(1.0f), {camPos.x, 0.0f, camPos.z});

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		standardShader->run();
		glUniform3f(lightSource, 0.0f, 50.0f, 0.0f); 
		glUniform3f(lightColor, 0.9f, 0.8f, 0.9f);
		glUniform3f(eyePos, camPos.x, camPos.y, camPos.z);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

		
		floor.draw(); 
		maze->drawWalls(mainCam.getMatrix(), MatrixID, lightColor);

		glUniform3f(lightColor, enemy_col.x, enemy_col.y, enemy_col.z);
		for (auto& e : enemies) {
			e.update();
			glm::mat4 mvp1 = mainCam.getMatrix() * glm::translate(glm::mat4(1.0f), e.getPosition() + glm::vec3(0.0f, 2.0f, 0.0f));
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp1[0][0]);
			enemy.draw();
		}

		SDL_GL_SwapWindow(window);
		
		for (auto& e : enemies) {
			auto a = e.getPosition();
			a.y = 0.0;
			camPos.y = 0.0;

			auto diff = (a - camPos);
			float l = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
			if (l < 4.0f) {
				printf("You lost!\n");
				alive = false;
				break;
			}
		}
		if (maze->atDest(camPos.x, camPos.z)) {
			std::printf("Congrat, you won!\n");
			break;
		}
	}
	
}
