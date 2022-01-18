#include "oglpch.h"
#include "Application.h"

#include "Core/KeyEvent.h"

#include <glm/glm.hpp>

#include "Scene/BasicScene.h"
#include "Scene/ShadowScene.h"
#include "Scene/EnvMapScene.h"
#include "Scene/ShadowEnvScene.h"
#include "Scene/StingScene.h"
#include "Scene/GunScene.h"
#include "Scene/CheckpointScene.h"

Application* Application::s_Instance = nullptr;

Application::Application(unsigned int i)
{
	s_Instance = this;

	m_Window = std::unique_ptr<Window>(new Window());
	m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

	LoadScene(i);
}

Application::~Application()
{
	if (m_Scene)
		delete m_Scene;
}

void Application::OnEvent(Event& e)
{
	if (e.GetEventType() == EventType::WindowClose)
	{
		OnWindowClose();
		return;
	}
	if (e.GetEventType() == EventType::KeyPressed)
	{
		if (((KeyPressedEvent&)e).GetKeyCode() == GLFW_KEY_ESCAPE)
		{
			OnWindowClose();
			return;
		}
		if (((KeyPressedEvent&)e).GetKeyCode() == GLFW_KEY_1)
		{
			LoadScene(1);
		}
		if (((KeyPressedEvent&)e).GetKeyCode() == GLFW_KEY_2)
		{
			LoadScene(2);
		}
		if (((KeyPressedEvent&)e).GetKeyCode() == GLFW_KEY_3)
		{
			LoadScene(3);
		}
		if (((KeyPressedEvent&)e).GetKeyCode() == GLFW_KEY_4)
		{
			LoadScene(4);
		}
	}

	m_Scene->OnEvent(e);
}

void Application::Run()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_MULTISAMPLE);

	float currentTime = 0.0f;
	float lastTime = 0.0f;
	float deltaTime = 0.0f;
	float totalTime = 0.0f;
	unsigned int frames = 0;
	while (m_Running)
	{
		currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		totalTime += deltaTime;
		frames++;
		if (totalTime >= 1.0f)
		{
			totalTime -= 1.0f;
			float frameTime = (int)(1000000.0f / frames) / 1000.0f;
			LOG_INFO("FPS: {0} ({1} ms)", frames, frameTime);
			frames = 0;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_Scene->OnUpdate(deltaTime);

		m_Window->OnUpdate();
	}
}

bool Application::OnWindowClose()
{
	m_Running = false;
	return true;
}

void Application::LoadScene(unsigned int sceneNum)
{
	if (sceneNum == m_CurrentScene)
		return;

	LOG_INFO("Started loading scene {0}...", sceneNum);

	Scene* scene = nullptr;
	switch (sceneNum)
	{
	case 1:
		scene = new ShadowScene();
		break;
	case 2:
		scene = new EnvMapScene();
		break;
	case 3:
		scene = new ShadowEnvScene();
		break;
	case 4:
		scene = new StingScene();
		break;
	case 5:
		scene = new GunScene();
		break;
	case 6:
		//scene = new CheckpointScene();
		break;
	default:
		scene = new CheckpointScene();
		break;
	}

	if (m_Scene)
	{
		delete m_Scene;
		LOG_TRACE("Deleted scene {0}: {1}", m_CurrentScene, m_Scene);
	}

	m_Scene = scene;
	m_CurrentScene = sceneNum;
}


int main(int argc, char * argv[])
{
	unsigned int scene = 5;

	std::cout << "Enter scene to load [1-5]: ";
	std::string input;
	std::cin >> input;
	std::stringstream ss(input);
	ss >> scene;
	if (scene < 1 || scene > 5)
		scene = 5;

	Application* app = new Application(scene);
	app->Run();
	delete app;

	return 0;
}