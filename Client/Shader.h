#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);


void SetShaderFloat(GLuint shader, std::string name, float value);
void SetShader3f(GLuint shader, std::string name, glm::vec3 value);

#endif