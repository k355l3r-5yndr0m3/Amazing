#pragma once 

#include "glad/glad.h"

#include <string>

class ShaderProgram {
	public:
		ShaderProgram(std::string vertex_glsl, std::string fragment_glsl);
		~ShaderProgram();


		void run();

		inline GLuint uniformLocation(std::string name) {
			return glGetUniformLocation(this->program, name.c_str());
		}
	private:
		GLuint program;

		GLuint create_shader_from_file(std::string file, GLenum type);
		GLuint create_shader_program(GLuint vertex_shader, GLuint fragment_shader);

};
