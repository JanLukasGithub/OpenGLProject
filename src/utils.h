#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <iostream>
#include <vector>
#include <cstring>

#include "../lib/glm/glm.hpp"

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint8_t byte;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef short float16;
typedef float float32;
typedef double float64;

typedef bool boolean;

struct Vertex {
	glm::vec3 position;

	glm::vec3 normal;

	glm::vec3 tangent;

	glm::vec2 textureCoords;
};

/* Example: "../folder/name.extension" -> "name.extension" */
std::string getFilename(const std::string& path);

/* Example: "../folder/name.extension" -> "extension" */
std::string getFileExtension(const std::string& path);

/** Example: "../folder/name.extension" -> "../folder/" */
std::string getFilePath(const std::string& path);

/* Only prints out information if compiled with debug flags.
 * Automatically adds a std::endl at the end */
template<typename T>
void debugOutputEndl(const T t) {
#ifdef _DEBUG
	std::cout << t << std::endl;
#endif
}

template<>
void debugOutputEndl<glm::mat4>(const glm::mat4 t);

/* Only prints out information if compiled with debug flags. */
template<typename T>
void debugOutput(const T t) {
#ifdef _DEBUG
	std::cout << t;
#endif
}

template<>
void debugOutput<glm::mat4>(const glm::mat4 t);

#endif