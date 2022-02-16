#pragma once
#include "../gl.h"
#include <string>
#include <fstream>
#include <iosfwd>
#include <sstream>
#include <vector>
#include "../../DefaultShaders.h"

GLuint CompileDefaultShaders();
GLuint CompileShaders(std::string VertexShaderCode, std::string FragmentShaderCode);
GLuint LoadShaders(const char *vertex_filepath, const char *fragment_filepath);