#include "model.h"

Model::Model(Model&& model) : m_filename{ model.m_filename }, m_meshes{ std::move(model.m_meshes) }, m_model_mat_buffer{ std::move(model.m_model_mat_buffer) } {}

Model::Model(const std::string& filename) : m_filename{ filename } {
    readModelFromFile();
}

Model::~Model() noexcept {}

GLsizeiptr Model::add_instance() noexcept {
    glm::mat4 modelMat{ 1.0f };
    return m_model_mat_buffer.add(modelMat);
}

GLsizeiptr Model::add_instance(const glm::vec3 position) noexcept {
    glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), position);
    return m_model_mat_buffer.add(modelMat);
}

GLsizeiptr Model::add_instance(const glm::mat4 modelMat) noexcept {
    return m_model_mat_buffer.add(modelMat);
}

void Model::set_instance(const GLsizeiptr index, const glm::mat4& model_mat) {
    if (index == -1)
        return;

    m_model_mat_buffer.set(index, model_mat);
}

void Model::remove_instance(const GLsizeiptr index) {
    if (index == -1)
        return;
    
    m_model_mat_buffer.remove(index);
}

void Model::render_models() const noexcept {
    for (int i = 0; i < m_meshes.size(); i++) {
        m_meshes[i].render(m_model_mat_buffer.get_size());
    }
}

void Model::render() const noexcept {
    render_models();
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

        m_materialIndices.push_back(Material_Manager::get_material_index(aiMat, m_filename));
    }
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