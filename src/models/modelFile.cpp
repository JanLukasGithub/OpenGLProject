#include "modelFile.h"

int32 ModelFile::indexOf(const char* filename) {
    return std::find(modelFiles.begin(), modelFiles.end(), filename).base() - modelFiles.data();
}