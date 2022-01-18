#pragma once

#include "Event.h"

#include <GLFW/glfw3.h>

class Window
{
public:
	using EventCallbackFn = std::function<void(Event&)>;

	struct WindowData
	{
		std::string title;
		unsigned int width, height;

		EventCallbackFn EventCallback;

		WindowData(const std::string& title = "OpenGL Renderer",
			unsigned int width = 400,
			unsigned int height = 250)
			: title(title), width(width), height(height)
		{}
	};

	Window(const WindowData& data = WindowData());
	~Window();

	void OnUpdate();

	inline unsigned int GetWidth() const { return m_Data.width; }
	inline unsigned int GetHeight() const { return m_Data.height; }

	inline void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

	inline GLFWwindow* GetNativeWindow() const { return m_Window; }

private:
	void Init(const WindowData& data);
	void Shutdown();

private:
	GLFWwindow* m_Window;

	WindowData m_Data;
};