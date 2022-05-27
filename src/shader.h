#ifndef SHADER_H_
#define SHADER_H_

#include <GL/glew.h>

#include <iostream>
#include <chrono>
#include <string>
#include <fstream>

#include "utils.h"

class Shader {
private:
	GLuint m_shaderId;

public:
	Shader(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName);
	virtual ~Shader();

	void bind();
	void unbind();

	// Call to compile a new shader and use that one
	void update(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName);

	GLuint getShaderId() const noexcept { return m_shaderId; }

private:
	GLuint compile(const std::string& shaderSource, GLenum type);
	std::string parse(const std::string& filename);
	GLuint createShader(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName);
};

#endif