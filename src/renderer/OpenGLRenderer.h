#ifndef OPEN_GL_RENDERER_H_
#define OPEN_GL_RENDERER_H_

// Glew
#include <GL/glew.h>

// SDL
#ifdef _WIN32
#include <SDL.h>
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#else
#include <SDL2/SDL.h>
#endif

#include <chrono>

// Glm
#include "../../lib/glm/glm.hpp"
#include "../../lib/glm/ext/matrix_transform.hpp"
#include "../../lib/glm/gtc/matrix_transform.hpp"

#include "../Shader.h"
#include "../lights/Lights.h"
#include "../cameras/FlyingCamera.h"
#include "../timer/timer.h"
#include "../models/Model.h"
#include "../models/Mesh.h"
#include "../font/font.h"

class OpenGLRenderer {
public:
    // Initializes rendering
    void init();
    // Resets rendering. Enables you to "hot swap" shaders or potentially recover from rendering issues
    void reset();
    // Resets Screen for rendering the next frame
    void startFrame();
    // Sets up shader and OpenGL settings for rendering 3d models
    void setup3DRender();
    // Sets up shader and OpenGL settings for rendering fonts
    void setupFontRender();
    // Sets up shader and OpenGL settings for rendering 2d images
    void setup2DRender();
    // Draws things on the screen
    void endFrame();

    // FPS are calculated each time endFrame() is called
    uint32 getFPS() { return m_FPS; }
    // Delta is calculated each time endFrame() is called
    float32 getDelta() { return m_delta; }

    Shader* getShader3d() { return m_shader3d; }
    Shader* getShaderFont() { return m_shaderFont; }
    Shader* getShader2d() { return m_shader2d; }

    int getWindowHeight() { return m_windowHeight; }
    int getWindowWidth() { return m_windowWidth; }

    FlyingCamera& getCamera() { return m_camera; }

    Font* getFontRenderer() { return m_fontRenderer; }

    // Idk why but the camera has to be initialized here apparently :shrug:
    OpenGLRenderer();
    virtual ~OpenGLRenderer();

    // Pointer to the currently active renderer. Set in the constructor
    inline static OpenGLRenderer* activeRenderer;
private:
    // Private move constructor
    OpenGLRenderer(OpenGLRenderer& renderer) : m_camera{ FlyingCamera(90.0f, 800, 600) } {}
    // Private assignment operator
    OpenGLRenderer& operator=(const OpenGLRenderer& renderer) { return *this; }

    // (Re-)Initializes the lights
    void initLights();

    // Dynamically allocated
    Shader* m_shader3d;
    Shader* m_shaderFont;
    Shader* m_shader2d;
    Font* m_fontRenderer;

    // Not dynamically allocated
    SDL_Window* m_window;
    int m_windowWidth{ 0 };
    int m_windowHeight{ 0 };

    // Can't see without a camera
    FlyingCamera m_camera{ FlyingCamera(90.0f, 800, 600) };

    // Without lights it'd be pretty dark
    DirectionalLight m_sun;
    PointLight m_pointLight;
    SpotLight m_flashlight;

    // For timing stuff
    uint64 m_perfCounterFrequency;
    uint64 m_lastCounter;
    float32 m_delta{ 0.0f };
    uint32 m_FPS{ 0 };
    IndependentTimer m_oneSecondTimer{ std::chrono::seconds(1) };

    // Model matrix
    glm::mat4 m_modelMatrix{ glm::mat4(1.0f) };
    glm::mat4 m_modelViewProj{ 1.0f };

    // Uniform locations
    int m_modelViewProjUniformLocation;
    int m_modelViewUniformLocation;
    int m_invModelViewUniformLocation;
};

// Display debug messages
void openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

#endif