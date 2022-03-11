#pragma once 
#include "glad/glad.h"
#include <string>

struct Vertex {
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat color[3];
	GLfloat texture[2];
};


class Mesh {
	public:
		Mesh();
		~Mesh();

		int loadFromFile(std::string path);

	private:
		Vertex* vertices;
		GLuint* indices;

		GLuint vertexCount;
		GLuint indexCount;

		GLuint VAO, VBO, EBO;
		
};
