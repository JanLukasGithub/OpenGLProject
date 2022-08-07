#include <iostream>
#include <vector>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION

#include "../lib/glm/glm.hpp"

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
#include "models/modelManager.h"
#include "models/modelUtils.h"
#include "font/font.h"
#include "renderer/renderer.h"
#include "terrain/terrain.h"

int numModels = 0;

/**
 * @brief asks the user for a model to load in std::cin and loads the model
 *
 * @return false iff user input was "none", "n" or "", true otherwise
 */
bool cliModelLoad() {
	std::string* modelname = new std::string();
	std::cout << "Please input the filename of the model you want to load or \"none\", \"n\" or \"\" to abort" << std::endl;
	std::cin >> *modelname;

	if (modelname->compare("none") * modelname->compare("n") * modelname->compare("") == 0)
		return false;

	numModels += utils::loadModelInstance(modelname->c_str(), glm::vec3(5.0f * numModels, 0.0f, 0.0f));

	return true;
}

int main(int argc, char** argv) {
	Renderer* renderer = new Renderer();

	utils::loadModelsFrom("assets/models/");

	if (argc < 2) {
		while (cliModelLoad());
	} else {
		for (int i = 1; i < argc; i++) {
			numModels += utils::loadModelInstance(argv[i], glm::vec3(5.0f * numModels, 0.0f, 0.0f));
		}
	}

	Font* font = renderer->getFontRenderer();

	SdlEventHandler* handler = new SdlEventHandler();

	Terrain terrain{0, 0, "assets/heightMaps/heightmapCologne.png"};

	bool isEscMenuOpen = false;
	bool running = true;

	while (running) {
		handler->update();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			} else {
				handler->handleSdlEvent(event, renderer->getDelta());
			}
		}

		if (handler->keyJustPressed(SDLK_r)) {
			renderer->reset();
		}

		if (handler->keyPressed(SDLK_l)) {
			numModels += utils::loadModelInstance("assets/models/QuadrupedTank/QuadrupedTank.obj", renderer->getCamera().getPosition());
		}

		if (handler->keyJustPressed(SDLK_ESCAPE)) {
			SDL_SetRelativeMouseMode(isEscMenuOpen ? SDL_TRUE : SDL_FALSE);
			isEscMenuOpen = !isEscMenuOpen;
		}

		if (!isEscMenuOpen) {
			renderer->getCamera().handleInputs(handler, renderer->getDelta());
		}

		renderer->startFrame();
		renderer->setup3DRender();

		for (int i = 0; i < Model_Manager::get_num_models(); i++) {
			Model_Manager::get_from_index(i).renderModels();
		}

		renderer->setupTerrainRender();

		terrain.render();

		renderer->setupFontRender();

		// Draw FPS and numModels
		font->drawString(5.0f, 20.0f, std::to_string(renderer->getFPS()).c_str());
		font->drawString(5.0f, 40.0f, std::to_string(numModels).c_str());

		renderer->setup2DRender();

		renderer->endFrame();
	}

	delete renderer;
	delete handler;

	return 0;
}
