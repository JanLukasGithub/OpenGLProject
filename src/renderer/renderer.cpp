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

    initLights();

    initCounter();

    initCamera();

    initMatrices();

    initUniforms();

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

    m_sun = DirectionalLight{
        glGetUniformLocation(m_shader3d->getShaderId(), "u_directionalLight.direction"),
        // Direction
        glm::vec3(-1.0f),
        // Color
        glm::vec3(0.8f, 0.8f, 0.8f),
        // Ambient color
        glm::vec3(0.16f, 0.16f, 0.16f) };
    glUniform3fv(glGetUniformLocation(m_shader3d->getShaderId(), "u_directionalLight.diffuse"), 1, (float*)&m_sun.color.r);
    glUniform3fv(glGetUniformLocation(m_shader3d->getShaderId(), "u_directionalLight.specular"), 1, (float*)&m_sun.color.r);
    glUniform3fv(glGetUniformLocation(m_shader3d->getShaderId(), "u_directionalLight.ambient"), 1, (float*)&m_sun.ambientColor.r);

    m_pointLight = PointLight{
        glGetUniformLocation(m_shader3d->getShaderId(), "u_pointLight.position"),
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
    glUniform3fv(glGetUniformLocation(m_shader3d->getShaderId(), "u_pointLight.diffuse"), 1, (float*)&m_pointLight.color.r);
    glUniform3fv(glGetUniformLocation(m_shader3d->getShaderId(), "u_pointLight.specular"), 1, (float*)&m_pointLight.color.r);
    glUniform3fv(glGetUniformLocation(m_shader3d->getShaderId(), "u_pointLight.ambient"), 1, (float*)&m_pointLight.ambientColor.r);

    glUniform1f(glGetUniformLocation(m_shader3d->getShaderId(), "u_pointLight.linear"), m_pointLight.linear);
    glUniform1f(glGetUniformLocation(m_shader3d->getShaderId(), "u_pointLight.quadratic"), m_pointLight.quadratic);

    m_flashlight = SpotLight{
        glGetUniformLocation(m_shader3d->getShaderId(), "u_spotLight.position"),
        glGetUniformLocation(m_shader3d->getShaderId(), "u_spotLight.direction"),
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

    glUniform3fv(m_flashlight.positionUniformLocation, 1, (float*)&m_flashlight.position.x);
    glUniform3fv(m_flashlight.directionUniformLocation, 1, (float*)&m_flashlight.direction.x);

    glUniform3fv(glGetUniformLocation(m_shader3d->getShaderId(), "u_spotLight.diffuse"), 1, (float*)&m_flashlight.color.r);
    glUniform3fv(glGetUniformLocation(m_shader3d->getShaderId(), "u_spotLight.specular"), 1, (float*)&m_flashlight.color.r);
    glUniform3fv(glGetUniformLocation(m_shader3d->getShaderId(), "u_spotLight.ambient"), 1, (float*)&m_flashlight.ambientColor.r);

    glUniform1f(glGetUniformLocation(m_shader3d->getShaderId(), "u_spotLight.linear"), m_flashlight.linear);
    glUniform1f(glGetUniformLocation(m_shader3d->getShaderId(), "u_spotLight.quadratic"), m_flashlight.quadratic);

    glUniform1f(glGetUniformLocation(m_shader3d->getShaderId(), "u_spotLight.innerCone"), m_flashlight.innerCone);
    glUniform1f(glGetUniformLocation(m_shader3d->getShaderId(), "u_spotLight.outerCone"), m_flashlight.outerCone);

    m_shader3d->unbind();
}

void Renderer::initCounter() {
    m_perfCounterFrequency = SDL_GetPerformanceFrequency();
    m_lastCounter = SDL_GetPerformanceCounter();
}

void Renderer::initCamera() {
    m_camera.translate(glm::vec3(0.0f, 0.0f, 5.0f));
    m_camera.update();
}

void Renderer::initMatrices() {
    m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(1.0f));
    m_modelViewProj = m_camera.getViewProjection() * m_modelMatrix;
}

void Renderer::initUniforms() {
    m_modelViewProjUniformLocation = glGetUniformLocation(m_shader3d->getShaderId(), "u_modelViewProj");
    m_modelViewUniformLocation = glGetUniformLocation(m_shader3d->getShaderId(), "u_modelView");
    m_invModelViewUniformLocation = glGetUniformLocation(m_shader3d->getShaderId(), "u_invModelView");

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
}

void Renderer::setup3DRender() {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    m_shader3d->bind();

    m_modelViewProj = m_camera.getViewProjection() * m_modelMatrix;
    glm::mat4 modelView = m_camera.getView() * m_modelMatrix;
    glm::mat4 invModelView = glm::transpose(glm::inverse(modelView));

    glm::vec4 transformedSunDirection = glm::transpose(glm::inverse(m_camera.getView())) * glm::vec4(m_sun.direction, 1.0f);
    glUniform3fv(m_sun.directionUniformLocation, 1, (float*)&transformedSunDirection.x);

    glm::mat4 pointLightMatrix = glm::rotate(glm::mat4(1.0f), m_delta, { 0.0f, 1.0f, 0.0f });
    m_pointLight.position = m_pointLight.position * pointLightMatrix;
    glm::vec3 transformedPointLightPosition = (glm::vec3)(m_camera.getView() * m_pointLight.position);
    glUniform3fv(m_pointLight.positionUniformLocation, 1, (float*)&transformedPointLightPosition.x);

    glUniformMatrix4fv(m_modelViewProjUniformLocation, 1, GL_FALSE, &m_modelViewProj[0][0]);
    glUniformMatrix4fv(m_modelViewUniformLocation, 1, GL_FALSE, &modelView[0][0]);
    glUniformMatrix4fv(m_invModelViewUniformLocation, 1, GL_FALSE, &invModelView[0][0]);
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

    m_modelViewProj = m_camera.getViewProjection() * m_modelMatrix;
    glUniformMatrix4fv(glGetUniformLocation(m_shaderTerrain->getShaderId(), "u_modelViewProj"), 1, GL_FALSE, &m_modelViewProj[0][0]);
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