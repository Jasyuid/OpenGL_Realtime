#include "oglpch.h"
#include "StingScene.h"

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

StingScene::StingScene()
	: Scene()
{
	m_Camera = new Camera(glm::vec3(1.5f, 1.5f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	/*
	//model = new Model("res/models/crate/wooden_crate.obj");
	//model->Scale(glm::vec3(1.0f));
	*/

	/*
	TextureMap gun_textures;
	gun_textures.emplace("texture_diffuse", new Texture("res/models/gun/Cerberus_A.png"));
	gun_textures.emplace("texture_specular", new Texture("res/models/gun/Cerberus_M.png"));
	gun_textures.emplace("texture_normal", new Texture("res/models/gun/Cerberus_N.png"));
	gun_textures.emplace("texture_roughness", new Texture("res/models/gun/Cerberus_R.png"));
	model = new Model("res/models/gun/Cerberus_LP.obj", gun_textures);
	model->Scale(glm::vec3(0.03f));
	model->Translate(glm::vec3(0.0f, 0.0f, 0.0f));
	*/

	/*
	TextureMap model_textures;
	model_textures.emplace("texture_diffuse", new Texture("res/textures/albedo.png"));
	model_textures.emplace("texture_specular", new Texture("res/textures/metallic.png"));
	model_textures.emplace("texture_normal", new Texture("res/textures/normal.png"));
	model_textures.emplace("texture_roughness", new Texture("res/textures/roughness.png"));
	model = new Model("res/models/sphere.obj", model_textures);
	model->Scale(glm::vec3(0.01f));
	model->Translate(glm::vec3(0.0f, 0.0f, 0.0f));
	*/

	
	TextureMap model_textures;
	model_textures.emplace("texture_diffuse", new Texture("res/models/sting/Sting_Base_Color.png"));
	model_textures.emplace("texture_specular", new Texture("res/models/sting/Sting_Metallic.png"));
	model_textures.emplace("texture_normal", new Texture("res/models/sting/Sting_Normal.png"));
	model_textures.emplace("texture_roughness", new Texture("res/models/sting/Sting_Roughness.png"));
	model = new Model("res/models/sting/Sting-Sword-lowpoly.obj", model_textures);
	model->Scale(glm::vec3(0.3f));
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
	textures.emplace("texture_specular", black_text);
	textures.emplace("texture_normal", normal_text);
	textures.emplace("texture_roughness", black_text);
	Mesh* ground_mesh = new Mesh(cube_face_vertices, 24 * 8, layout, cube_face_indices, 36, textures);
	ground = new Model(ground_mesh);
	ground->Scale(glm::vec3(20.0f, 0.1f, 20.0f));
	ground->Translate(glm::vec3(0.0f, -1.7, 0.0f));

	wall = new Model(ground_mesh);
	wall->Scale(glm::vec3(0.1f, 50.0f, 100.0f));
	wall->Translate(glm::vec3(5.0f, 48.3f, 0.0f));
	//wall->Rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	light1 = new Model("res/models/sphere.obj");
	light1->Scale(glm::vec3(0.0005f));
	//light1->Translate(glm::vec3(-4.0f, 1.0f, 3.0f));
	light1->Translate(glm::vec3(30.0f, 30.0f, 30.0f));
	//light1_col = glm::vec3(0.3f, 0.3f, 1.0f);
	light1_col = glm::vec3(1.0f, 1.0f, 1.0f);

	modelShader = new Shader("res/shaders/ShadowEnv.glsl");
	//modelShader = new Shader("res/shaders/ModelShader.glsl");
	lightShader = new Shader("res/shaders/BasicLight.glsl");
	depthShader = new Shader("res/shaders/BasicDepth.glsl");
	convertShader = new Shader("res/shaders/CubemapConverter.glsl");
	skyboxShader = new Shader("res/shaders/Skybox.glsl");

	glGenFramebuffers(1, &depth_map_buffer);

	/*
	glGenTextures(1, &depth_map);
	glBindTexture(GL_TEXTURE_2D, depth_map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_SIZE, SHADOW_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	*/

	/*
	glBindFramebuffer(GL_FRAMEBUFFER, depth_map_buffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	*/

	glGenTextures(1, &depth_cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depth_cubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_SIZE, SHADOW_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, depth_map_buffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_cubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(1, &depth_map_buffer_2);



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
	//hdr_map = new Texture("res/textures/kloppenheim_02_1k.hdr", true, true);
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

StingScene::~StingScene()
{
	delete model;
	delete modelShader;

	delete light1;
	delete lightShader;
}

void StingScene::OnEvent(Event& e)
{
	m_Camera->OnEvent(e);

	if (e.GetEventType() == EventType::KeyPressed)
	{
		if (((KeyPressedEvent&)e).GetKeyCode() == GLFW_KEY_R)
		{
			m_Rotate = !m_Rotate;
		}
	}
}

void StingScene::OnUpdate(float dt)
{
	if (m_Rotate)
		model->Rotate(0.1f, glm::vec3(0.0f, 1.0f, 0.0f));

	if (Input::IsKeyPressed(GLFW_KEY_A))
		model->Translate(glm::vec3(0.0f, 0.0f, 3.0f * dt));
	if (Input::IsKeyPressed(GLFW_KEY_D))
		model->Translate(glm::vec3(0.0f, 0.0f, -3.0f * dt));


	glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);
	glBindFramebuffer(GL_FRAMEBUFFER, depth_map_buffer);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 light_proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10000.0f);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(light_proj * glm::lookAt(light1->GetPosition(), light1->GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(light_proj * glm::lookAt(light1->GetPosition(), light1->GetPosition() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(light_proj * glm::lookAt(light1->GetPosition(), light1->GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(light_proj * glm::lookAt(light1->GetPosition(), light1->GetPosition() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(light_proj * glm::lookAt(light1->GetPosition(), light1->GetPosition() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(light_proj * glm::lookAt(light1->GetPosition(), light1->GetPosition() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	depthShader->Bind();
	depthShader->SetUniformMat4("shadow_matrices[0]", shadowTransforms[0]);
	depthShader->SetUniformMat4("shadow_matrices[1]", shadowTransforms[1]);
	depthShader->SetUniformMat4("shadow_matrices[2]", shadowTransforms[2]);
	depthShader->SetUniformMat4("shadow_matrices[3]", shadowTransforms[3]);
	depthShader->SetUniformMat4("shadow_matrices[4]", shadowTransforms[4]);
	depthShader->SetUniformMat4("shadow_matrices[5]", shadowTransforms[5]);
	depthShader->SetUniformFloat3("light_pos", light1->GetPosition());
	depthShader->SetUniformFloat("far_plane", 10000.0f);

	glCullFace(GL_FRONT);

	//wall->Draw(depthShader);
	ground->Draw(depthShader);
	model->Draw(depthShader);

	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	unsigned int width = Application::Get().GetWindow().GetWidth();
	unsigned int height = Application::Get().GetWindow().GetHeight();
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0 + 5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depth_cubemap);

	glActiveTexture(GL_TEXTURE0 + 7);
	glBindTexture(GL_TEXTURE_CUBE_MAP, env_map);

	lightShader->Bind();
	lightShader->SetUniformMat4("camera", m_Camera->GetCameraMat());
	lightShader->SetUniformFloat3("light_color", light1_col);

	light1->Draw(lightShader);


	modelShader->Bind();
	modelShader->SetUniformMat4("camera", m_Camera->GetCameraMat());
	modelShader->SetUniformFloat3("view_pos", m_Camera->GetPosition());
	modelShader->SetUniformInt("light_num", 1);
	modelShader->SetUniformFloat3("light[0].position", light1->GetPosition());
	modelShader->SetUniformFloat3("light[0].color", light1_col);
	//modelShader->SetUniformFloat3("light[1].position", light2->GetPosition());
	//modelShader->SetUniformFloat3("light[1].color", light2_col);
	modelShader->SetUniformInt("shadow_maps[0]", 5);
	//modelShader->SetUniformInt("shadow_maps[1]", 6);
	modelShader->SetUniformFloat("far_plane", 10000.0f);
	modelShader->SetUniformInt("env_map", 7);

	modelShader->SetUniformFloat("material.rough_add", 0.0f);
	//wall->Draw(modelShader);
	ground->Draw(modelShader);
	modelShader->SetUniformFloat("material.rough_add", 0.5f);
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