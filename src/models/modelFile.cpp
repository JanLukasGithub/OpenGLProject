#include "modelFile.h"

int32 ModelFile::addModelFile(const char* filename) {
    int32 index = indexOf(filename);
    if (index != -1)
        return index;

    modelFiles.push_back(ModelFile(filename));
    return modelFiles.size() - 1;
}

int32 ModelFile::indexOf(const char* filename) {
    return std::find(modelFiles.begin(), modelFiles.end(), filename).base() - modelFiles.data();
}

ModelFile& ModelFile::getFromList(int32 index) {
    return modelFiles.at(index);
}

ModelFile::ModelFile(const char* filename) {
    ;
}

ModelFile::~ModelFile() noexcept {
	for (unsigned int i = 0; i < m_meshes.size(); i++) {
		delete m_meshes[i];
	}
}

bool operator==(const ModelFile& model, const char* const filename) {
    return strcmp(model.m_filename, filename) == 0;
}

bool operator==(const ModelFile& model1, const ModelFile& model2) {
    return strcmp(model1.m_filename, model2.m_filename) == 0;
}