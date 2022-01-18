#include "oglpch.h"
#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Model::Model(Mesh* mesh) :
	m_ModelMat(glm::mat4(1.0)), m_TranMat(glm::mat4(1.0)), m_RotMat(glm::mat4(1.0)), m_ScaleMat(glm::mat4(1.0)), m_Position(glm::vec3(0.0f))
{
	m_Meshes.push_back(mesh);
}

Model::Model(std::string filename, bool flip) :
	m_ModelMat(glm::mat4(1.0)), m_TranMat(glm::mat4(1.0)), m_RotMat(glm::mat4(1.0)), m_ScaleMat(glm::mat4(1.0)), m_Position(glm::vec3(0.0f))
{
	m_Directory = filename.substr(0, filename.find_last_of('/')) + "/";

	LoadModel(filename);
}

Model::Model(std::string filename, TextureMap tmap) :
	m_ModelMat(glm::mat4(1.0)), m_TranMat(glm::mat4(1.0)), m_RotMat(glm::mat4(1.0)), m_ScaleMat(glm::mat4(1.0)), m_Position(glm::vec3(0.0f))
{
	m_Directory = filename.substr(0, filename.find_last_of('/')) + "/";
	m_tmap = tmap;

	LoadModel(filename);
}

Model::~Model()
{
	for (Mesh* m : m_Meshes)
	{
		delete m;
	}

	for (const std::pair<std::string, Texture*> entry : m_LoadedTextures)
	{
		delete entry.second;
	}
}

void Model::Translate(glm::vec3 move)
{
	glm::mat4 t = glm::translate(move);
	m_TranMat = t * m_TranMat;

	m_Position += move;
}

void Model::Rotate(float angle, glm::vec3 axis)
{
	glm::mat4 r = glm::rotate(glm::radians(angle), axis);
	m_RotMat = r * m_RotMat;
}

void Model::Scale(glm::vec3 scale)
{
	glm::mat4 s = glm::scale(scale);
	m_ScaleMat = s * m_ScaleMat;
}

void Model::Draw(Shader* shader)
{
	m_ModelMat = m_TranMat * m_RotMat * m_ScaleMat;
	shader->Bind();
	shader->SetUniformMat4("model", m_ModelMat);

	for (Mesh* m : m_Meshes)
	{
		m->Draw(shader);
	}
}

void Model::LoadModel(std::string filename)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_ERROR("Failed to load model!");
		return;
	}

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		std::vector<float> vertices;
		std::vector<unsigned int> indicies;
		TextureMap textures;
		// Load vertex data
		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
		{
			vertices.push_back((float)mesh->mVertices[j].x);
			vertices.push_back((float)mesh->mVertices[j].y);
			vertices.push_back((float)mesh->mVertices[j].z);

			vertices.push_back((float)mesh->mNormals[j].x);
			vertices.push_back((float)mesh->mNormals[j].y);
			vertices.push_back((float)mesh->mNormals[j].z);

			if (mesh->mTextureCoords[0])
			{
				vertices.push_back((float)mesh->mTextureCoords[0][j].x);
				vertices.push_back((float)mesh->mTextureCoords[0][j].y);
			}
			else
			{
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
			}
		}

		// Load index data
		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			aiFace face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; k++)
			{
				indicies.push_back(face.mIndices[k]);
			}
		}

		// Doesn't work for most models, doing it manually for now

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);

		if (m_tmap.size() > 0)
		{
			m_Meshes.push_back(new Mesh(vertices.data(), vertices.size(), layout, indicies.data(), indicies.size(), m_tmap));
		}
		else
		{
			if (mesh->mMaterialIndex >= 0)
			{
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

				Texture* diffuse = LoadTexture(material, aiTextureType_DIFFUSE);
				if (diffuse)
					textures.emplace("texture_diffuse", diffuse);
				Texture* specular = LoadTexture(material, aiTextureType_SPECULAR);
				if (specular)
					textures.emplace("texture_specular", specular);
				Texture* normal = LoadTexture(material, aiTextureType_HEIGHT);
				if (normal)
					textures.emplace("texture_normal", normal);

			}
			m_Meshes.push_back(new Mesh(vertices.data(), vertices.size(), layout, indicies.data(), indicies.size(), textures));
		}	
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Texture* Model::LoadTexture(aiMaterial* mat, aiTextureType type)
{
	Texture* texture;
	// TODO: Add support for multiple textures of one type per mesh
	if (mat->GetTextureCount(type) > 0)
	{
		aiString str;
		mat->GetTexture(type, 0, &str);
		std::string file = str.C_Str();
		for (const std::pair<std::string, Texture*> entry : m_LoadedTextures)
		{
			if (entry.first == file)
			{
				texture = entry.second;
				return texture;
			}
		}
		std::string path = m_Directory + str.C_Str();
		if (type == aiTextureType_DIFFUSE)
			texture = new Texture(path, false, true);
		else
			texture = new Texture(path);

		LOG_TRACE("Loaded texture '{0}'", str.C_Str());
		m_LoadedTextures.emplace(file, texture);
		return texture;
	}

	return nullptr;
}