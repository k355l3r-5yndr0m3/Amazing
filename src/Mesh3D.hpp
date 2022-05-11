#pragma once 

#include "glad/glad.h"

// #include "stb_image.h" 

struct Vertex {
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat color[3];
	GLfloat texture[2];
};

class Mesh3D {
	public:
		Mesh3D();
		~Mesh3D();

		int loadFromFile(const char* path, const char* texture = nullptr);
		void draw();
	private: 
		GLuint VAO, VBO, TEX;
		unsigned int vertexCount;


};
