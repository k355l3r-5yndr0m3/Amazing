#pragma once 
#include "glad/glad.h"

#include <string>
GLuint create_shader_from_file(std::string file, GLenum type);
