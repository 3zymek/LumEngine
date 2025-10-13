#pragma once
#include "IRenderer.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


class SFMLRenderer : public IRenderer
{
public:

	SFMLRenderer()	{}
	SFMLRenderer(const Math::Vector2u& _size, const STRING _name) : m_windowSize(_size), m_windowName(_name) {}
	~SFMLRenderer() {}

	void renderAll()		override;
	void init()				override;

	void			setWindowSize(const Math::Vector2u&)	override;
	void			setWindowName(const STRING)				override;
	Math::Vector2u	getWindowSize()							override;
	STRING			getWindowName()							override;
	BOOL			windowOpen()							override;
	void			pollEvents()							override;

	void display()			override;
	void draw()				override;
	void clear()			override;

private:

	sf::RenderWindow	m_renderWindow;

	Math::Vector2u		m_windowSize = { 800, 800 };
	STRING				m_windowName = "Default";

};

