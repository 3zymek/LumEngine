#pragma once
#include "IRenderer.hpp"
#include "namespaces/Math.hpp"

class OpenGLRenderer : public IRenderer
{
public:

	~OpenGLRenderer() {}

	OpenGLRenderer() {}

	void renderAll()							override;
	void init()									override;

	void setWindowSize(const Math::Vector2u&)	override;
	void setWindowName(const STRING)			override;
	Math::Vector2u	getWindowSize()				override;
	STRING			getWindowName()				override;
	BOOL windowOpen()							override;

	void display()								override;
	void draw()									override;
	void clear()								override;

	// FINISH THIS ONCE YOU LEARN OPENGL
private:

	Math::Vector2u		m_windowSize = { 0, 0 };
	STRING				m_windowName = "";

};

