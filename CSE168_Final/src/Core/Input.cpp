#include "oglpch.h"
#include "Input.h"

#include "Application.h"

bool Input::IsKeyPressed(unsigned int keycode)
{
	GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();
	int state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(unsigned int button)
{
	GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();
	int state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePosition()
{
	GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return { (float)xpos, (float)ypos };
}

float Input::GetMouseX()
{
	auto pos = GetMousePosition();
	return pos.first;
}

float Input::GetMouseY()
{
	auto pos = GetMousePosition();
	return pos.second;
}
