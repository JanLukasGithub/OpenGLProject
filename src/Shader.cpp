#include "Shader.h"

Shader::Shader(const char* vertexShaderFileName, const char* fragmentShaderFileName) {
    shaderId = createShader(vertexShaderFileName, fragmentShaderFileName);
}

Shader::~Shader() {
    glDeleteProgram(shaderId);
}

void Shader::bind() {
    glUseProgram(shaderId);
}

void Shader::unbind() {
    glUseProgram(0);
}

GLuint Shader::compile(std::string shaderSource, GLenum type) {
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

        std::cout << "Shader compile infos: " << message << std::endl;

        delete[] message;
        return 0;
    }
    return id;
}

std::string Shader::parse(const char* filename) {
    FILE* file;
    file = fopen(filename, "rb");
    if (file == nullptr) {
        std::cerr << "File " << filename << " could not be read" << std::endl;
        return 0;
    }

    std::string contents;
    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);
    rewind(file);
    contents.resize(filesize);

    fread(&contents[0], 1, filesize, file);
    fclose(file);

    return contents;
}

GLuint Shader::createShader(const char* vertexShaderFileName, const char* fragmentShaderFileName) {
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

    glDetachShader(program, vs);
    glDetachShader(program, fs);

    glDeleteShader(vs);
    glDeleteShader(fs);

    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_span = endTime - startTime;

    debugOutput("Created shader in "); debugOutput(time_span.count()); debugOutputEndl("ms!");

    return program;
}

void Shader::update(const char* vertexShaderFileName, const char* fragmentShaderFileName) {
    this->unbind();
    glDeleteProgram(shaderId);
    shaderId = createShader(vertexShaderFileName, fragmentShaderFileName);
    this->bind();
}

