#include "engine/Engine.hpp"
#include "managers/renderers/IRenderer.hpp"
#include "managers/renderers/SFMLRenderer.hpp"
#include "managers/renderers/OpenGLRenderer.hpp"

void Engine::init() {

	switch (config.api) {
	case(API::SFML): {
		m_renderer = std::make_unique<SFMLRenderer>(config.windowSize, config.windowName);
		break;
	}
	case(API::OPENGL): {
		
		break;
	}
	}
	
	// Inits

	m_renderer->init();
	
}
void Engine::run() {

	if (config.api == API::SFML) {
		handleSFML();
	}
	else if (config.api == API::OPENGL) {
		handleGL();
	}

}



void Engine::handleSFML() {

	while (m_renderer->windowOpen()) {

		m_renderer->pollEvents();

		m_renderer->clear();
		m_renderer->draw();
		m_renderer->display();

	}

}
void Engine::handleGL() {
	// FINISH WHEN YOU LEARN OPENGL
}