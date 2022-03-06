// Standard library
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

// Glew
#define GLEW_STATIC
#include <GL/glew.h>

// STB Image
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION

// Glm
#include "../lib/glm/glm.hpp"
#include "../lib/glm/ext/matrix_transform.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"

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
#include "font/font.h"
#include "timer/timer.h"
#include "renderer/OpenGL/OpenGLRenderer.h"
#include "gui/Gui.h"

/**
 * @brief asks the user for a model to load in std input and loads the model
 * 
 * @param modelList 
 * @param renderer 
 * @return true if user input wasn't "none"
 * @return false if user input was "none"
 */
bool userModelLoad(std::vector<Model*>& modelList, OpenGLRenderer* renderer) {
	std::string modelname = std::string();
	// Ask the user what model he wants to load
	std::cout << "Please input the filename of the model you want to load or \"none\", \"n\" or \"\" to abort" << std::endl;
	std::cin >> modelname;

	// If the user wants to abort do that
	if (modelname.compare("none") * modelname.compare("n") * modelname.compare("") == 0)
		return false;

	// Load model
	try {
		modelList.push_back(new Model(modelname.c_str(), renderer->getShader3d()));
	} catch(std::exception* e) {
		std::cout << e->what() << std::endl;
	} catch(std::exception e) {
		std::cout << e.what() << std::endl;
	}

	// User didn't abort
	return true;
}

int main(int argc, char** argv) {
	// Renderer
	OpenGLRenderer* renderer = new OpenGLRenderer();
	renderer->init();
	renderer->getCamera().translate(glm::vec3(0.0f, 1.0f, 0.0f));

	// Create the "models" vector
	std::vector<Model*> models;

	if (argc < 2) {
		// Load models
		while (userModelLoad(models, renderer));
	} else {
		// Use the first argument as model to load
		std::string modelname = std::string(argv[1]);

		// Load model
		models.push_back(new Model((modelname).c_str(), renderer->getShader3d()));
	}

	// Load font
	Font font;
	font.initFont("assets/fonts/OpenSans-Regular.ttf", renderer->getShaderFont());

	// Handle SDL events (keyboard, mouse, ...)
	SdlEventHandler* handler = new SdlEventHandler();

	bool isEscMenuOpen = false;
	bool running = true;

	while (running) {
		handler->update();

		// Handle events
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			// Handle quit
			if (event.type == SDL_QUIT) {
				running = false;
			}
			// Handle input
			else {
				handler->handleSdlEvent(event, renderer->getDelta());
			}
		}

		// Pressing R resets the renderer
		if (handler->keyJustPressed(SDLK_r)) {
			renderer->reset();
		}

		if (handler->keyJustPressed(SDLK_ESCAPE)) {
			// Change mouse mode to relative when exiting EscMenu and normal when entering it
			SDL_SetRelativeMouseMode(isEscMenuOpen ? SDL_TRUE : SDL_FALSE);
			isEscMenuOpen = !isEscMenuOpen;
		}

		if (!isEscMenuOpen) {
			// Actually does things with the inputs
			renderer->getCamera().handleInputs(handler, renderer->getDelta());
		}

		// Start the frame
		renderer->startFrame();
		// Start rendering 3d stuff
		renderer->setup3DRender();

		// Render models
		for (int i = 0; i < models.size(); i++) {
			models[i]->render();
		}

		// Start rendering font
		renderer->setupFontRender();

		// Draw FPS to the screen
		font.drawString(5.0f, 20.0f, std::to_string(renderer->getFPS()).c_str());

		// Start rendering 2D stuff
		renderer->setup2DRender();

		// End the frame
		renderer->endFrame();
	}

	// Delete dynamically allocated memory (OS would do this as well)
	delete renderer;
	delete handler;

	for (int i = 0; i < models.size(); i++) {
		delete models[i];
	}

	return 0;
}
