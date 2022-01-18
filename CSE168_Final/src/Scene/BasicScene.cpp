#include "oglpch.h"
#include "BasicScene.h"

#include "Application.h"
#include "Core/MouseEvent.h"
#include "Core/Input.h"

#include <glad/glad.h>

const float cube_vertices[] = {
		-1.0f,-1.0f,-1.0f,		0.0f, 0.0f, 0.0f, //0
		 1.0f,-1.0f,-1.0f,		1.0f, 0.0f, 0.0f, //1
		-1.0f, 1.0f,-1.0f,		0.0f, 1.0f, 0.0f, //2
		 1.0f, 1.0f,-1.0f,		1.0f, 1.0f, 0.0f, //3
		-1.0f,-1.0f, 1.0f,		0.0f, 0.0f, 1.0f, //4
		 1.0f,-1.0f, 1.0f,		1.0f, 0.0f, 1.0f, //5
		-1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, //6
		 1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f  //7
};
const unsigned int cube_indices[] = {
		2, 1, 0, //Front-1
		1, 2, 3, //Front-2
		6, 0, 4, //Left-1
		0, 6, 2, //Left-2
		3, 5, 1, //Right-1
		5, 3, 7, //Right-2
		0, 5, 4, //Bottom-1
		5, 0, 1, //Bottom-2
		6, 3, 2, //Top-1
		3, 6, 7, //Top-2
		4, 5, 6, //Back-1
		7, 6, 5  //Back-2
};

BasicScene::BasicScene()
	: Scene()
{
	m_Camera = new Camera(glm::vec3(1.5f, 1.5f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	cube_VBO = new VertexBuffer(cube_vertices, 8 * 6 * sizeof(float));
	cube_IBO = new IndexBuffer(cube_indices, 12 * 3);

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	cube_VAO = new VertexArray();
	cube_VAO->AddBuffer(*cube_VBO, layout);

	cubeModel = glm::mat4(1.0f);
	cubeModel = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f)) * cubeModel;
	cubeModel = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)) * cubeModel;

	basicShader = new Shader("res/shaders/Basic.glsl");
	basicShader->Bind();
	basicShader->SetUniformMat4("model", cubeModel);
	basicShader->SetUniformMat4("camera", m_Camera->GetCameraMat());
}

BasicScene::~BasicScene()
{
	delete cube_VAO;
	delete cube_VBO;
	delete cube_IBO;

	delete basicShader;
}

void BasicScene::OnEvent(Event& e)
{
	m_Camera->OnEvent(e);
}

void BasicScene::OnUpdate(float dt)
{
	basicShader->Bind();
	basicShader->SetUniformMat4("model", cubeModel);
	basicShader->SetUniformMat4("camera", m_Camera->GetCameraMat());

	cube_VAO->Bind();
	cube_IBO->Bind();
	glDrawElements(GL_TRIANGLES, cube_IBO->GetCount(), GL_UNSIGNED_INT, nullptr);

}