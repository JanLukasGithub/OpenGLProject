/*
 * main.cpp
 *
 *  Created on: Jul 21, 2021
 *      Author: jan
 */

// Standard library
#include <filesystem>
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>

// Glew
#define GLEW_STATIC
#include <GL/glew.h>

// STB Image
#define STB_IMAGE_IMPLEMENTATION
// Ignore warnings from this file
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include "../lib/stb_image.h"
#pragma GCC diagnostic pop
#undef STB_IMAGE_IMPLEMENTATION

// Glm
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wsign-conversion"

#include "../lib/glm/glm.hpp"
#include "../lib/glm/ext/matrix_transform.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"

#pragma GCC diagnostic pop

// SDL
#define SDL_MAIN_HANDLED
#ifdef _WIN32
#include <SDL.h>
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#else
#include <SDL2/SDL.h>
#endif

// Self written headers
#include "utils.h"

#include "lights/Lights.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "cameras/FlyingCamera.h"
#include "SdlEventHandler.h"
#include "models/Model.h"
#include "font.h"

// These functions are at the beginning of the file, so no forward declaration is needed and the corresponding warning can be ignored
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wunused-parameter"

const char* getExtensionFromModelName(std::string filenameNoExtension) {
	if (std::filesystem::exists(filenameNoExtension + ".bmf")) {
		return ".bmf";
	} else if (std::filesystem::exists(filenameNoExtension + ".obj")) {
		return ".obj";
	} else if (std::filesystem::exists(filenameNoExtension + ".fbx")) {
		return ".fbx";
	} else if (std::filesystem::exists(filenameNoExtension + ".FBX")) {
		return ".FBX";
	}
	return "";
}

void openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
	std::cout << "[OpenGL]: " << message << std::endl;
}

int main(int argc, char **argv) {
	// Init SDL, the window, OpenGL and GLEW
	SDL_Window *window;
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// 0 should mean no VSync, 1 means VSync
	SDL_GL_SetSwapInterval(1);

	// Change mouse mode to relative
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// Give debug output if needed
#ifdef _DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	//uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP;
	uint32 flags = SDL_WINDOW_OPENGL;

	window = SDL_CreateWindow("C++ OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, flags);
	// Can be stored, but is currently not used for anything
//	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	SDL_GL_CreateContext(window);

	// Return errors
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		std::cin.get();
		return -1;
	}

	int *windowWidth = nullptr, *windowHeight = nullptr;

	SDL_GetWindowSize(window, windowWidth, windowHeight);

	debugOutput("OpenGL Version: "); debugOutputEndl(glGetString(GL_VERSION));

	// Give debug output if wanted
#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openGLDebugCallback, 0);
#endif

	// Load the font shaders
	Shader* fontShader = new Shader("src/shaders/font.vs", "src/shaders/font.fs");

	// Load the shaders
	Shader *shader = new Shader("src/shaders/basic.vs", "src/shaders/basic.fs");
	shader->bind();

	DirectionalLight sun {
	// Direction uniform location
	        glGetUniformLocation(shader->getShaderId(), "u_directionalLight.direction"),
	        // Direction
	        glm::vec3(-1.0f),
	        // Color
	        glm::vec3(0.8f, 0.8f, 0.8f),
	        // Ambient color
	        glm::vec3(0.16f, 0.16f, 0.16f) };
	// Give uniforms the colors
	glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_directionalLight.diffuse"), 1, (float*) &sun.color.r);
	glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_directionalLight.specular"), 1, (float*) &sun.color.r);
	glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_directionalLight.ambient"), 1, (float*) &sun.ambientColor.r);

	PointLight pointLight {
	// Position uniform location
	        glGetUniformLocation(shader->getShaderId(), "u_pointLight.position"),
	        // Position
	        glm::vec4(0.0f, 0.0f, 10.0f, 1.0f),
	        // Color
	        glm::vec3(0.2f, 0.2f, 1.0f),
	        // Ambient color
	        glm::vec3(0.04f, 0.04f, 0.2f),
	        // Linear attenuation
	        0.027f,
	        // Quadratic attenuation
	        0.0026f };
	// Give uniforms the colors
	glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_pointLight.diffuse"), 1, (float*) &pointLight.color.r);
	glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_pointLight.specular"), 1, (float*) &pointLight.color.r);
	glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_pointLight.ambient"), 1, (float*) &pointLight.ambientColor.r);
	// Give uniforms the linear and quadratic values
	glUniform1f(glGetUniformLocation(shader->getShaderId(), "u_pointLight.linear"), pointLight.linear);
	glUniform1f(glGetUniformLocation(shader->getShaderId(), "u_pointLight.quadratic"), pointLight.quadratic);

	SpotLight flashlight {
	// Position uniform location
	        glGetUniformLocation(shader->getShaderId(), "u_spotLight.position"),
	        // Direction uniform location
	        glGetUniformLocation(shader->getShaderId(), "u_spotLight.direction"),
	        // Position
	        glm::vec3(0.0f, 0.0f, 0.0f),
	        // Direction
	        glm::vec3(0.0f, 0.0f, 1.0f),
	        // Color
	        glm::vec3(1.0f, 1.0f, 1.0f),
	        // Ambient color
	        glm::vec3(0.2f, 0.2f, 0.2f),
	        // Linear attenuation
	        0.027f,
	        // Quadratic attenuation
	        0.0026f,
	        // Inner cone
	        1.0f,
	        // Outer cone
	        0.9f };

	// This spot light behaves like a flash light, it's always at the same point relative to the camera, so we don't need to update its position and direction
	glUniform3fv(flashlight.positionUniformLocation, 1, (float*) &flashlight.position.x);
	glUniform3fv(flashlight.directionUniformLocation, 1, (float*) &flashlight.direction.x);

	// Give uniforms the colors
	glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_spotLight.diffuse"), 1, (float*) &flashlight.color.r);
	glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_spotLight.specular"), 1, (float*) &flashlight.color.r);
	glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_spotLight.ambient"), 1, (float*) &flashlight.ambientColor.r);

	// Give uniforms the attenuation
	glUniform1f(glGetUniformLocation(shader->getShaderId(), "u_spotLight.linear"), flashlight.linear);
	glUniform1f(glGetUniformLocation(shader->getShaderId(), "u_spotLight.quadratic"), flashlight.quadratic);

	// Give uniforms the inner and outer cone
	glUniform1f(glGetUniformLocation(shader->getShaderId(), "u_spotLight.innerCone"), flashlight.innerCone);
	glUniform1f(glGetUniformLocation(shader->getShaderId(), "u_spotLight.outerCone"), flashlight.outerCone);

	// Create the models vector
	std::vector<Model*> models;

	// Load the desired model
	std::cout << "Please input the name of the model you want to load" << std::endl;

	std::string modelname = std::string();
	std::cin >> modelname;

	std::string filenameNoExtension = "assets/models/" + modelname + "/" + modelname;
	const char* extension = getExtensionFromModelName(filenameNoExtension);

	models.push_back(new Model((filenameNoExtension + extension).c_str(), shader));

	Font font;
	font.initFont("assets/fonts/OpenSans-Regular.ttf");

	// FPS count stuff
	uint64 perfCounterFrequency = SDL_GetPerformanceFrequency();
	uint64 lastCounter = SDL_GetPerformanceCounter();
	float32 delta = 0.0f;

	// Model matrix
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.2f));

	// Camera object
	FlyingCamera camera(90.0f, 800, 600);
	camera.translate(glm::vec3(0.0f, 0.0f, 5.0f));
	camera.update();

	glm::mat4 modelViewProj = camera.getViewProjection() * modelMatrix;

	int modelViewProjUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_modelViewProj");
	int modelViewUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_modelView");
	int invModelViewUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_invModelView");

	// Wireframe mode for debugging
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

// Input and Event handling
	SdlEventHandler *handler = new SdlEventHandler();

	bool isEscMenuOpen = false;

	bool close = false;
	while (!close) {
		// Update the input handler
		handler->update();

		// Handle events
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			// Handle quit
			if (event.type == SDL_QUIT) {
				close = true;
			}
			// Handle input
			else {
				handler->handleSdlEvent(event, delta);
			}
		}

		if (handler->keyJustPressed(SDLK_r)) {
			fontShader->update("src/shaders/font.vs", "src/shaders/font.fs");

			// Update shader
			shader->update("src/shaders/basic.vs", "src/shaders/basic.fs");

			// Update uniform locations
			sun.directionUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_directionalLight.direction");
			pointLight.positionUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_pointLight.position");
			flashlight.positionUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_spotLight.position");
			flashlight.directionUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_spotLight.direction");
			modelViewProjUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_modelViewProj");
			modelViewUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_modelView");
			invModelViewUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_invModelView");

			// Set values of "constant" uniforms

			// Give uniforms the colors
			glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_directionalLight.diffuse"), 1, (float*) &sun.color.r);
			glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_directionalLight.specular"), 1, (float*) &sun.color.r);
			glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_directionalLight.ambient"), 1, (float*) &sun.ambientColor.r);

			// Give uniforms the colors
			glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_pointLight.diffuse"), 1, (float*) &pointLight.color.r);
			glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_pointLight.specular"), 1, (float*) &pointLight.color.r);
			glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_pointLight.ambient"), 1, (float*) &pointLight.ambientColor.r);
			// Give uniforms the linear and quadratic values
			glUniform1f(glGetUniformLocation(shader->getShaderId(), "u_pointLight.linear"), pointLight.linear);
			glUniform1f(glGetUniformLocation(shader->getShaderId(), "u_pointLight.quadratic"), pointLight.quadratic);

			// This spot light behaves like a flash light, it's always at the same point relative to the camera, so we don't need to update its position and direction
			glUniform3fv(flashlight.positionUniformLocation, 1, (float*) &flashlight.position.x);
			glUniform3fv(flashlight.directionUniformLocation, 1, (float*) &flashlight.direction.x);
			// Give uniforms the colors
			glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_spotLight.diffuse"), 1, (float*) &flashlight.color.r);
			glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_spotLight.specular"), 1, (float*) &flashlight.color.r);
			glUniform3fv(glGetUniformLocation(shader->getShaderId(), "u_spotLight.ambient"), 1, (float*) &flashlight.ambientColor.r);
			// Give uniforms the attenuation
			glUniform1f(glGetUniformLocation(shader->getShaderId(), "u_spotLight.linear"), flashlight.linear);
			glUniform1f(glGetUniformLocation(shader->getShaderId(), "u_spotLight.quadratic"), flashlight.quadratic);
			// Give uniforms the inner and outer cone
			glUniform1f(glGetUniformLocation(shader->getShaderId(), "u_spotLight.innerCone"), flashlight.innerCone);
			glUniform1f(glGetUniformLocation(shader->getShaderId(), "u_spotLight.outerCone"), flashlight.outerCone);
		}

		if (handler->keyJustPressed(SDLK_ESCAPE)) {
			// Change mouse mode to relative when exiting EscMenu and normal when entering it
			SDL_SetRelativeMouseMode(isEscMenuOpen ? SDL_TRUE : SDL_FALSE);
			isEscMenuOpen = !isEscMenuOpen;
		}

		if (!isEscMenuOpen)
			// Actually does things with the inputs
			camera.handleInputs(handler, delta);

		// Clear existing things
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Enable face culling
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		if (!isEscMenuOpen);
			// Rotate model
//			modelMatrix = glm::rotate(modelMatrix, 1.0f * delta, glm::vec3(0, 1, 0));

		// View things
		modelViewProj = camera.getViewProjection() * modelMatrix;
		glm::mat4 modelView = camera.getView() * modelMatrix;
		glm::mat4 invModelView = glm::transpose(glm::inverse(modelView));

		// Transform sun direction and change the uniform value
		glm::vec4 transformedSunDirection = glm::transpose(glm::inverse(camera.getView())) * glm::vec4(sun.direction, 1.0f);
		glUniform3fv(sun.directionUniformLocation, 1, (float*) &transformedSunDirection.x);

		// Rotate point light and change the uniform value
		glm::mat4 pointLightMatrix = glm::rotate(glm::mat4(1.0f), delta, { 0.0f, 1.0f, 0.0f });
		pointLight.position = pointLight.position * pointLightMatrix;
		glm::vec3 transformedPointLightPosition = (glm::vec3) (camera.getView() * pointLight.position);
		glUniform3fv(pointLight.positionUniformLocation, 1, (float*) &transformedPointLightPosition.x);

		// Change the modelUniform value
		glUniformMatrix4fv(modelViewProjUniformLocation, 1, GL_FALSE, &modelViewProj[0][0]);
		glUniformMatrix4fv(modelViewUniformLocation, 1, GL_FALSE, &modelView[0][0]);
		glUniformMatrix4fv(invModelViewUniformLocation, 1, GL_FALSE, &invModelView[0][0]);

		// Render
		shader->bind();
		for (unsigned int i = 0; i < models.size(); i++) {
			models[i]->render();
		}
		shader->unbind();

		fontShader->bind();
		
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		glm::mat4 ortho = glm::ortho(0.0f, (float)w, (float)h, 0.0f);
		glUniformMatrix4fv(glGetUniformLocation(fontShader->getShaderId(), "u_modelViewProj"), 1, GL_FALSE, &ortho[0][0]);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		font.drawString(20.0f, 20.0f, "Ganymede", fontShader);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		fontShader->unbind();

		// Display things you just made and use other buffer for the next frame
		SDL_GL_SwapWindow(window);

		// More FPS stuff
		uint64 endCounter = SDL_GetPerformanceCounter();
		uint64 counterElapsed = endCounter - lastCounter;
		delta = ((float32) counterElapsed) / ((float32) perfCounterFrequency);

		// This variable has no use when using Release Build Configuration, disable the corresponding warning
		uint32 FPS = (uint32) ((float32) perfCounterFrequency / (float32) counterElapsed);
		debugOutputEndl(FPS);
		lastCounter = endCounter;
	}

	// OS should do this but bad OSes like Windows exist
	delete handler;
	delete shader;
	delete fontShader;

	for (unsigned int i = 0; i < models.size(); i++) {
		delete models[i];
	}

	return 0;

}

#pragma GCC diagnostic pop // Disable unused parameter and no function forward declaration warnings
