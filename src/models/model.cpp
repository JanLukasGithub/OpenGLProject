#include "model.h"

Model& Model::addModelFile(const std::string& filename) {
    Model* model = getFromList(filename);
    if (model)
        return *model;

    modelFiles.push_back(Model(filename));
    return modelFiles[modelFiles.size() - 1];
}

const int32 Model::indexOf(const std::string& filename) noexcept {
    std::vector<Model>::iterator found = std::find(modelFiles.begin(), modelFiles.end(), filename);
    return found == modelFiles.end() ? -1 : found.base() - modelFiles.data();
}

const int32 Model::indexOf(const Model& model) noexcept {
    return indexOf(model.m_filename);
}

Model& Model::getFromList(const int32 index) noexcept {
    return modelFiles[index];
}

Model* Model::getFromList(const std::string& filename) noexcept {
    int index = indexOf(filename);
    return index == -1 ? nullptr : &modelFiles[index];
}

const int Model::getListSize() noexcept {
    return modelFiles.size();
}

Model::Model(Model&& model) : m_filename{ model.m_filename }, m_meshes{ std::move(model.m_meshes) }, m_model_mat_buffer{ std::move(model.m_model_mat_buffer) } {}

Model::Model(const std::string& filename) : m_filename{ filename } {
    readModelFromFile();
}

Model::~Model() noexcept {}

GLsizeiptr Model::addInstance() noexcept {
    glm::mat4 modelMat{ 1.0f };
    return m_model_mat_buffer.add(modelMat);
}

GLsizeiptr Model::addInstance(const glm::vec3 position) noexcept {
    glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), position);
    return m_model_mat_buffer.add(modelMat);
}

GLsizeiptr Model::addInstance(const glm::mat4 modelMat) noexcept {
    return m_model_mat_buffer.add(modelMat);
}

void Model::renderModels() const noexcept {
    for (int i = 0; i < m_meshes.size(); i++) {
        m_meshes[i].render(m_model_mat_buffer.get_size());
    }
}

void Model::render() const noexcept {
    renderModels();
}

bool operator==(const Model& ModelFile, const std::string& filename) {
    return filename == ModelFile.m_filename;
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

    debugOutputEndl("Loading Model from file " + m_filename + "...");

    processMaterials(scene);

    processNodes(scene, scene->mRootNode);
}

void Model::processMaterials(const aiScene* scene) {
    uint32 numMaterials = scene->mNumMaterials;
    m_materialIndices.reserve(numMaterials);

    for (uint32 i = 0; i < numMaterials; i++) {
        aiMaterial* aiMat = scene->mMaterials[i];

        Material mat{ aiMat, m_filename };

        std::vector<Material>::iterator found = std::find(Material::materials.begin(), Material::materials.end(), mat);
        if (found != Material::materials.end()) {
            m_materialIndices.push_back(found - Material::materials.begin());
            continue;
        }

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
    if (mat->GetTextureCount(type) > 0) {
        aiString textureNameBuffer{ };
        mat->GetTexture(type, 0, &textureNameBuffer);
        return getFilePath(m_filename) + textureNameBuffer.C_Str();
    }

    debugOutputEndl("No texture found, using default texture!");
    return std::string("");
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

    std::vector<Vertex> vertices{};
    vertices.reserve(numVertices);

    std::vector<uint32> indices{};

    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for (uint32_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
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

        vertices.push_back(v);
    }

    int globalMaterialIndex = m_materialIndices[mesh->mMaterialIndex];

    m_meshes.push_back(Mesh(vertices, indices, globalMaterialIndex, m_model_mat_buffer));
}