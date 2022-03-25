#include "modelFile.h"

int32 ModelFile::addModelFile(const char* filename) {
    int32 index = indexOf(filename);
    if (index != -1)
        return index;

    modelFiles.push_back(ModelFile(filename));
    return modelFiles.size() - 1;
}

int32 ModelFile::indexOf(const char* filename) {
    auto found = std::find(modelFiles.begin(), modelFiles.end(), filename);
    return found == modelFiles.end() ? -1 : found.base() - modelFiles.data();
}

int32 ModelFile::indexOf(const ModelFile& model) {
    return indexOf(model.m_filename);
}

ModelFile& ModelFile::getFromList(int32 index) {
    return modelFiles.at(index);
}

ModelFile::ModelFile(ModelFile&& model) : m_filename{ model.m_filename }, m_meshes{ model.m_meshes } {
    for (uint32 i = 0; i < model.m_meshes.size(); i++) {
        model.m_meshes[i] = nullptr;
    }
}

ModelFile::ModelFile(const char* filename) {
    readModelFromFile(filename);
}

ModelFile::~ModelFile() noexcept {
    for (unsigned int i = 0; i < m_meshes.size(); i++) {
        delete m_meshes[i];
    }
}

bool operator==(const ModelFile& ModelFile, const char* const filename) {
    return strcmp(ModelFile.m_filename, filename) == 0;
}

bool operator==(const ModelFile& model1, const ModelFile& model2) {
    return model1 == model2.m_filename;
}

// Reads ModelFile files using assimp
void ModelFile::readModelFromFile(const char* filename) {
    // Assimp will do the work for us
    Assimp::Importer importer;

    // Read the file
    const aiScene* scene = importer.ReadFile(filename, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality);

    // Check for success
    if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
        std::cerr << "Error while loading the ModelFile with assimp: " << importer.GetErrorString() << std::endl;
        throw new std::exception();
    }

    // Notify user
    debugOutputEndl("Loading ModelFile from file...");
    // Process the materials
    processMaterials(scene, filename);
    // Process the nodes recursively
    processNodes(scene, scene->mRootNode);
}

void ModelFile::processMaterials(const aiScene* scene, const char* path) {
    // Get the amount of materials and make enough space for them in the vector to save time
    uint32 numMaterials = scene->mNumMaterials;
    m_materialIndices.reserve(numMaterials);

    // Process every material
    for (uint32 i = 0; i < numMaterials; i++) {
        // Material struct
        Material mat = { };
        // Material from assimp
        aiMaterial* aiMaterial = scene->mMaterials[i];

        // Load diffuse color
        aiColor3D diffuse(0.0f, 0.0f, 0.0f);
        if (AI_SUCCESS != aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse)) {
            // No diffuse color, so default color black will be used
            debugOutputEndl("No diffuse color, using default color black!");
        }
        mat.diffuse = { diffuse.r, diffuse.g, diffuse.b };

        // Load specular color
        aiColor3D specular(0.0f, 0.0f, 0.0f);
        if (AI_SUCCESS != aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular)) {
            // No specular color, so default color black will be used
            debugOutputEndl("No specular color, using default color black!");
        }
        mat.specular = { specular.r, specular.g, specular.b };

        // Load emissive color
        aiColor3D emissive(0.0f, 0.0f, 0.0f);
        if (AI_SUCCESS != aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissive)) {
            // No emissive color, so default color black will be used
            debugOutputEndl("No emissive color, using default color black!");
        }
        mat.emissive = { emissive.r, emissive.g, emissive.b };

        // Load shininess
        float shininess(0.0f);
        if (AI_SUCCESS != aiMaterial->Get(AI_MATKEY_SHININESS, shininess)) {
            // No shininess, so default value 0 will be used
            debugOutputEndl("No shininess, using default value 0!");
        }
        mat.shininess = shininess;

        // Load shininess strength and multiply specular by it
        float shininessStrength(1.0f);
        if (AI_SUCCESS != aiMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength)) {
            // No shininess strength, so default value 1 will be used
            debugOutputEndl("No shininess strength, using default value 1!");
        }
        mat.specular.x *= shininessStrength;
        mat.specular.y *= shininessStrength;
        mat.specular.z *= shininessStrength;

        // Get the number of diffuse and normal maps
        uint32_t numDiffuseMaps = aiMaterial->GetTextureCount(aiTextureType_DIFFUSE);
        uint32_t numNormalMaps = aiMaterial->GetTextureCount(aiTextureType_NORMALS);

        // Load the diffuse map name; in case none exists use the default one
        if (numDiffuseMaps > 0) {
            aiString diffuseMapNameBuffer{ };
            if (numDiffuseMaps > 1)
                debugOutputEndl("More than one diffuse texture present!");
            aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseMapNameBuffer);
            mat.diffuseMapName = std::string(getFilePath(path)) + diffuseMapNameBuffer.C_Str();
        } else {
            debugOutputEndl("No diffuse map, using default diffuse map!");
            mat.diffuseMapName = std::string("");
        }

        // Load the normal map name; in case none exists use the default one
        if (numNormalMaps > 0) {
            aiString normalMapNameBuffer{ };
            if (numNormalMaps > 1)
                debugOutputEndl("More than one normal map present!");
            aiMaterial->GetTexture(aiTextureType_NORMALS, 0, &normalMapNameBuffer);
            mat.normalMapName = std::string(std::string(getFilePath(path)) + normalMapNameBuffer.C_Str());
        } else {
            debugOutputEndl("No normal map, using default normal map!");
            mat.normalMapName = std::string("");
        }

        // Try to find an equal material already in the list of materials
        std::vector<Material>::iterator found = std::find(Material::materials.begin(), Material::materials.end(), mat);

        // If the material is already in the materials list
        if (found != Material::materials.end()) {
            // Add the material's index to the list
            m_materialIndices.push_back(found.base() - Material::materials.data());
            // Don't load textures again
            continue;
        }

        // Variables for stbi image loading
        int32 textureWidth = 0;
        int32 textureHeight = 0;
        int32 bitsPerPixel = 0;

        // Load the textures -------------------------------------------------------------------------------------
        // Make space for textures in VRAM
        glGenTextures(2, &mat.diffuseMap);
        // Flip texture to fit ModelFile
        stbi_set_flip_vertically_on_load(true);
        if (mat.diffuseMapName.compare("") != 0) {
            // Buffer to store texture
            auto textureBuffer = stbi_load(mat.diffuseMapName.data(), &textureWidth, &textureHeight, &bitsPerPixel, 4);
            // Check if texture loaded
            if (!textureBuffer) {
                std::cerr << "Couldn't load image at " << mat.diffuseMapName.data() << "! Aborting!" << std::endl;
                assert(textureBuffer);
            }
            assert(mat.diffuseMap);

            // Bind the diffuse texture
            glBindTexture(GL_TEXTURE_2D, mat.diffuseMap);

            // Settings for diffuse texture
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Load texture in VRAM
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);

            // Delete the texture from the buffer
            stbi_image_free(textureBuffer);
        }

        if (mat.normalMapName.compare("") != 0) {
            // Buffer to store texture
            auto textureBuffer = stbi_load(mat.normalMapName.data(), &textureWidth, &textureHeight, &bitsPerPixel, 4);
            // Check if texture loaded
            if (!textureBuffer) {
                std::cerr << "Couldn't load image at " << mat.normalMapName.data() << "! Aborting!" << std::endl;
                assert(textureBuffer);
            }
            assert(mat.normalMap);

            // Bind the normal map
            glBindTexture(GL_TEXTURE_2D, mat.normalMap);

            // Settings for normal map
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Load texture in VRAM
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);

            // Delete the texture from the buffer
            stbi_image_free(textureBuffer);
        }

        // Reset the bound texture
        glBindTexture(GL_TEXTURE_2D, 0);

        // Add the material to the material list
        Material::materials.push_back(mat);
        // Add the material's index to the list
        m_materialIndices.push_back(Material::materials.end().base() - Material::materials.data());
    }
}

void ModelFile::processNodes(const aiScene* scene, aiNode* node) {
    // Process the meshes from this node
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh);
    }

    // Process the next node recursively
    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        processNodes(scene, node->mChildren[i]);
    }
}

void ModelFile::processMesh(aiMesh* mesh) {
    // Index of the material, materials were loaded already
    uint64 localMaterialIndex = mesh->mMaterialIndex;
    uint64 numVertices = mesh->mNumVertices;
    // Dynamically allocated to ensure the variable lives after exiting function
    std::vector<Vertex>* vertices = new std::vector<Vertex>();

    // Dynamically allocated to ensure the variable lives after exiting function
    std::vector<uint32>* indices = new std::vector<uint32>();

    // Get the indices -------------------------------------------------------------------------------------------
    // Iterate over faces
    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // Add the indices to the list of indices
        for (uint32_t j = 0; j < face.mNumIndices; j++) {
            indices->push_back(face.mIndices[j]);
        }
    }

    // Get the vertices ------------------------------------------------------------------------------------------
    // Iterate over vertices
    for (uint64 i = 0; i < numVertices; i++) {
        // Create a vertex
        Vertex v = { };

        // Get the position
        v.position.x = mesh->mVertices[i].x;
        v.position.y = mesh->mVertices[i].y;
        v.position.z = mesh->mVertices[i].z;

        // Get the normal
        v.normal.x = mesh->mNormals[i].x;
        v.normal.y = mesh->mNormals[i].y;
        v.normal.z = mesh->mNormals[i].z;

        // Get the tangent
        v.tangent.x = mesh->mTangents[i].x;
        v.tangent.y = mesh->mTangents[i].y;
        v.tangent.z = mesh->mTangents[i].z;

        // Check for textures existing
        assert((*mesh->mNumUVComponents) > 0);
        // Get the texture coords
        v.textureCoords.x = mesh->mTextureCoords[0][i].x;
        v.textureCoords.y = mesh->mTextureCoords[0][i].y;

        // Add the vertex to the list of vertices
        vertices->push_back(v);
    }

    // Material index in Material::materials
    int globalMaterialIndex = m_materialIndices[localMaterialIndex];

    // Add the mesh to the list of meshes
    m_meshes.push_back(new Mesh(vertices, indices, globalMaterialIndex));
}