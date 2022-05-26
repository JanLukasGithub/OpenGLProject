#include "utils.h"

const char* getFilename(const char* path) {
	uint64 len = strlen(path);

	const char* lastSlash = path;
	for (long unsigned int i = 0; i < len; i++) {
		if (path[i] == '/' || path[i] == '\\') {
			lastSlash = path + i + 1;
		}
	}

	return lastSlash;
}

const char* getFileExtension(const char* path) {
	uint64 len = strlen(path);

	const char* lastPoint = path;
	for (long unsigned int i = 0; i < len; i++) {
		if (path[i] == '.') {
			lastPoint = path + i + 1;
		}
	}

	return lastPoint;
}

const char* getFilePath(const char* path) {
	uint64 len = strlen(path);

	const char* lastSlash = path;
	for (uint64 i = 0; i < len; i++) {
		if (path[i] == '/' || path[i] == '\\') {
			lastSlash = path + i + 1;
		}
	}

	uint64 newStringLen = lastSlash - path;
	char* newString = new char[newStringLen + 1];
	for (int i = 0; i < newStringLen + 1; i++) {
		newString[i] = '\0';
	}

	strncpy(newString, path, newStringLen);
	return newString;
}