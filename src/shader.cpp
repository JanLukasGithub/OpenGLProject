#include "shader.h"

Shader::Shader(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName) {
    m_shaderId = createShader(vertexShaderFileName, fragmentShaderFileName);
}

Shader::Shader(const std::string& vertexShaderFileName, const std::string& geometryShaderFilename, const std::string& fragmentShaderFileName) {
    m_shaderId = createShader(vertexShaderFileName, geometryShaderFilename, fragmentShaderFileName);
}

Shader::~Shader() {
    glDeleteProgram(m_shaderId);
}

void Shader::bind() {
    glUseProgram(m_shaderId);
}

void Shader::unbind() {
    glUseProgram(0);
}

void Shader::update(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName) {
    this->unbind();
    glDeleteProgram(m_shaderId);
    m_shaderId = createShader(vertexShaderFileName, fragmentShaderFileName);
    this->bind();
}

void Shader::update(const std::string& vertexShaderFileName, const std::string& geometryShaderFilename, const std::string& fragmentShaderFileName) {
    this->unbind();
    glDeleteProgram(m_shaderId);
    m_shaderId = createShader(vertexShaderFileName, geometryShaderFilename, fragmentShaderFileName);
    this->bind();
}

GLuint Shader::compile(const std::string& shaderSource, GLenum type) {
    GLuint id = glCreateShader(type);
    const char* src = shaderSource.c_str();
    glShaderSource(id, 1, &src, 0);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
        int length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);

        std::cerr << "While compiling Shader:\n" << shaderSource << "\n\nReceived errors:\n" << message << std::endl;

        delete[] message;
        return 0;
    }
    return id;
}

std::string Shader::parse(const std::string& filename) {
    std::streampos fileSize;
    std::ifstream file(filename);

    if (file.fail() || !file.is_open()) {
        std::cerr << "Shader " << filename << " could not be opened!" << std::endl;
        file.close();
        return "";
    }

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string contents{};
    contents.resize(fileSize);

    file.read(contents.data(), contents.size());

    file.close();

    return contents;
}

GLuint Shader::createShader(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName) {
    debugOutputEndl("Creating shader...");

    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

    std::string vertexShaderSource = parse(vertexShaderFileName);
    std::string fragmentShaderSource = parse(fragmentShaderFileName);

    GLuint program = glCreateProgram();
    GLuint vs = compile(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fs = compile(fragmentShaderSource, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);

    GLint linkStatus{ 0 };

    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if (!linkStatus) {
        std::cerr << "Couldn't link shader!" << std::endl;
    }

    glDetachShader(program, vs);
    glDetachShader(program, fs);

    glDeleteShader(vs);
    glDeleteShader(fs);

    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_span = endTime - startTime;

    debugOutput("Created shader in "); debugOutput(time_span.count()); debugOutputEndl("ms!");

    return program;
}

GLuint Shader::createShader(const std::string& vertexShaderFileName, const std::string& geometryShaderFilename, const std::string& fragmentShaderFileName) {
    debugOutputEndl("Creating shader...");

    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

    std::string vertexShaderSource = parse(vertexShaderFileName);
    std::string geometryShaderSource = parse(geometryShaderFilename);
    std::string fragmentShaderSource = parse(fragmentShaderFileName);

    GLuint program = glCreateProgram();
    GLuint vs = compile(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint gs = compile(geometryShaderSource, GL_GEOMETRY_SHADER);
    GLuint fs = compile(fragmentShaderSource, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, gs);
    glAttachShader(program, fs);

    glLinkProgram(program);

    GLint linkStatus{ 0 };

    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if (!linkStatus) {
        std::cerr << "Couldn't link shader!" << std::endl;
    }

    glDetachShader(program, vs);
    glDetachShader(program, gs);
    glDetachShader(program, fs);

    glDeleteShader(vs);
    glDeleteShader(gs);
    glDeleteShader(fs);

    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_span = endTime - startTime;

    debugOutput("Created shader in "); debugOutput(time_span.count()); debugOutputEndl("ms!");

    return program;
}