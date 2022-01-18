#pragma once

#include "Scene.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Model.h"

class EnvMapScene : public Scene
{
public:
	EnvMapScene();
	~EnvMapScene();

	void OnEvent(Event& e) override;
	void OnUpdate(float dv) override;
private:
	Model* model;
	Shader* modelShader;

	Model* ground;
	Model* wall;

	Texture* hdr_map;
	unsigned int capture_fb, capture_rb;
	unsigned int env_map;
	Shader* convertShader;

	Model* cubeModel;
	Shader* skyboxShader;

	glm::vec3 light1_col;
	Model* light1;
	Shader* lightShader;

};