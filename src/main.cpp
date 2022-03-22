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

#include "resource_loader.hpp"
#include "Mesh.hpp"
#include "ShaderProgram.hpp"
#include "Maze.hpp"

#define GAME_NAME "Game" 
#define PRIMARY_FONT_FILE "assets/fonts/RobotoMono-Regular.ttf"


struct default_cube {
/*
	static constexpr Vertex vertices[] = {
		{ -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		{  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		{ -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		{  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		{ -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		{ -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		{  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		{  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		{  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
	};
 *
 *
 *
 *
 *
 * 	static constexpr Vertex vertices[] = {
		{-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
    	{ 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f}, 
    	{ 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f}, 
    	{ 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f}, 
    	{-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f}, 
    	{-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f}, 

    	{-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f },
    	{ 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f },
    	{ 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f },
    	{ 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f },
    	{-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f },
    	{-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f },

    	{-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f},
    	{-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
    	{-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
    	{-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
    	{-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f},
    	{-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f},

    	{ 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f},
    	{ 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
    	{ 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
    	{ 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
    	{ 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f},
    	{ 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f},

    	{-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},
    	{ 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},
    	{ 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
    	{ 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
    	{-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
    	{-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},

    	{-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f},
    	{ 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f},
    	{ 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
    	{ 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
    	{-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
    	{-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f}
	};

*/

	static constexpr Vertex vertices[] = {
		{-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f	},
    	{ 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f	}, 
    	{ 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f	}, 
    	{ 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f	}, 
    	{-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f	}, 
    	{-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f	}, 
    	{-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f	},
    	{ 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f	},
    	{ 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f	},
    	{ 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f	},
    	{-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f	},
    	{-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f	},
    	{-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f	},
    	{-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f	},
    	{-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f	},
    	{-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f	},
    	{-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f	},
    	{-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f	},
    	{ 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f	},
    	{ 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f	},
    	{ 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f	},
    	{ 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f	},
    	{ 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f	},
    	{ 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f	},
    	{-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f	},
    	{ 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f	},
    	{ 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f	},
    	{ 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f	},
    	{-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f	},
    	{-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f	},
    	{-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f	},
    	{ 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f	},
    	{ 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f	},
    	{ 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f	},
    	{-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f	},
    	{-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f	}
	};



	static constexpr GLuint elements[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint texture;

	void init() {
		glGenBuffers(1, &EBO);
		glGenBuffers(1, &VBO);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)); 

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

		glBindVertexArray(0);

		int texture_width, texture_height, texture_nrChannels;
		unsigned char *data = stbi_load("assets/textures/container.jpg", &texture_width, &texture_height, &texture_nrChannels, 0);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

	}

	void destroy() {
		glDeleteTextures(1, &texture);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}

	void draw() {
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		// glDrawElements(GL_TRIANGLES, sizeof(elements) / sizeof(elements[0]) , GL_UNSIGNED_INT, 0);

	}
};

inline void initialization() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		std::exit(-1);
	}
	
}




int notmain() { 
	PrimMaze<MAZE_SIZE> testMaze(3451341);
	testMaze.printMaze();

	


	initialization();
	
	SDL_Window* window;
	if (!(window = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL))) {
		std::fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
		return -1;
	}
	

	SDL_GLContext gl_context;
	if (!(gl_context = SDL_GL_CreateContext(window))) {
		std::fprintf(stderr, "Failed to create OpenGL context: %s\n", SDL_GetError());
		return -1;
	}

	gladLoadGLLoader(SDL_GL_GetProcAddress);

	int w, h; 
	SDL_GetWindowSize(window, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.4f, 0.1f, 0.2f, 0.7f);
	glClearDepth(1.0);






	Assimp::Importer importer;

	const aiScene* utah = importer.ReadFile("assets/objects/Wall.obj", aiProcess_Triangulate | aiProcess_GenSmoothNormals);

	Mesh teacup(utah->mMeshes[0], true);


	const aiScene* sphere = importer.ReadFile("assets/objects/sphere.obj", aiProcess_GenSmoothNormals | aiProcess_Triangulate);
	Mesh ball(sphere->mMeshes[0]);

	ShaderProgram wall_material("assets/shaders/wall_material_vertex.glsl", "assets/shaders/wall_material_fragment.glsl");

	/*
	default_cube cube;
	cube.init();
	*/


	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 Scale = glm::scale(Model, {1.0f, 1.0f, 1.0f});
	glm::mat4 Position_0 = glm::translate(Scale, glm::vec3(0.0f, 0.0f,-1.0f));

	glm::mat4 Position_1 = glm::scale(glm::translate(glm::mat4(1.0f), {1.0f, 4.0f, 1.0f}), {1.0f, 1.0f, 1.0f});

	Camera cam(w, h, {4.0f, 2.0f, 0.0f}, {M_PI, 0.0});

	glm::mat4 mvp0 = cam.getMatrix() * Position_0;
	glm::mat4 mvp1 = cam.getMatrix() * Position_1;
	GLuint MatrixID_wall = wall_material.uniformLocation("MVP");


	// Texture 
	glEnable(GL_DEPTH_TEST);

	GLuint lightSource = wall_material.uniformLocation("lightSource");
	GLuint lightColor = wall_material.uniformLocation("lightColor");
	GLuint eyePos = wall_material.uniformLocation("eyePos");

	bool alive = 1;
	glm::vec3 velocity = {0.0f, 0.0f, 0.0f};

	glm::vec3 forward = cam.getFacingDirection();
	forward.y = 0.0f;
	
	glm::vec3 left = glm::cross({0.0f, 1.0f, 0.0f}, forward);
	
	float move_forward = 0.0f;
	float move_left = 0.0f;
	while (alive) {
		forward = cam.getFacingDirection();
		forward.y = 0.0f;
		forward = glm::normalize(forward);
		left = glm::cross({0.0f, 1.0f, 0.0f}, forward);

		SDL_Event event;
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
					cam.rotate({-event.motion.xrel / 120.0f, event.motion.yrel / 120.0f});
					break;
			}
		}
		cam.translate((left * move_left + forward * move_forward) * 0.1f);
		mvp1 = cam.getMatrix() * Position_1;
		mvp0 = cam.getMatrix() * Position_0;

		auto camPos = cam.getPosition();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		wall_material.run();
		glUniform3f(lightSource, 0.0f, 6.0f, 0.0f);
		glUniform3f(lightColor, 0.9f, 0.8f, 0.9f);
		glUniform3f(eyePos, camPos.x, camPos.y, camPos.z);
		glUniformMatrix4fv(MatrixID_wall, 1, GL_FALSE, &mvp0[0][0]);
		teacup.draw();
		// glBindTexture(GL_TEXTURE_2D, cube.texture);
	//	cube.draw();
		glUniformMatrix4fv(MatrixID_wall, 1, GL_FALSE, &mvp1[0][0]);
		ball.draw();


		SDL_GL_SwapWindow(window);
	}

	// cube.destroy();
	

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}

int main() {
	Application app;
	app.mainLoop();

	return 0;
}
