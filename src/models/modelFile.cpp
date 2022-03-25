#include "modelFile.h"

int32 ModelFile::addModelFile(const char* filename) {
    int32 index = indexOf(filename);
    if (index != -1)
        return index;

    modelFiles.push_back(ModelFile());
    return modelFiles.size() - 1;
}

int32 ModelFile::indexOf(const char* filename) {
    return std::find(modelFiles.begin(), modelFiles.end(), filename).base() - modelFiles.data();
}