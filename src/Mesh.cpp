#include "Mesh.hpp"
#include <cstdint>
#include <vector>

Mesh::Mesh(aiMesh* data, bool flipNormal) {
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	vertexCount = data->mNumVertices;
	std::vector<Vertex> vertexBufferData(vertexCount);

	for (uint32_t i = 0; i < vertexCount; i++) {
		if (flipNormal) {
			vertexBufferData[i] = {
				.position = {data->mVertices[i].x, data->mVertices[i].y, data->mVertices[i].z }, 
				.normal = {-data->mNormals[i].x, -data->mNormals[i].y, -data->mNormals[i].z },
				.color = {1.0f, 1.0f, 1.0f},
				.texture = {0, 0}
			};
		} else {
			vertexBufferData[i] = {
				.position = {data->mVertices[i].x, data->mVertices[i].y, data->mVertices[i].z }, 
				.normal = {data->mNormals[i].x, data->mNormals[i].y, data->mNormals[i].z },
				.color = {1.0f, 1.0f, 1.0f},
				.texture = {0, 0}
			};
		}
	}
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferData.size() * sizeof(vertexBufferData[0]), vertexBufferData.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)); 

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

	glBindVertexArray(0);
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::draw() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
