/*
 * Shader.h
 *
 *  Created on: Jul 21, 2021
 *      Author: jan
 */

#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <iostream>
#include <chrono>
#include "GL/glew.h"

#include "utils.h"

class Shader {
public:
	Shader(const char* vertexShaderFileName, const char* fragmentShaderFileName);
	virtual ~Shader();

	void bind();
	void unbind();

	// Call to compile a new shader and use that one
	void update(const char* vertexShaderFileName, const char* fragmentShaderFileName);

	GLuint getShaderId() {
	    return shaderId;
	}
private:
	GLuint compile(std::string shaderSource, GLenum type);
	std::string parse(const char* filename);
	GLuint createShader(const char* vertexShaderFileName, const char* fragmentShaderFileName);

	GLuint shaderId;
};

#endif /* SHADER_H_ */
