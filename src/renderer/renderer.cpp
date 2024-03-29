#include "renderer.h"

Renderer::Renderer() {
    init();

    Renderer::activeRenderer = this;
}

Renderer::~Renderer() {
    delete m_shader3d;
    delete m_shaderFont;
    delete m_shader2d;
    delete m_shaderTerrain;

    delete m_fontRenderer;
    delete m_lights;
}

void Renderer::init() {
    initSDL();

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        std::cin.get();
        throw std::exception();
    }

    SDL_GetWindowSize(m_window, &m_windowWidth, &m_windowHeight);

    debugOutput("OpenGL Version: ");
    debugOutputEndl(glGetString(GL_VERSION));

#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openGLDebugCallback, 0);
#endif

    m_shader3d = new Shader("src/shaders/3d.vs", "src/shaders/3d.fs");
    m_shaderFont = new Shader("src/shaders/font.vs", "src/shaders/font.fs");
    m_shader2d = new Shader("src/shaders/2d.vs", "src/shaders/2d.fs");
    m_shaderTerrain = new Shader("src/shaders/terrain.vs", "src/shaders/terrain.gs", "src/shaders/terrain.fs");

    initUniforms();

    initLights();

    initCounter();

    initCamera();

    initMatrices();

    m_fontRenderer = new Font{ "assets/fonts/OpenSans-Regular.ttf", m_shaderFont };
}

void Renderer::initSDL() {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetSwapInterval(1);

    SDL_SetRelativeMouseMode(SDL_TRUE);

#ifdef _DEBUG
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

    m_window = SDL_CreateWindow("C++ OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(m_window);
}

void Renderer::initLights() {
    m_shader3d->bind();

    delete m_lights;
    m_lights = new Lights();

    m_sun = DirectionalLight{
        .direction = glm::normalize(glm::vec3(-1.0f)),
        .diffuseColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
        .specularColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
        .ambientColor = glm::vec4(0.16f, 0.16f, 0.16f, 1.0f)
    };

    m_pointLight = PointLight{
        .position = glm::vec4(0.0f, 0.0f, 10.0f, 1.0f),
        .linear = 0.027f,
        .diffuseColor = glm::vec4(0.2f, 0.2f, 1.0f, 1.0f),
        .quadratic = 0.0026f,
        .specularColor = glm::vec4(0.2f, 0.2f, 1.0f, 1.0f),
        .ambientColor = glm::vec4(0.04f, 0.04f, 0.2f, 1.0f),
    };

    m_flashlight = SpotLight{
        .position = glm::vec4(0.0f),
        .linear = 0.027f,
        .direction = glm::normalize(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)),
        .quadratic = 0.0026f,
        .diffuseColor = glm::vec4(1.0f),
        .innerCone = 1.0f,
        .specularColor = glm::vec4(1.0f),
        .outerCone = 0.9f,
        .ambientColor = glm::vec4(0.2f),
    };

    m_lights->getDirectionalLightBuffer().add(m_sun);
    m_lights->getPointLightBuffer().add(m_pointLight);
    m_lights->getSpotLightBuffer().add(m_flashlight);

    m_shader3d->unbind();
}

void Renderer::initCounter() {
    m_perfCounterFrequency = SDL_GetPerformanceFrequency();
    m_lastCounter = SDL_GetPerformanceCounter();
}

void Renderer::initCamera() {
    m_camera.translate(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera.update();
}

void Renderer::initMatrices() {
    m_viewProj = m_camera.getViewProjection();
    m_view = m_camera.getView();
    m_invView = glm::transpose(glm::inverse(m_view));
}

void Renderer::initUniforms() {
    m_view_proj_uniform_location = glGetUniformLocation(m_shader3d->getShaderId(), "u_view_proj");
    m_view_uniform_location = glGetUniformLocation(m_shader3d->getShaderId(), "u_view");
    m_inv_view_uniform_location = glGetUniformLocation(m_shader3d->getShaderId(), "u_inv_view");

    Mesh::initUniforms(m_shader3d);
    Font::initUniforms(m_shaderFont);
    Terrain::initUniforms(m_shaderTerrain);
}

void Renderer::reset() {
    m_shader3d->update("src/shaders/3d.vs", "src/shaders/3d.fs");
    m_shaderFont->update("src/shaders/font.vs", "src/shaders/font.fs");
    m_shader2d->update("src/shaders/2d.vs", "src/shaders/2d.fs");
    m_shaderTerrain->update("src/shaders/terrain.vs", "src/shaders/terrain.gs", "src/shaders/terrain.fs");

    initLights();
    initUniforms();
}

void Renderer::startFrame() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_viewProj = m_camera.getViewProjection();
    m_view = m_camera.getView();
    m_invView = glm::transpose(glm::inverse(m_view));

    m_pointLight.position = glm::vec4(m_pointLight.position, 1.0f) * glm::rotate(glm::mat4(1.0f), m_delta, { 0.0f, 1.0f, 0.0f });
}

void Renderer::setup3DRender() {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    m_shader3d->bind();

    DirectionalLight transformedSun{
        .direction = glm::normalize(glm::mat3(m_invView) * m_sun.direction),
        .diffuseColor = m_sun.diffuseColor,
        .specularColor = m_sun.specularColor,
        .ambientColor = m_sun.ambientColor
    };
    m_lights->getDirectionalLightBuffer().set(transformedSun, 0);

    PointLight transformedPoint{
        .position = glm::mat3(m_camera.getView()) * m_pointLight.position,
        .linear = m_pointLight.linear,
        .diffuseColor = m_pointLight.diffuseColor,
        .quadratic = m_pointLight.quadratic,
        .specularColor = m_pointLight.specularColor,
        .alignment1 = 0.0f,
        .ambientColor = m_pointLight.ambientColor,
        .alignment2 = 0.0f
    };
    m_lights->getPointLightBuffer().set(transformedPoint, 0);

    m_lights->bind();

    glUniformMatrix4fv(m_view_proj_uniform_location, 1, GL_FALSE, &m_viewProj[0][0]);
    glUniformMatrix4fv(m_view_uniform_location, 1, GL_FALSE, &m_view[0][0]);
    glUniformMatrix4fv(m_inv_view_uniform_location, 1, GL_FALSE, &m_invView[0][0]);
}

void Renderer::setupFontRender() {
    m_shaderFont->bind();

    glm::mat4 ortho = glm::ortho(0.0f, (float)m_windowWidth, (float)m_windowHeight, 0.0f);
    glUniformMatrix4fv(glGetUniformLocation(m_shaderFont->getShaderId(), "u_modelViewProj"), 1, GL_FALSE, &ortho[0][0]);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
}

void Renderer::setup2DRender() {
    m_shader2d->bind();

    glm::mat4 ortho = glm::ortho(0.0f, (float)m_windowWidth, (float)m_windowHeight, 0.0f);
    glUniformMatrix4fv(glGetUniformLocation(m_shader2d->getShaderId(), "u_modelViewProj"), 1, GL_FALSE, &ortho[0][0]);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
}

void Renderer::setupTerrainRender() {
    m_shaderTerrain->bind();

    m_lights->bind();

    glUniformMatrix4fv(glGetUniformLocation(m_shaderTerrain->getShaderId(), "u_viewProj"), 1, GL_FALSE, &m_viewProj[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shaderTerrain->getShaderId(), "u_view"), 1, GL_FALSE, &m_view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shaderTerrain->getShaderId(), "u_invView"), 1, GL_FALSE, &m_invView[0][0]);
}

void Renderer::endFrame() {
    SDL_GL_SwapWindow(m_window);

    uint64 endCounter = SDL_GetPerformanceCounter();
    uint64 counterElapsed = endCounter - m_lastCounter;
    m_delta = ((float32)counterElapsed) / ((float32)m_perfCounterFrequency);
    if (m_oneSecondTimer.hasTimeElapsed())
        m_FPS = (uint32)((float32)m_perfCounterFrequency / (float32)counterElapsed);
    m_lastCounter = endCounter;
}

void openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << "[OpenGL]: " << message << std::endl;
}