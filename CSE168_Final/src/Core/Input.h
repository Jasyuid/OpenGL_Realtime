#pragma once

#include <utility>

class Input
{
	friend class Window;
public:
	// Key inputs
	static bool IsKeyPressed(unsigned int keycode);

	// Mouse inputs
	static bool IsMouseButtonPressed(unsigned int button);
	static std::pair<float, float> GetMousePosition();
	static float GetMouseX();
	static float GetMouseY();

};