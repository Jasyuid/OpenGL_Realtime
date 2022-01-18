#include "oglpch.h"
#include "Window.h"

#include <glad/glad.h>

#include "WindowEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

static bool s_GLFWInitialized = false;

Window::Window(const WindowData& data)
{
	Init(data);
}

Window::~Window()
{
	Shutdown();
}

void Window::Init(const WindowData& data)
{
	m_Data.title = data.title;
	m_Data.width = data.width;
	m_Data.height= data.height;

	LOG_INFO("Creating GLFW window '{0}' ({1}, {2})", data.title, data.width, data.height);

	// Initialize GLFW
	if (!s_GLFWInitialized)
	{
		int success = glfwInit();
		// Check for error
		if (!success)
			LOG_ERROR("Failed to  initialize GLFW!");
		// Create error callback
		glfwSetErrorCallback([](int error, const char* description) {
			LOG_ERROR("GLFW Error ({0}): {1}", error, description);
			});
		s_GLFWInitialized = true;
	}

	// TODO: Maybe change window hints
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFW window
	m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);

	// Initialize Glad
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!status)
		LOG_ERROR("Failed to initialize Glad!");
	// Final setup
	glfwSetWindowUserPointer(m_Window, &m_Data);
	glfwSwapInterval(0);

	// Window callbacks
	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			// Reset the viewport
			glViewport(0, 0, width, height);

			WindowResizeEvent event(width, height);
			data.EventCallback(event);

		});
	glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			if (focused == GLFW_TRUE)
			{
				WindowFocusEvent event;
				data.EventCallback(event);
			}
			else
			{
				WindowLostFocusEvent event;
				data.EventCallback(event);
			}
		});
	glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowMovedEvent event(xpos, ypos);
			data.EventCallback(event);
		});

	// Key Callbacks
	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			}
		});

	// Mouse Callbacks
	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});
	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});


}

void Window::OnUpdate()
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

void Window::Shutdown()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}



