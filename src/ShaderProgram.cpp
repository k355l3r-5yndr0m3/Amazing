#include "ShaderProgram.hpp"

#include <fstream>
#include <sstream>

ShaderProgram::ShaderProgram(std::string vertex_glsl, std::string fragment_glsl) {
	GLuint vertex_shader = this->create_shader_from_file(vertex_glsl, GL_VERTEX_SHADER);
	GLuint fragment_shader = this->create_shader_from_file(fragment_glsl, GL_FRAGMENT_SHADER);

	this->program = this->create_shader_program(vertex_shader, fragment_shader);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(this->program);
}

void ShaderProgram::run() {
	glUseProgram(this->program);
}

GLuint ShaderProgram::create_shader_from_file(std::string file, GLenum type) {
	std::ifstream shader_file(file);
	std::string code;
	std::ostringstream ss;
	ss << shader_file.rdbuf();
	code = ss.str();
	const GLchar* const code_ptr = code.c_str();
	
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &code_ptr, nullptr);
	glCompileShader(shader);

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		char buffer[2024];
		glGetShaderInfoLog(shader, sizeof(buffer), nullptr, buffer);
		std::fprintf(stderr, "Failed to compile %s shader %s: %s\n", file.c_str(), (type == GL_FRAGMENT_SHADER ? "fragment" : "vertex") , buffer);	
		exit(-1);
	}

	shader_file.close();
	return shader;
}

GLuint ShaderProgram::create_shader_program(GLuint vertex_shader, GLuint fragment_shader) {
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	
	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		char buffer[2024];
		glGetProgramInfoLog(program, sizeof(buffer), nullptr, buffer);
		std::fprintf(stderr, "Failed to link program: %s\n", buffer);

		exit(-1);
	}
	
	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);

	return program;
}

