#include "model.h"

Model& Model::addModelFile(const char* const filename) {
    Model* model = getFromList(filename);
    if (model)
        return *model;

    modelFiles.push_back(Model(filename));
    return modelFiles[modelFiles.size() - 1];
}

const int32 Model::indexOf(const char* const filename) noexcept {
    std::vector<Model>::iterator found = std::find(modelFiles.begin(), modelFiles.end(), filename);
    return found == modelFiles.end() ? -1 : found.base() - modelFiles.data();
}

const int32 Model::indexOf(const Model& model) noexcept {
    return indexOf(model.m_filename);
}

Model& Model::getFromList(const int32 index) noexcept {
    return modelFiles[index];
}

Model* Model::getFromList(const char* const filename) noexcept {
    int index = indexOf(filename);
    return index == -1 ? nullptr : &modelFiles[index];
}

const int Model::getListSize() noexcept {
    return modelFiles.size();
}

Model::Model(Model&& model) : m_filename{ model.m_filename }, m_meshes{ model.m_meshes }, m_models{ model.m_models } {
    for (unsigned int i = 0; i < model.m_meshes.size(); i++) {
        model.m_meshes[i] = nullptr;
    }
    for (unsigned int i = 0; i < model.m_models.size(); i++) {
        model.m_models[i] = nullptr;
    }
}

Model::Model(const char* const filename) : m_filename{ filename } {
    readModelFromFile();
}

Model::~Model() noexcept {
    for (unsigned int i = 0; i < m_meshes.size(); i++) {
        delete m_meshes[i];
    }
    for (unsigned int i = 0; i < m_models.size(); i++) {
        delete m_models[i];
    }
}

void Model::addInstance() noexcept {
    m_models.push_back(new ModelInstance());
}

void Model::addInstance(const glm::vec3 position) noexcept {
    m_models.push_back(new ModelInstance(position));
}

void Model::addInstance(const glm::mat4 modelMat) noexcept {
    m_models.push_back(new ModelInstance(modelMat));
}

void Model::renderModels() const noexcept {
    if (m_models.size() < 1)
        return;

    for (int i = 0; i < m_meshes.size(); i++) {
        glUniformMatrix4fv(ModelInstance::modelMatLocation, 1, GL_FALSE, &((m_models[0]->getModelMat())[0][0]));
        m_meshes[i]->render();

        for (int j = 1; j < m_models.size(); j++) {
            glUniformMatrix4fv(ModelInstance::modelMatLocation, 1, GL_FALSE, &((m_models[j]->getModelMat())[0][0]));
            m_meshes[i]->fastRender();
        }
    }
}

void Model::render() const noexcept {
    renderModels();
}

bool operator==(const Model& ModelFile, const char* const filename) {
    return strcmp(ModelFile.m_filename, filename) == 0;
}

bool operator==(const Model& model1, const Model& model2) {
    return model1 == model2.m_filename;
}

void Model::readModelFromFile() {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(m_filename, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality);

    if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
        std::cerr << "Error while loading the Model with assimp: " << importer.GetErrorString() << std::endl;
        throw std::exception();
    }

    debugOutputEndl("Loading Model from file...");

    processMaterials(scene);

    processNodes(scene, scene->mRootNode);
}

void Model::processMaterials(const aiScene* scene) {
    uint32 numMaterials = scene->mNumMaterials;
    m_materialIndices.reserve(numMaterials);

    for (uint32 i = 0; i < numMaterials; i++) {
        Material mat = { };
        aiMaterial* aiMat = scene->mMaterials[i];

        mat.diffuse = getColor(aiMat, AI_MATKEY_COLOR_DIFFUSE);
        mat.specular = getColor(aiMat, AI_MATKEY_COLOR_SPECULAR);
        mat.emissive = getColor(aiMat, AI_MATKEY_COLOR_EMISSIVE);

        mat.shininess = getFloat(aiMat, AI_MATKEY_SHININESS);

        mat.specular *= getFloat(aiMat, AI_MATKEY_SHININESS_STRENGTH);

        mat.diffuseMapName = getTexturePath(aiMat, aiTextureType_DIFFUSE);
        mat.normalMapName = getTexturePath(aiMat, aiTextureType_NORMALS);

        std::vector<Material>::iterator found = std::find(Material::materials.begin(), Material::materials.end(), mat);
        if (found != Material::materials.end()) {
            m_materialIndices.push_back(found.base() - Material::materials.data());
            continue;
        }

        loadTexture(mat.diffuseMapName, &mat.diffuseMap);
        loadTexture(mat.normalMapName, &mat.normalMap);

        Material::materials.push_back(mat);
        m_materialIndices.push_back(Material::materials.size() - 1);
    }
}

glm::vec3 Model::getColor(aiMaterial* mat, const char* pKey, unsigned int type, unsigned int idx) {
    aiColor3D color(0.0f, 0.0f, 0.0f);
    if (AI_SUCCESS != mat->Get(pKey, type, idx, color)) {
        debugOutputEndl("No color found, using default color black!");
    }
    return glm::vec3(color.r, color.g, color.b);
}

float Model::getFloat(aiMaterial* mat, const char* pKey, unsigned int type, unsigned int idx) {
    float value(0.0f);
    if (AI_SUCCESS != mat->Get(pKey, type, idx, value)) {
        debugOutputEndl("No value, using default value 0!");
    }
    return value;
}

std::string Model::getTexturePath(aiMaterial* mat, aiTextureType type) {
    uint32 numTextures = mat->GetTextureCount(type);

    if (numTextures > 0) {
        aiString textureNameBuffer{ };
        mat->GetTexture(type, 0, &textureNameBuffer);
        return std::string(getFilePath(m_filename)) + textureNameBuffer.C_Str();
    } else {
        debugOutputEndl("No texture found, using default texture!");
        return std::string("");
    }
}

void Model::loadTexture(std::string& path, GLuint* textureId) {
    if (path.compare("") == 0)
        return;

    int32 textureWidth = 0;
    int32 textureHeight = 0;
    int32 bitsPerPixel = 0;

    stbi_set_flip_vertically_on_load(true);

    auto textureBuffer = stbi_load(path.c_str(), &textureWidth, &textureHeight, &bitsPerPixel, 4);
    if (!textureBuffer) {
        std::cerr << "Couldn't load image at " << path.c_str() << "! Aborting!" << std::endl;
        throw std::exception();
    }

    glGenTextures(1, textureId);
    glBindTexture(GL_TEXTURE_2D, *textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);

    stbi_image_free(textureBuffer);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Model::processNodes(const aiScene* scene, aiNode* node) {
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh);
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        processNodes(scene, node->mChildren[i]);
    }
}

void Model::processMesh(aiMesh* mesh) {
    uint64 numVertices = mesh->mNumVertices;
    // Dynamically allocated to ensure the variable lives after exiting function. Owned by the Mesh created at the bottom of the function
    // TODO: Remove dynamic allocation
    std::vector<Vertex>* vertices = new std::vector<Vertex>();
    vertices->reserve(numVertices);

    // Dynamically allocated to ensure the variable lives after exiting function. Owned by the Mesh created at the bottom of the function
    // TODO: Remove dynamic allocation
    std::vector<uint32>* indices = new std::vector<uint32>();

    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for (uint32_t j = 0; j < face.mNumIndices; j++) {
            indices->push_back(face.mIndices[j]);
        }
    }

    for (uint64 i = 0; i < numVertices; i++) {
        Vertex v = { };

        v.position.x = mesh->mVertices[i].x;
        v.position.y = mesh->mVertices[i].y;
        v.position.z = mesh->mVertices[i].z;

        v.normal.x = mesh->mNormals[i].x;
        v.normal.y = mesh->mNormals[i].y;
        v.normal.z = mesh->mNormals[i].z;

        v.tangent.x = mesh->mTangents[i].x;
        v.tangent.y = mesh->mTangents[i].y;
        v.tangent.z = mesh->mTangents[i].z;

        v.textureCoords.x = mesh->mTextureCoords[0][i].x;
        v.textureCoords.y = mesh->mTextureCoords[0][i].y;

        vertices->push_back(v);
    }

    int globalMaterialIndex = m_materialIndices[mesh->mMaterialIndex];

    m_meshes.push_back(new Mesh(vertices, indices, globalMaterialIndex));
}