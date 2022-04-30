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
#include "models/model.h"
#include "models/modelUtils.h"
#include "font/font.h"
#include "renderer/renderer.h"
#include "terrain/terrain.h"

int numModels = 0;

// Loads the model from the specified filename. Handles exceptions by telling the user in std::cerr
// TODO: Move to modelUtils
void loadModel(const char* const filename) noexcept {
	try {
		Model::addModelFile(filename).addInstance(glm::vec3(numModels * 5.0f, 0.0f, 0.0f));
		numModels++;
	}
	catch (const std::exception& e) {
		std::cerr << "Error occurred while loading model " << filename << "!" << std::endl;
		std::cerr << e.what() << std::endl;
	}
}

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

	loadModel(modelname->c_str());

	return true;
}

int main(int argc, char** argv) {
	Renderer* renderer = new Renderer();

	utils::loadModelsFrom("assets/models/");

	if (argc < 2) {
		while (cliModelLoad());
	} else {
		for (int i = 1; i < argc; i++) {
			loadModel(argv[i]);
		}
	}

	Font* font = renderer->getFontRenderer();

	SdlEventHandler* handler = new SdlEventHandler();

	Terrain* terrain;

	{	// Get rid of height map data right after initialization of terrain
		const int sizeX = 100, sizeZ = 100;

		short heightMap[sizeX * sizeZ]{ glm::detail::toFloat16(0.0f) };
		terrain = new Terrain(-50, -50, sizeX, sizeZ, heightMap);
	}

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
			Model::addModelFile("assets/models/QuadrupedTank/QuadrupedTank.obj").addInstance(renderer->getCamera().getPosition());
			numModels++;
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

		for (int i = 0; i < Model::getListSize(); i++) {
			Model::getFromList(i).renderModels();
		}

		renderer->setupTerrainRender();

		terrain->render();

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
