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
#include "font/font.h"
#include "renderer/renderer.h"

/**
 * @brief asks the user for a model to load in std input and loads the model
 *
 * @param modelList
 * @return false if user input was "none", "n" or "", true otherwise
 */
bool userModelLoad(std::vector<Model*>& modelList) {
	std::string modelname = std::string();
	// Ask the user what model he wants to load
	std::cout << "Please input the filename of the model you want to load or \"none\", \"n\" or \"\" to abort" << std::endl;
	std::cin >> modelname;

	// If the user wants to abort do that
	if (modelname.compare("none") * modelname.compare("n") * modelname.compare("") == 0)
		return false;

	// Load model
	try {
		modelList.push_back(new Model(modelname.c_str(), glm::vec3((modelList.size() - 1) * 5.0f, 0.0f, 0.0f)));
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

	// Create the "models" vector
	std::vector<Model*>& models{ renderer->getModels() };

	// Always load the floor
	models.push_back(new Model("assets/models/Floor/Floor.obj", glm::vec3(0.0f, -1.0f, 0.0f)));

	if (argc < 2) {
		// Load models
		while (userModelLoad(models));
	} else {
		for (int i = 1; i < argc; i++) {
			// Use the first argument as model to load
			std::string modelname = std::string(argv[i]);

			// Load model
			try {
				models.push_back(new Model(modelname.c_str(), glm::vec3((models.size() - 1) * 5.0f, 0.0f, 0.0f)));
			}
			catch (const std::exception& e) {
				std::cout << "Error occurred while loading model " << modelname << "!" << std::endl;
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

		if (handler->keyJustPressed(SDLK_l)) {
			models.push_back(new Model("assets/models/QuadrupedTank/QuadrupedTank.obj", renderer->getCamera().getPosition()));
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
		font->drawString(5.0f, 20.0f, std::to_string(renderer->getFPS()).c_str());

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
