#include "Application.hpp"
#define MAZE_SIZE 16



#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib> 
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <string> 
#include <chrono>



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SOIL/SOIL.h>

#include <assimp/Importer.hpp> 
#include <assimp/scene.h> 
#include <assimp/postprocess.h>

#include "Camera.hpp"
#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ShaderProgram.hpp"
#include "Maze.hpp"

#define GAME_NAME "Game" 
#define PRIMARY_FONT_FILE "assets/fonts/RobotoMono-Regular.ttf"



int main() {
	Application app;
	app.mainLoop();

	return 0;
}
