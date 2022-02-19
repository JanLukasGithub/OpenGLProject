/*
 * typedefs.h
 *
 *  Created on: Jul 21, 2021
 *      Author: jan
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <cstdint>
#include <cstring>
#include <iostream>
#include <filesystem>

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

typedef float float32;
typedef double float64;

struct Vertex {
	glm::vec3 position;

	glm::vec3 normal;

	glm::vec3 tangent;

	glm::vec2 textureCoords;
};

/* Example: "../folder/name.extension" -> "name.extension" */
const char* getFilename(const char* path);

/* Example: "../folder/name.extension" -> "extension" */
const char* getFileExtension(const char* path);

/* Example: "../folder/name.extension" -> "../folder/" */
const char* getFilePath(const char* path);

/* Example: "../folder/name" -> "../folder/name.obj" -> ".obj" */
const char* getExtensionFromModelName(std::string& filenameNoExtension);

/* Only prints out information if compiled with debug flags.
 * Automatically adds a std::endl at the end */
template<typename T>
void debugOutputEndl(const T* t);

/* Only prints out information if compiled with debug flags.
 * Automatically adds a std::endl at the end */
template<typename T>
void debugOutputEndl(const T t);

/* Only prints out information if compiled with debug flags. */
template<typename T>
void debugOutput(const T* t);

/* Only prints out information if compiled with debug flags. */
template<typename T>
void debugOutput(const T t);

#endif /* UTILS_H_ */
