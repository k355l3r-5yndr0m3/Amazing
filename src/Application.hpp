#pragma once 

#include <SDL2/SDL_video.h>

#include "Maze.hpp"
#include "ShaderProgram.hpp"
#include "Camera.hpp"

class Application {
	public: 
		Application();
		~Application();

		void mainLoop();

	private:
		static constexpr char applicationName[] = "Procedurally generated maze";

		int windowWidth = 1000, windowHeight = 800;

		SDL_Window* window;
		SDL_GLContext glcontext;

		ShaderProgram* standardShader;
		PrimMaze<32>* maze;
};
