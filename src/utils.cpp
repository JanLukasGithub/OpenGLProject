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

template<>
void debugOutput<glm::mat4>(const glm::mat4 t) {
#ifdef _DEBUG
	std::cout << "[[" << t[0][0] << ", " << t[0][1] << ", " << t[0][2] << ", " << t[0][3] << "], ["
		<< t[1][0] << ", " << t[1][1] << ", " << t[1][2] << ", " << t[1][3] << "], [" 
		<< t[2][0] << ", " << t[2][1] << ", " << t[2][2] << ", " << t[2][3] << "], [" 
		<< t[3][0] << ", " << t[3][1] << ", " << t[3][2] << ", " << t[3][3] << "]]";
#endif
}

template<>
void debugOutputEndl<glm::mat4>(const glm::mat4 t) {
#ifdef _DEBUG
	std::cout << "[[" << t[0][0] << ", " << t[0][1] << ", " << t[0][2] << ", " << t[0][3] << "], ["
		<< t[1][0] << ", " << t[1][1] << ", " << t[1][2] << ", " << t[1][3] << "], [" 
		<< t[2][0] << ", " << t[2][1] << ", " << t[2][2] << ", " << t[2][3] << "], [" 
		<< t[3][0] << ", " << t[3][1] << ", " << t[3][2] << ", " << t[3][3] << "]]" << std::endl;
#endif
}
