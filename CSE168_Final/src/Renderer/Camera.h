#pragma once

#include "Core/Event.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// Projection matrix properties
struct ProjectionProps
{
	float FOV;
	float Ratio;
	float Near, Far;
	// Default properties
	ProjectionProps(float fov = 60.0f, float ratio = 16.0f / 9.0f, float min = 0.1f, float max = 10000.0f)
		: FOV(fov), Ratio(ratio), Near(min), Far(max)
	{
	}
};

class Camera
{
public:
	Camera(const glm::vec3& position, const glm::vec3& target, const ProjectionProps& props = ProjectionProps());
	~Camera();

	// Event Handler
	void OnEvent(Event& e);

	// Camera transformations
	void Rotate(float deltaYaw, float deltaPitch);
	void Zoom(float deltaZoom);
	void Move(const glm::vec3& deltaPos);

	// Set camera transform properties
	void SetZoom(float zoom);
	void SetTarget(const glm::vec3& target);

	// Reset camera target
	inline void ResetTarget() { m_Target = m_InitTarget; };

	inline glm::mat4 GetViewMat() const { return m_View; }
	inline glm::mat4 GetProjMat() const { return m_Projection; }
	inline glm::mat4 GetCameraMat() const { return m_Projection * m_View; }
	inline glm::vec3 GetPosition() const { return m_UnitPosition * m_Radius; }
	inline float GetRadius() const { return m_Radius; }

private:
	// Projection and view matrices
	glm::mat4 m_Projection, m_View;
	// Target positions
	glm::vec3 m_Target, m_InitTarget;
	// Camera angle values
	glm::vec3 m_UnitPosition;
	float m_Radius;
	double m_Yaw, m_Pitch;
};