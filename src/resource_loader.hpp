#pragma once 
#include <string> 

#include "glad/glad.h"

GLuint create_shader_from_file(std::string file, GLenum type);
GLuint create_shader_program(GLuint vertex_shader, GLuint fragment_shader);
inline GLuint create_shader_program_from_files(std::string vertex_glsl, std::string fragment_glsl) {
	GLuint vertex_shader = create_shader_from_file(vertex_glsl, GL_VERTEX_SHADER),
		   fragment_shader = create_shader_from_file(fragment_glsl, GL_FRAGMENT_SHADER);
	
	GLuint program  = create_shader_program(vertex_shader, fragment_shader);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return program;

}



