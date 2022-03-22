#include "Application.hpp" 

#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include "glad/glad.h"
#include "Mesh.hpp"

#include <SDL2/SDL.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cstdio>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

Application::Application() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw "SDL initialization failed"; 
	}
	if ((window = SDL_CreateWindow(applicationName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL)) == nullptr) {
		throw "Failed to create window";
	}
	glcontext = SDL_GL_CreateContext(window);
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	glViewport(0, 0, windowWidth, windowHeight);
	
	glClearColor(0.4f, 0.1f, 0.2f, 0.7f);
	glClearDepth(1.0);

	glEnable(GL_DEPTH_TEST);
	
	standardShader = new ShaderProgram("assets/shaders/wall_material_vertex.glsl", "assets/shaders/wall_material_fragment.glsl");
}

Application::~Application() {
	delete standardShader;

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);

}

void Application::mainLoop() {
	bool alive = true;

	Assimp::Importer meshImporter;
	const aiScene* mazeWall = meshImporter.ReadFile("assets/objects/MazeWall_unoptimized.obj", aiProcess_GenSmoothNormals | aiProcess_Triangulate);

	Mesh wallMaze(mazeWall->mMeshes[0]);

	Camera mainCam(windowWidth, windowHeight, {0.0f, 4.8f, 0.0f});

	glm::mat4 wallMazeMatrix = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});


	GLuint MatrixID = standardShader->uniformLocation("MVP");
	GLuint lightSource = standardShader->uniformLocation("lightSource");
	GLuint lightColor = standardShader->uniformLocation("lightColor");
	GLuint eyePos = standardShader->uniformLocation("eyePos");





	glm::vec3 velocity = {0.0f, 0.0f, 0.0f};
	glm::vec3 forward = mainCam.getFacingDirection();
	forward.y = 0.0f;
	
	glm::vec3 left = glm::cross({0.0f, 1.0f, 0.0f}, forward);
	float move_left = 0.0, move_forward = 0.0;
	while (alive) {
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
		mainCam.translate((left * move_left + forward * move_forward) * 0.1f);
		glm::mat4 mvp = mainCam.getMatrix() * wallMazeMatrix;




		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::vec3 camPos = mainCam.getPosition();

		standardShader->run();
		glUniform3f(lightSource, 4.0f, 10.0f, 4.0f);
		glUniform3f(lightColor, 0.9f, 0.8f, 0.9f);
		glUniform3f(eyePos, camPos.x, camPos.y, camPos.z);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
		
		wallMaze.draw();

		SDL_GL_SwapWindow(window);

	}

}
