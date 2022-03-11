#include "resource_loader.hpp" 

#include <fstream>
#include <string> 
#include <sstream>

GLuint create_shader_from_file(std::string file, GLenum type) {
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
		GLsizei msg_size;
		glGetShaderInfoLog(shader, 0, &msg_size, nullptr);
		GLchar* msg = new GLchar[msg_size + 1];
		glGetShaderInfoLog(shader, msg_size + 1, nullptr, msg);
		std::fprintf(stderr, "Failed to compile %s shader: %s\n", (type == GL_FRAGMENT_SHADER ? "fragment" : "vertex") , msg);
		delete[] msg;
		
		exit(-1);
	}

	shader_file.close();
	return shader;
}

GLuint create_shader_program(GLuint vertex_shader, GLuint fragment_shader) {
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	
	GLint result;
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

	return program;
}

