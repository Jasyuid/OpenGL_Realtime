#include "oglpch.h"
#include "Camera.h"

#include "Core/MouseEvent.h"
#include "Core/Input.h"
#include "Application.h"

// Camera movement utils
glm::vec2 m_start = glm::vec2(0.0f, 0.0f);
glm::vec2 m_end = glm::vec2(0.0f, 0.0f);

Camera::Camera(const glm::vec3& position, const glm::vec3& target, const ProjectionProps& props)
	: m_Target(target), m_InitTarget(target)
{
	// Create projection matrix
	m_Projection = glm::perspective(glm::radians(props.FOV), props.Ratio, props.Near, props.Far);

	// Store camera position on unit sphere
	m_UnitPosition = glm::normalize(position - target);
	m_Radius = glm::length(position);

	// Store yaw and pitch values (radians)
	m_Yaw = std::asin(m_UnitPosition.x);
	m_Pitch = std::asin(m_UnitPosition.y);

	// Create view matrix
	m_View = glm::lookAt(m_UnitPosition * m_Radius, m_Target, glm::vec3(0, 1, 0));
}

Camera::~Camera()
{
}

void Camera::OnEvent(Event& e)
{
	if (e.GetEventType() == EventType::MouseButtonPressed)
	{
		if (((MouseButtonPressedEvent&)e).GetMouseButton() == GLFW_MOUSE_BUTTON_1)
		{
			// Reset tracking points
			m_start = glm::vec2(0.0f, 0.0f);
			m_end = glm::vec2(0.0f, 0.0f);

			// Set starting point to current mouse position
			float xpos = Input::GetMouseX();
			float ypos = Input::GetMouseY();
			m_start = glm::vec2(xpos, ypos);
		}
	}
	else if (e.GetEventType() == EventType::MouseScrolled)
	{
		// Zoom camera based on current zoom level
		float y = ((MouseScrolledEvent&)e).GetY();
		Zoom(y * -(m_Radius / 12.0f));

	}
	else if (e.GetEventType() == EventType::MouseMoved)
	{
		// Get current mouse position
		float x = ((MouseMovedEvent&)e).GetX();
		float y = ((MouseMovedEvent&)e).GetY();

		if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
		{
			// Set the end point to where the mouse cursor currently is
			m_end = glm::vec2(x, y);

			// Rotate the camera by change in mouse position
			glm::vec2 delta = m_end - m_start;
			Rotate(delta.x * -0.005f, delta.y * 0.005f);

			// Reset the starting vertex to the current one
			m_start = m_end;
		}
	}
}

void Camera::Rotate(float deltaYaw, float deltaPitch)
{
	// Adjust yaw and pitch values
	m_Yaw += deltaYaw;
	m_Pitch += deltaPitch;

	if (m_Yaw > 2 * std::acos(0.0))
		m_Yaw -= 4 * std::acos(0.0);
	else if (m_Yaw < -2 * std::acos(0.0))
		m_Yaw += 4 * std::acos(0.0);

	if (m_Pitch >= std::acos(0.0))
		m_Pitch = std::acos(0.0) - 0.001f;
	else if (m_Pitch <= -std::acos(0.0))
		m_Pitch = -std::acos(0.0) + 0.001f;

	m_UnitPosition.x = (float)(std::sin(m_Yaw) * std::cos(m_Pitch));
	m_UnitPosition.y = (float)(std::sin(m_Pitch));
	m_UnitPosition.z = (float)(std::cos(m_Yaw) * std::cos(m_Pitch));

	m_View = glm::lookAt((m_UnitPosition * m_Radius) + m_Target, m_Target, glm::vec3(0, 1, 0));
}
void Camera::Zoom(float deltaZoom)
{
	m_Radius += deltaZoom;

	if (m_Radius <= 0.0f)
		m_Radius = 0.001f;

	m_View = glm::lookAt((m_UnitPosition * m_Radius) + m_Target, m_Target, glm::vec3(0, 1, 0));
}

void Camera::Move(const glm::vec3& deltaPos)
{
	m_Target.x += (float)((-std::sin(m_Yaw) * deltaPos.z) + (std::cos(m_Yaw) * deltaPos.x));
	m_Target.y += deltaPos.y;
	m_Target.z += (float)((-std::cos(m_Yaw) * deltaPos.z) + (-std::sin(m_Yaw) * deltaPos.x));

	m_View = glm::lookAt((m_UnitPosition * m_Radius) + m_Target, m_Target, glm::vec3(0, 1, 0));
}

void Camera::SetZoom(float zoom)
{
	m_Radius = zoom;

	m_View = glm::lookAt((m_UnitPosition * m_Radius) + m_Target, m_Target, glm::vec3(0, 1, 0));
}

void Camera::SetTarget(const glm::vec3& target)
{
	m_Target = target;
}

