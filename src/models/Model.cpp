/*
 * Model.cpp
 *
 *  Created on: Aug 17, 2021
 *      Author: jan
 */

#include "Model.h"

// Ignore "member should be initialized in initializer list" warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
Model::Model(const char *filename, Shader *shader) {
	if (strcmp(getFileExtension(filename), "bmf") == 0)
		readModelFromBmfFile(filename, shader, glm::vec3(0.0f, 0.0f, 0.0f));
	else
		readModelFromFile(filename, shader, glm::vec3(0.0f, 0.0f, 0.0f));
}

Model::Model(const char *filename, Shader *shader, glm::vec3 offset) {
	if (strcmp(getFileExtension(filename), "bmf") == 0)
		readModelFromBmfFile(filename, shader, offset);
	else
		readModelFromFile(filename, shader, offset);
}
#pragma GCC diagnostic pop

// Reads bmf files
void Model::readModelFromBmfFile(const char *filename, Shader *shader, glm::vec3 offset) {
	std::ifstream input = std::ifstream(filename, std::ios::in | std::ios::binary);
	if (!input.is_open()) {
		std::cout << "Error reading model file " << filename << "!" << std::endl;
		throw std::exception();
	}

	// Materials
	uint64 numMaterials = 0;
	input.read((char*) &numMaterials, sizeof(numMaterials));
	m_materials.reserve(numMaterials);

	for (uint64 i = 0; i < numMaterials; i++) {
		Material mat = { };
		input.read((char*) &mat, sizeof(BMFMaterial));

		int64 diffuseMapNameLength = 0;
		input.read((char*) &diffuseMapNameLength, sizeof(diffuseMapNameLength));
		std::string diffuseMapName(diffuseMapNameLength, '\0');
		input.read(diffuseMapName.data(), diffuseMapNameLength);

		int64 normalMapNameLength = 0;
		input.read((char*) &normalMapNameLength, sizeof(normalMapNameLength));
		std::string normalMapName(normalMapNameLength, '\0');
		input.read(normalMapName.data(), normalMapNameLength);

		assert(diffuseMapNameLength > 0);
		assert(normalMapNameLength > 0);

		int32 textureWidth = 0;
		int32 textureHeight = 0;
		int32 bitsPerPixel = 0;

		glGenTextures(2, &mat.diffuseMap);
		stbi_set_flip_vertically_on_load(true);
		{
			auto textureBuffer = stbi_load(diffuseMapName.c_str(), &textureWidth, &textureHeight, &bitsPerPixel, 4);
			if (!textureBuffer) {
				std::cout << "Couldn't load image at " << diffuseMapName << "! Aborting!" << std::endl;
				assert(textureBuffer);
			}
			assert(mat.diffuseMap);

			glBindTexture(GL_TEXTURE_2D, mat.diffuseMap);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);

			if (textureBuffer) {
				stbi_image_free(textureBuffer);
			}
		}

		{
			auto textureBuffer = stbi_load(normalMapName.c_str(), &textureWidth, &textureHeight, &bitsPerPixel, 4);
			assert(textureBuffer);
			assert(mat.normalMap);

			glBindTexture(GL_TEXTURE_2D, mat.normalMap);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);

			if (textureBuffer) {
				stbi_image_free(textureBuffer);
			}
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		m_materials.push_back(mat);
	}

	// Meshes
	uint64 numMeshes = 0;
	input.read((char*) &numMeshes, sizeof(numMeshes));
	m_meshes.reserve(numMeshes);

	for (uint64 i = 0; i < numMeshes; i++) {
		// Index of the material, materials were loaded already
		uint64 materialIndex = 0;
		uint64 numVertices = 0;
		// Dynamically allocated to ensure the variable lives after exiting function
		std::vector<Vertex> *vertices = new std::vector<Vertex>();

		uint64 numIndices = 0;
		// Dynamically allocated to ensure the variable lives after exiting function
		std::vector<uint32> *indices = new std::vector<uint32>();

		input.read((char*) &materialIndex, sizeof(materialIndex));
		input.read((char*) &numVertices, sizeof(numVertices));
		input.read((char*) &numIndices, sizeof(numIndices));

		for (uint64 j = 0; j < numVertices; j++) {
			Vertex vertex;
			input.read((char*) &vertex.position.x, sizeof(float));
			input.read((char*) &vertex.position.y, sizeof(float));
			input.read((char*) &vertex.position.z, sizeof(float));

			input.read((char*) &vertex.normal.x, sizeof(float));
			input.read((char*) &vertex.normal.y, sizeof(float));
			input.read((char*) &vertex.normal.z, sizeof(float));

			input.read((char*) &vertex.tangent.x, sizeof(float));
			input.read((char*) &vertex.tangent.y, sizeof(float));
			input.read((char*) &vertex.tangent.z, sizeof(float));

			input.read((char*) &vertex.textureCoords.x, sizeof(float));
			input.read((char*) &vertex.textureCoords.y, sizeof(float));

			vertex.position.x += offset.x;
			vertex.position.y += offset.y;
			vertex.position.z += offset.z;

			vertices->push_back(vertex);
		}
		for (uint64 j = 0; j < numIndices; j++) {
			uint32 index;
			input.read((char*) &index, sizeof(uint32));
			indices->push_back(index);
		}

		Mesh *mesh = new Mesh(vertices, indices, m_materials[materialIndex], shader);

		m_meshes.push_back(mesh);
	}

	input.close();
}

// Reads model files using assimp
void Model::readModelFromFile(const char *filename, Shader *shader, glm::vec3 offset) {
	// Assimp will do the work for us
	Assimp::Importer importer;

	// Read the file
	m_scene = importer.ReadFile(filename, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality);

	// Check for success
	if (!m_scene || (m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !m_scene->mRootNode) {
		std::cout << "Error while loading the model with assimp: " << importer.GetErrorString() << std::endl;
		throw new std::exception();
	}

	// Notify user
	debugOutputEndl("Loading model from file...");
	// Process the materials
	processMaterials(filename);
	// Process the nodes recursively
	processNodes(m_scene->mRootNode, shader);
}

void Model::processMaterials(const char *path) {
	// Get the amount of materials and make enough space for them in the vector to save time
	uint32 numMaterials = m_scene->mNumMaterials;
	m_materials.reserve(numMaterials);

	// Process every material
	for (uint32 i = 0; i < numMaterials; i++) {
		// Material struct
		Material mat = { };
		// Material from assimp
		aiMaterial *aiMaterial = m_scene->mMaterials[i];

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
			aiString diffuseMapNameBuffer { };
			if (numDiffuseMaps > 1)
				debugOutputEndl("More than one diffuse texture present!");
			aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseMapNameBuffer);
			mat.diffuseMapName = std::string(getFilePath(path)) + diffuseMapNameBuffer.C_Str();
		} else {
			debugOutputEndl("Using default diffuse map!");
			mat.diffuseMapName = std::string("assets/models/default/diffuse.tga");
		}

		// Load the normal map name; in case none exists use the default one
		if (numNormalMaps > 0) {
			aiString normalMapNameBuffer { };
			if (numNormalMaps > 1)
				debugOutputEndl("More than one normal map present!");
			aiMaterial->GetTexture(aiTextureType_NORMALS, 0, &normalMapNameBuffer);
			mat.normalMapName = std::string(std::string(getFilePath(path)) + normalMapNameBuffer.C_Str());
		} else {
			debugOutputEndl("Using default normal map!");
			mat.normalMapName = std::string("assets/models/default/normal.tga");
		}

		// Variables for stbi image loading
		int32 textureWidth = 0;
		int32 textureHeight = 0;
		int32 bitsPerPixel = 0;

		// Load the textures -------------------------------------------------------------------------------------
		// Make space for textures in VRAM
		glGenTextures(2, &mat.diffuseMap);
		// Flip texture to fit model
		stbi_set_flip_vertically_on_load(true);
		{
			// Buffer to store texture
			auto textureBuffer = stbi_load(mat.diffuseMapName.data(), &textureWidth, &textureHeight, &bitsPerPixel, 4);
			// Check if texture loaded
			if (!textureBuffer) {
				std::cout << "Couldn't load image at " << mat.diffuseMapName.data() << "! Aborting!" << std::endl;
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

		{
			// Buffer to store texture
			auto textureBuffer = stbi_load(mat.normalMapName.data(), &textureWidth, &textureHeight, &bitsPerPixel, 4);
			// Check if texture loaded
			if (!textureBuffer) {
				std::cout << "Couldn't load image at " << mat.diffuseMapName.data() << "! Aborting!" << std::endl;
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
		m_materials.push_back(mat);
	}
}

void Model::processNodes(aiNode *node, Shader *shader) {
	// Process the meshes from this node
	for (uint32_t i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = m_scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, shader);
	}

	// Process the next node recursively
	for (uint32_t i = 0; i < node->mNumChildren; i++) {
		processNodes(node->mChildren[i], shader);
	}
}

void Model::processMesh(aiMesh *mesh, Shader *shader) {
	// Index of the material, materials were loaded already
	uint64 materialIndex = mesh->mMaterialIndex;
	uint64 numVertices = mesh->mNumVertices;
	// Dynamically allocated to ensure the variable lives after exiting function
	std::vector<Vertex> *vertices = new std::vector<Vertex>();

	// Dynamically allocated to ensure the variable lives after exiting function
	std::vector<uint32> *indices = new std::vector<uint32>();

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

	// Add the mesh to the list of meshes
	m_meshes.push_back(new Mesh(vertices, indices, m_materials[materialIndex], shader));
}

Model::~Model() {
	for (unsigned int i = 0; i < m_meshes.size(); i++) {
		delete m_meshes[i];
	}
	delete m_scene;
}

void Model::render() {
	for (unsigned int i = 0; i < m_meshes.size(); i++) {
		m_meshes[i]->render();
	}
}
