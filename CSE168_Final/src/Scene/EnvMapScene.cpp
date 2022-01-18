#include "oglpch.h"
#include "EnvMapScene.h"

#include "Application.h"
#include "Core/MouseEvent.h"
#include "Core/KeyEvent.h"
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
const float cube_face_vertices[] = {
		-1.0f,-1.0f,-1.0f,		0.0f, 0.0f, -1.0f,		0.0f, 0.0f, //0
		 1.0f,-1.0f,-1.0f,		0.0f, 0.0f, -1.0f,		1.0f, 0.0f, //1
		-1.0f, 1.0f,-1.0f,		0.0f, 0.0f, -1.0f,		0.0f, 1.0f, //2
		 1.0f, 1.0f,-1.0f,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f, //3

		-1.0f,-1.0f, 1.0f,	   -1.0f, 0.0f, 0.0f,		0.0f, 1.0f, //4
		-1.0f,-1.0f,-1.0f,	   -1.0f, 0.0f, 0.0f,		0.0f, 0.0f, //0
		-1.0f, 1.0f, 1.0f,	   -1.0f, 0.0f, 0.0f,		1.0f, 1.0f, //6
		-1.0f, 1.0f,-1.0f,	   -1.0f, 0.0f, 0.0f,		1.0f, 0.0f, //2

		 1.0f,-1.0f,-1.0f,	    1.0f, 0.0f, 0.0f,		0.0f, 0.0f, //1
		 1.0f,-1.0f, 1.0f,		1.0f, 0.0f, 0.0f,		0.0f, 1.0f, //5
		 1.0f, 1.0f,-1.0f,		1.0f, 0.0f, 0.0f,		1.0f, 0.0f, //3
		 1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f, //7

		-1.0f,-1.0f, 1.0f,		0.0f,-1.0f, 0.0f,		0.0f, 1.0f, //4
		 1.0f,-1.0f, 1.0f,		0.0f,-1.0f, 0.0f,		1.0f, 1.0f, //5
		-1.0f,-1.0f,-1.0f,		0.0f,-1.0f, 0.0f,		0.0f, 0.0f, //0
		 1.0f,-1.0f,-1.0f,		0.0f,-1.0f, 0.0f,		1.0f, 0.0f, //1

		-1.0f, 1.0f,-1.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f, //2
		 1.0f, 1.0f,-1.0f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f, //3
		-1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f, //6
		 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,		1.0f, 1.0f, //7 

		-1.0f,-1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f, //4
		 1.0f,-1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, //5
		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f, //6
		 1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 		1.0f, 1.0f //7 
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
const unsigned int cube_face_indices[] = {
		2, 1, 0, //Front-1
		1, 2, 3, //Front-2
		6, 5, 4, //Left-1
		5, 6, 7, //Left-2
		10, 9, 8, //Right-1
		9, 10, 11, //Right-2
		14, 13, 12, //Bottom-1
		13, 14, 15, //Bottom-2
		18, 17, 16, //Top-1
		17, 18, 19, //Top-2
		20, 21, 22, //Back-1
		23, 22, 21  //Back-2

};

EnvMapScene::EnvMapScene()
	: Scene()
{
	m_Camera = new Camera(glm::vec3(1.5f, 1.5f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	TextureMap model_textures;
	model_textures.emplace("texture_diffuse", new Texture("res/textures/abstract.jpg"));
	model_textures.emplace("texture_specular", new Texture("res/models/crate/crate_Metallic.png"));
	model_textures.emplace("texture_normal", new Texture("res/textures/basic_normal.jpg"));
	model_textures.emplace("texture_roughness", new Texture("res/models/crate/crate_Metallic.png"));
	model = new Model("res/models/sphere.obj", model_textures);
	model->Scale(glm::vec3(0.01f));
	model->Translate(glm::vec3(0.0f, 0.0f, 0.0f));


	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	TextureMap textures;
	Texture* ground_text = new Texture("res/textures/gray.jpg");
	Texture* black_text = new Texture("res/models/crate/crate_Metallic.png");
	Texture* normal_text = new Texture("res/textures/blank_normal.jpg");
	textures.emplace("texture_diffuse", ground_text);
	textures.emplace("texture_specular", ground_text);
	textures.emplace("texture_normal", normal_text);
	textures.emplace("texture_roughness", black_text);
	Mesh* ground_mesh = new Mesh(cube_face_vertices, 24 * 8, layout, cube_face_indices, 36, textures);
	ground = new Model(ground_mesh);
	ground->Scale(glm::vec3(100.0f, 0.1f, 100.0f));
	ground->Translate(glm::vec3(0.0f, -1.7, 0.0f));

	wall = new Model(ground_mesh);
	wall->Scale(glm::vec3(0.1f, 50.0f, 100.0f));
	wall->Translate(glm::vec3(5.0f, 48.3f, 0.0f));
	//wall->Rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	light1 = new Model("res/models/sphere.obj");
	light1->Scale(glm::vec3(0.001f));
	light1->Translate(glm::vec3(10.0f, 10.0f, 10.0f));
	light1_col = glm::vec3(0.3f, 0.3, 1.0f);

	modelShader = new Shader("res/shaders/BasicEnvMap.glsl");
	lightShader = new Shader("res/shaders/BasicLight.glsl");
	convertShader = new Shader("res/shaders/CubemapConverter.glsl");
	skyboxShader = new Shader("res/shaders/Skybox.glsl");



	glGenFramebuffers(1, &capture_fb);
	glGenRenderbuffers(1, &capture_rb);

	glBindFramebuffer(GL_FRAMEBUFFER, capture_fb);
	glBindRenderbuffer(GL_RENDERBUFFER, capture_rb);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, capture_rb);


	glGenTextures(1, &env_map);
	glBindTexture(GL_TEXTURE_CUBE_MAP, env_map);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	hdr_map = new Texture("res/textures/parched_canal_1k.hdr", true, true);
	Mesh* cube_mesh = new Mesh(cube_face_vertices, 24 * 8, layout, cube_face_indices, 36, textures);
	cubeModel = new Model(cube_mesh);

	glm::mat4 capture_proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 capture_views[] =
	{
		glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
	};

	hdr_map->Bind(6);

	convertShader->Bind();
	convertShader->SetUniformInt("rectMap", 6);

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, capture_fb);
	for (unsigned int i = 0; i < 6; ++i)
	{
		convertShader->SetUniformMat4("camera", capture_proj * capture_views[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env_map, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cubeModel->Draw(convertShader);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

EnvMapScene::~EnvMapScene()
{
	delete model;
	delete modelShader;

	delete light1;
}

void EnvMapScene::OnEvent(Event& e)
{
	m_Camera->OnEvent(e);

	if (e.GetEventType() == EventType::KeyPressed)
	{
		if (((KeyPressedEvent&)e).GetKeyCode() == GLFW_KEY_N)
		{
			//m_Normal = !m_Normal;
		}
	}
}

void EnvMapScene::OnUpdate(float dt)
{
	//model->Rotate(0.1f, glm::vec3(0.0f, 1.0f, 0.0f));

	if (Input::IsKeyPressed(GLFW_KEY_A))
		model->Translate(glm::vec3(0.0f, 0.0f, 3.0f * dt));
	if (Input::IsKeyPressed(GLFW_KEY_D))
		model->Translate(glm::vec3(0.0f, 0.0f, -3.0f * dt));



	unsigned int width = Application::Get().GetWindow().GetWidth();
	unsigned int height = Application::Get().GetWindow().GetHeight();
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	lightShader->Bind();
	lightShader->SetUniformMat4("camera", m_Camera->GetCameraMat());
	lightShader->SetUniformFloat3("light_color", light1_col);

	light1->Draw(lightShader);
	

	glActiveTexture(GL_TEXTURE0 + 7);
	glBindTexture(GL_TEXTURE_CUBE_MAP, env_map);

	modelShader->Bind();
	modelShader->SetUniformMat4("camera", m_Camera->GetCameraMat());
	modelShader->SetUniformFloat3("view_pos", m_Camera->GetPosition());
	modelShader->SetUniformInt("env_map", 7);

	modelShader->SetUniformFloat("material.rough_add", 0.0f);
	model->Draw(modelShader);

	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_FRONT);

	skyboxShader->Bind();
	skyboxShader->SetUniformMat4("view", m_Camera->GetViewMat());
	skyboxShader->SetUniformMat4("proj", m_Camera->GetProjMat());
	skyboxShader->SetUniformInt("env_map", 7);

	cubeModel->Draw(skyboxShader);

	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
}