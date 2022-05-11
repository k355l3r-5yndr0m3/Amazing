#include "Mesh3D.hpp" 

#include "stb_image.h" 

#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/scene.h> 
#include <assimp/postprocess.h>

Mesh3D::Mesh3D() {

}

Mesh3D::~Mesh3D() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

int Mesh3D::loadFromFile(const char* filename, const char* texture) {
	if (texture != nullptr) {
		int w, h, c;
		unsigned char* data = stbi_load(texture, &w, &h, &c, 0);
		
		if (data == nullptr) {
			TEX = 0;
		} else { 
			glGenTextures(1, &TEX);
		
			glBindTexture(GL_TEXTURE_2D, TEX);

			GLenum format;
			if (c == 3) {
				format = GL_RGB;
			} else if (c == 4) {
				format = GL_RGBA;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data); 
		}
	} else {
		TEX = 0;
	}



	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	const aiScene* scene = aiImportFile(filename, aiProcess_Triangulate | aiProcess_GenNormals);
	if (scene == nullptr) 
		return -1;

	const aiMesh* mesh = scene->mMeshes[0];
	vertexCount = mesh->mNumVertices;
	
	Vertex* const vertices = new Vertex[vertexCount];

	for (unsigned int i = 0; i < vertexCount; i++) {
		vertices[i] = {
			.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z},
			.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z},
			.color = {1.0f, 1.0f, 1.0f}, // TODO: Implement this 
			.texture = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y} // TODO: Implement this 
		};
		
	}
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)); 

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

	glBindVertexArray(0);


	delete[] vertices;
	aiReleaseImport(scene);
	return 0;
}

void Mesh3D::draw() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
