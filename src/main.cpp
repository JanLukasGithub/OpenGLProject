#include <iostream>
#include <vector>
#include <string>

// Glew
#define GLEW_STATIC
#include <GL/glew.h>

// STB Image
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION

// Glm
#include "../lib/glm/glm.hpp"

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

#include "utils.h"
#include "sdlEventHandler.h"
#include "models/model.h"
#include "models/modelInstance.h"
#include "font/font.h"
#include "renderer/renderer.h"

int numModels = 0;

/**
 * @brief asks the user for a model to load in std input and loads the model
 *
 * @return false if user input was "none", "n" or "", true otherwise
 */
bool userModelLoad() {
	std::string* modelname = new std::string();
	// Ask the user what model he wants to load
	std::cout << "Please input the filename of the model you want to load or \"none\", \"n\" or \"\" to abort" << std::endl;
	std::cin >> *modelname;

	// If the user wants to abort do that
	if (modelname->compare("none") * modelname->compare("n") * modelname->compare("") == 0)
		return false;

	// Load model
	try {
		Model::addModelFile(modelname->c_str()).addModel(new ModelInstance(glm::vec3((numModels - 1) * 5.0f, 0.0f, 0.0f)));
		numModels++;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	// User didn't abort
	return true;
}

int main(int argc, char** argv) {
	// Renderer
	Renderer* renderer = new Renderer();

	// Always load the floor
	Model::addModelFile("assets/models/Floor/Floor.obj").addModel(new ModelInstance(glm::vec3(0.0f, -1.0f, 0.0f)));
	numModels++;

	if (argc < 2) {
		// Load models
		while (userModelLoad());
	} else {
		// Load models
		for (int i = 1; i < argc; i++) {
			try {
				Model::addModelFile(argv[i]).addModel(new ModelInstance(glm::vec3((numModels - 1) * 5.0f, 0.0f, 0.0f)));
				numModels++;
			}
			catch (const std::exception& e) {
				std::cout << "Error occurred while loading model " << argv[i] << "!" << std::endl;
				std::cout << e.what() << std::endl;
			}
		}
	}

	// Load font
	Font* font = renderer->getFontRenderer();

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

		// Pressing R resets the renderer, reloading the shaders
		if (handler->keyJustPressed(SDLK_r)) {
			renderer->reset();
		}

		if (handler->keyPressed(SDLK_l)) {
			Model::addModelFile("assets/models/QuadrupedTank/QuadrupedTank.obj").addModel(new ModelInstance(renderer->getCamera().getPosition()));
			numModels++;
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
		for (int i = 0; i < Model::getListSize(); i++) {
			Model::getFromList(i).renderModels();
		}

		// Start rendering font
		renderer->setupFontRender();

		// Draw FPS to the screen
		font->drawString(5.0f, 20.0f, std::to_string(renderer->getFPS()).c_str());
		font->drawString(5.0f, 40.0f, std::to_string(numModels).c_str());

		// Start rendering 2D stuff
		renderer->setup2DRender();

		// End the frame
		renderer->endFrame();
	}

	// Delete dynamically allocated memory (OS would do this as well)
	delete renderer;
	delete handler;

	return 0;
}
