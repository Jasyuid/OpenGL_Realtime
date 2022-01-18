#pragma once

#include "Core/Event.h"
#include "Scene/Scene.h"

#include "Core/Window.h"

class Application
{
public:
	Application(unsigned int i);
	~Application();

	void OnEvent(Event& e);

	void Run();

	inline Window& GetWindow() { return *m_Window; }
	inline static Application& Get() { return *s_Instance; }

private:
	bool OnWindowClose();

	void LoadScene(unsigned int sceneNum);

private:
	std::unique_ptr<Window> m_Window;
	bool m_Running = true;

	unsigned int m_CurrentScene = 0;
	Scene* m_Scene = nullptr;

	static Application* s_Instance;

};