#pragma once

#include "Core/Event.h"
#include "Renderer/Camera.h"

class Scene
{
public:
	Scene() : m_Camera(nullptr) {}
	virtual ~Scene() { delete m_Camera; }

	virtual void OnEvent(Event& e) = 0;
	virtual void OnUpdate(float dt) = 0;
protected:
	Camera* m_Camera;

};