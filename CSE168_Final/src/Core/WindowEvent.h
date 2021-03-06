#pragma once

#include "Event.h"

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {}

	EVENT_CLASS_TYPE(WindowClose);
};

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(unsigned int width, unsigned int height)
		: m_Width(width), m_Height(height) {}

	inline unsigned int GetWidth() const { return m_Width; }
	inline unsigned int GetHeight() const { return m_Height; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: (" << m_Width << ", " << m_Height << ")";
		return ss.str();
	}

	EVENT_CLASS_TYPE(WindowResize);

private:
	unsigned int m_Width, m_Height;
};

class WindowFocusEvent : public Event
{
public:
	WindowFocusEvent() {}

	EVENT_CLASS_TYPE(WindowFocus);
};

class WindowLostFocusEvent : public Event
{
public:
	WindowLostFocusEvent() {}

	EVENT_CLASS_TYPE(WindowLostFocus);
};

class WindowMovedEvent : public Event
{
public:
	WindowMovedEvent(unsigned int xPos, unsigned int yPos)
		: m_XPos(xPos), m_YPos(yPos) {}

	inline unsigned int GetX() const { return m_XPos; }
	inline unsigned int GetY() const { return m_YPos; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowMovedEvent: (" << m_XPos << ", " << m_YPos << ")";
		return ss.str();
	}

	EVENT_CLASS_TYPE(WindowMoved);

private:
	unsigned int m_XPos, m_YPos;
};