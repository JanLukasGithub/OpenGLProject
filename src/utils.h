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
void debugOutputEndl(const T* t) {
#ifdef _DEBUG
	std::cout << t << std::endl;
#endif
}

/* Only prints out information if compiled with debug flags.
 * Automatically adds a std::endl at the end */
template<typename T>
void debugOutputEndl(const T t) {
#ifdef _DEBUG
	std::cout << t << std::endl;
#endif
}

/* Only prints out information if compiled with debug flags. */
template<typename T>
void debugOutput(const T* t) {
#ifdef _DEBUG
	std::cout << t;
#endif
}

/* Only prints out information if compiled with debug flags. */
template<typename T>
void debugOutput(const T t) {
#ifdef _DEBUG
	std::cout << t;
#endif
}

/**
 * original = { {1, 2, 3},
 * 		{3, 4, 5},
 * {5, 6, 7}}
 *
 * result = getRectFrom2dArray(original, 1, 1, 2, 2);
 *
 * result = {{4, 5}, {6, 7}}
 *
 * xStart and yStart are indices (start at 0), lengthX and lengthY determine how big the resulting vector will be
 */
template<typename T>
std::vector<std::vector<T>> getRectFrom2dArray(const std::vector<std::vector<T>>& original, uint64 xStart, uint64 yStart, uint64 lengthX, uint64 lengthY) {
	std::vector<std::vector<T>> result{};
	result.resize(lengthX);

	for (int x = 0; x < lengthX; x++) {
		result[x].resize(lengthY);

		memcpy(result[x].data(), &original[xStart + x][yStart], lengthY * sizeof(T));
	}

	return result;
}

#endif