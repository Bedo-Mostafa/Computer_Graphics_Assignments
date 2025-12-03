#pragma once
#include <gl/glew/glew.h>
#include <string>

// Function declarations
char* readFile(const char* fileName);
bool compiledStatus(GLint shaderID);
GLuint makeShader(const char* ShaderSourceCode, GLenum SHADER_TYPE);
GLuint makeShaderProgram(GLuint vertShaderID, GLuint fragShaderID);
GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);
