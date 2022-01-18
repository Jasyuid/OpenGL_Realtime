#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class Model
{
public:
	Model(Mesh* mesh);
	Model(std::string filename, bool flip = false);
	Model(std::string filename, TextureMap tmap);
	~Model();

	void Translate(glm::vec3 move);
	void Rotate(float angle, glm::vec3 axis);
	void Scale(glm::vec3 scale);

	void Draw(Shader* shader);

	inline glm::vec3 GetPosition() const { return m_Position; }

private:

	void LoadModel(std::string filename);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Texture* LoadTexture(aiMaterial* mat, aiTextureType type);

private:
	std::vector<Mesh*> m_Meshes;

	std::string m_Directory;
	std::map<std::string, Texture*> m_LoadedTextures;
	TextureMap m_tmap;

	glm::mat4 m_ModelMat;
	glm::mat4 m_TranMat;
	glm::mat4 m_ScaleMat;
	glm::mat4 m_RotMat;

	glm::vec3 m_Position;

};