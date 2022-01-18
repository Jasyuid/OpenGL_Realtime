#pragma once

#include "Scene.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Model.h"

class CheckpointScene : public Scene
{
public:
	CheckpointScene();
	~CheckpointScene();

	void OnEvent(Event& e) override;
	void OnUpdate(float dv) override;
private:
	Model* model;
	Shader* modelShader;

	Model* ground;
	Model* wall;
	Shader* depthShader;

	unsigned int depth_map_buffer, depth_map_buffer_2;
	unsigned int depth_cubemap, depth_cubemap_2;
	const unsigned int SHADOW_SIZE = 256;

	Texture* hdr_map;
	unsigned int capture_fb, capture_rb;
	unsigned int env_map;
	Shader* convertShader;

	Model* cubeModel;
	Shader* skyboxShader;

	glm::vec3 light1_col, light2_col;
	Model* light1;
	Model* light2;
	Shader* lightShader;

};