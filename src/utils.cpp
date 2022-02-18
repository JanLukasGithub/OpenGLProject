/*
 * utils.cpp
 *
 *  Created on: Jan 30, 2022
 *      Author: jan
 */

#include "utils.h"

const char* getFilename(const char* path) {
	// Length of the path string
	uint64 len = strlen(path);
	/* Pointer to one after the last / or \ */
	const char* lastSlash = path;
	/* Actually get the pointer to the last / or \ */
	for (long unsigned int i = 0; i < len; i++) {
		if (path[i] == '/' || path[i] == '\\') {
			lastSlash = path + i + 1;
		}
	}
	/* Return the pointer to one after the last / or \ */
	return lastSlash;
}

const char* getFileExtension(const char* path) {
	// Length of the path string
	uint64 len = strlen(path);
	// Pointer to one after the last .
	const char* lastPoint = path;
	// Actually get the pointer to the last .
	for (long unsigned int i = 0; i < len; i++) {
		if (path[i] == '.') {
			lastPoint = path + i + 1;
		}
	}
	// Return the pointer to one after the last .
	return lastPoint;
}

const char* getFilePath(const char* path) {
	// Length of the path string
	uint64 len = strlen(path);
	/* Pointer to one after the last / or \ */
	const char* lastSlash = path;
	/* Actually get the pointer to the last / or \ */
	for (uint64 i = 0; i < len; i++) {
		if (path[i] == '/' || path[i] == '\\') {
			lastSlash = path + i + 1;
		}
	}

	// Length of the new string
	uint64 newStringLen = lastSlash - path;
	// The new string
	char* newString = new char[newStringLen + 1];
	// Ensuring the string is actually null-initialized
	for (int i = 0; i <= newStringLen; i++) {
		newString[i] = '\0';
	}

	// Copy as many chars to the new string as it's long
	strncpy(newString, path, newStringLen);
	// Return the pointer to the new string
	return newString;
}

/* Example: "../folder/name" -> "../folder/name.obj" -> ".obj" */
const char* getExtensionFromModelName(std::string& filenameNoExtension) {
	if (std::filesystem::exists(filenameNoExtension + ".obj")) {
		return ".obj";
	} else if (std::filesystem::exists(filenameNoExtension + ".fbx")) {
		return ".fbx";
	} else if (std::filesystem::exists(filenameNoExtension + ".FBX")) {
		return ".FBX";
	}
	// bmf has least priority as it may not support features of the other formats
	else if (std::filesystem::exists(filenameNoExtension + ".bmf")) {
		return ".bmf";
	}
	return "";
}

template void debugOutputEndl<char>(const char* t);
template void debugOutputEndl<unsigned char>(const unsigned char* t);
template<typename T>
void debugOutputEndl(const T* t) {
#ifdef _DEBUG
	std::cout << t << std::endl;
#endif
}

template void debugOutputEndl<uint32>(const uint32 t);
template<typename T>
void debugOutputEndl(const T t) {
#ifdef _DEBUG
	std::cout << t << std::endl;
#endif
}

template void debugOutput<char>(const char* t);
template<typename T>
void debugOutput(const T* t) {
#ifdef _DEBUG
	std::cout << t;
#endif
}

template void debugOutput<double>(const double t);
template<typename T>
void debugOutput(const T t) {
#ifdef _DEBUG
	std::cout << t;
#endif
}
