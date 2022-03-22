#pragma once 
#include "glad/glad.h"
#include <assimp/SceneCombiner.h>
#include <string>

#include <assimp/scene.h>

struct Vertex {
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat color[3];
	GLfloat texture[2];
};


class Mesh {
	public:
		Mesh(aiMesh* data, bool flipNormal = false);
		~Mesh();
		
		void draw();

	private:
		Vertex* vertices;
		GLuint* indices;

		GLuint vertexCount;
		GLuint indexCount;

		GLuint VAO, VBO, EBO;
		
};
