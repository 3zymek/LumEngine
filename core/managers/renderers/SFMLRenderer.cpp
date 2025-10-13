#include "SFMLRenderer.hpp"


void			SFMLRenderer::renderAll() {

}
void			SFMLRenderer::init() {
	m_renderWindow.create(sf::VideoMode({ m_windowSize.x, m_windowSize.y }), m_windowName);
}

void			SFMLRenderer::setWindowSize(const Math::Vector2u& _size) {
	m_windowSize = _size;
}
void			SFMLRenderer::setWindowName(const STRING _name) {
	m_windowName = _name;
}
Math::Vector2u	SFMLRenderer::getWindowSize() {
	return m_windowSize;
}
STRING			SFMLRenderer::getWindowName() {
	return m_windowName;
}
BOOL			SFMLRenderer::windowOpen() {
	return m_renderWindow.isOpen();
}
void			SFMLRenderer::pollEvents() {
	while (auto event = m_renderWindow.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			m_renderWindow.close();
		}
	}
}

void SFMLRenderer::display() {
	m_renderWindow.display();
}
void SFMLRenderer::draw() {
	sf::CircleShape c{ 100.f, 100 };
	c.setFillColor(sf::Color::White);
	m_renderWindow.draw(c);
}
void SFMLRenderer::clear() {
	m_renderWindow.clear();
}