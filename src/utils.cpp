#include "utils.h"

std::string getFilename(const std::string& path) {
	return path.substr(path.find_last_of('/') + 1);
}

std::string getFileExtension(const std::string& path) {
	return path.substr(path.find_last_of('.') + 1);
}

std::string getFilePath(const std::string& path) {
	return path.substr(0, path.find_last_of('/') + 1);
}