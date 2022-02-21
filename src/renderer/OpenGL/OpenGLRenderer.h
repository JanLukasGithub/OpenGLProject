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
#include "../../../lib/glm/glm.hpp"
#include "../../../lib/glm/ext/matrix_transform.hpp"
#include "../../../lib/glm/gtc/matrix_transform.hpp"

#include "../renderer.h"
#include "../../Shader.h"
#include "../../lights/Lights.h"
#include "../../cameras/FlyingCamera.h"
#include "../../timer/timer.h"

class OpenGLRenderer : Renderer {
public:
    // Initializes rendering
    virtual void init() override;
    // Resets rendering. Enables you to "hot swap" shaders or potentially recover from rendering issues
    virtual void reset() override;
    // Resets Screen for rendering the next frame
    virtual void startFrame() override;
    // Sets up shader and render library settings for rendering 3d models
    virtual void setup3DRender() override;
    // Sets up shader and render library settings for rendering fonts
    virtual void setup2dRender() override;
    // Draws things on the screen
    virtual void endFrame() override;

    // FPS are calculated each time endFrame() is called
    virtual uint32 getFPS() override { return m_FPS; }
    // Delta is calculated each time endFrame() is called
    virtual float32 getDelta() override { return m_delta; }

    Shader* getShader3d() { return m_shader3d; }
    Shader* getShader2d() { return m_shader2d; }

    int getWindowHeight() { return m_windowHeight; }
    int getWindowWidth() { return m_windowWidth; }

    FlyingCamera& getCamera() { return m_camera; }

    // Idk why but the camera has to be initialized here apparently :shrug:
    OpenGLRenderer() : m_camera{ FlyingCamera(90.0f, 800, 600) } { OpenGLRenderer::activeRenderer = this; }
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
    Shader* m_shader2d;

    // Not dynamically allocated
    SDL_Window* m_window;
    int m_windowWidth{ 0 };
    int m_windowHeight{ 0 };

    // Can't see without a camera
    FlyingCamera m_camera;

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