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
#include "../../lib/glm/gtc/matrix_transform.hpp"

#include "../shader.h"
#include "../lights.h"
#include "../cameras/flyingCamera.h"
#include "../timer/timer.h"
#include "../models/model.h"
#include "../models/mesh.h"
#include "../font/font.h"

class Renderer {
public:
    // Pointer to the currently active renderer. Set in the constructor
    inline static Renderer* activeRenderer;

private:
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
    Timer m_oneSecondTimer{ std::chrono::seconds(1) };

    // Model matrix
    glm::mat4 m_modelMatrix{ glm::mat4(1.0f) };
    glm::mat4 m_modelViewProj{ 1.0f };

    // Uniform locations. Not static because only one instance of this class should exist
    int m_modelViewProjUniformLocation;
    int m_modelViewUniformLocation;
    int m_invModelViewUniformLocation;

public:
    Renderer();
    virtual ~Renderer();

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
    uint32 getFPS() const noexcept { return m_FPS; }
    // Delta is calculated each time endFrame() is called
    float32 getDelta() const noexcept { return m_delta; }

    Shader* getShader3d() const noexcept { return m_shader3d; }
    Shader* getShaderFont() const noexcept { return m_shaderFont; }
    Shader* getShader2d() const noexcept { return m_shader2d; }

    int getWindowHeight() const noexcept { return m_windowHeight; }
    int getWindowWidth() const noexcept { return m_windowWidth; }

    FlyingCamera& getCamera() noexcept { return m_camera; }

    Font* getFontRenderer() const noexcept { return m_fontRenderer; }
private:
    // Private move constructor
    Renderer(Renderer& renderer) {}
    // Private assignment operator
    Renderer& operator=(const Renderer& renderer) { return *this; }

    // (Re-)Initializes the lights
    void initLights();
};

// Display debug messages
void openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

#endif