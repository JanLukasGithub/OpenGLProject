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
	Shader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	Shader(const std::string& vertexShaderFilename, const std::string& geometryShaderFilename, const std::string& fragmentShaderFilename);
	virtual ~Shader();

	void bind();
	void unbind();

	// Call to compile a new shader and use that one
	void update(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	void update(const std::string& vertexShaderFilename, const std::string& geometryShaderFilename, const std::string& fragmentShaderFilename);

	GLuint getShaderId() const noexcept { return m_shaderId; }

private:
	GLuint compile(const std::string& shaderSource, GLenum type);
	std::string parse(const std::string& filename);
	GLuint createShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	GLuint createShader(const std::string& vertexShaderFilename, const std::string& geometryShaderFilename, const std::string& fragmentShaderFilename);
};

#endif