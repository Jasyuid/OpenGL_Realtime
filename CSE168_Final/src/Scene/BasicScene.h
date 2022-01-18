#pragma once

#include "Scene.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"

class BasicScene : public Scene
{
public:
	BasicScene();
	~BasicScene();

	void OnEvent(Event& e) override;
	void OnUpdate(float dv) override;
private:
	VertexArray* cube_VAO;
	VertexBuffer* cube_VBO;
	IndexBuffer* cube_IBO;

	Shader* basicShader;

	glm::mat4 cubeModel;

};