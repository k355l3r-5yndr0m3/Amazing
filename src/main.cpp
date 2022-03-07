#include <cstdio>
#include <cstdlib> 
#include <string> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SOIL/SOIL.h>

#include "glad/glad.h"
#include "shaders.hpp"

#define GAME_NAME "Game" 
#define PRIMARY_FONT_FILE "assets/fonts/RobotoMono-Regular.ttf"


struct default_cube {
	static constexpr GLfloat vertices[] = {
    	-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Top-left
     	 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Top-right
     	 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom-right
    	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, // Bottom-left
	};

	static constexpr GLuint elements[] = {
		0, 1, 2,
		2, 3, 0,
	};

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3*sizeof(GLfloat))); 

		glBindVertexArray(0);
	}

	void destroy() {
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}

	void draw() {
		glBindVertexArray(VAO);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
};

inline void initialization() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		std::exit(-1);
	}
	
}



inline GLuint create_opengl_program() {
	GLuint vertex_shader = create_shader_from_file("assets/shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	GLuint fragment_shader = create_shader_from_file("assets/shaders/fragment_shader.glsl", GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	GLint result;
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		GLsizei msg_size;
		glGetProgramInfoLog(program, 0, &msg_size, nullptr);
		GLchar* msg = new GLchar[msg_size + 1];
		glGetProgramInfoLog(program, msg_size + 1, nullptr, msg);
		std::fprintf(stderr, "Failed to link program: %s\n", msg);

		delete[] msg;
		exit(-1);
	}
	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	
	return program;
}

int main() {
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

	GLuint opengl_program = create_opengl_program();
	glUseProgram(opengl_program);

	default_cube cube;
	cube.init();


	int w, h; 
	SDL_GetWindowSize(window, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.4f, 0.1f, 0.2f, 0.7f);

	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.0f, 100.0f);
	glm::mat4 View = glm::lookAt(glm::vec3(4, 4, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glm::mat4 Model = glm::mat4(1.0f);

	glm::mat4 mvp = Projection * View * Model;
	// glm::mat4 mvp(1.0f);
	GLuint MatrixID = glGetUniformLocation(opengl_program, "MVP");


	bool alive = 1;
	while (alive) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					alive = false;
					break;
			}
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
		cube.draw();
		SDL_GL_SwapWindow(window);

	}

	cube.destroy();
	glDeleteProgram(opengl_program);


	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
