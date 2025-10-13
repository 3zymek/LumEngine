#pragma once
#include "namespaces/Math.hpp"
#include <iostream>


class IRenderer;
class SFMLRenderer;

enum class API {
	SFML,
	OPENGL,
	NONE
};

struct EngineConfig {

public:

	EngineConfig() {}

	API				api			= API::NONE;
	STRING			windowName	= "Default";
	Math::Vector2u	windowSize	= { 800, 800 };

	void setAPI(const API& _api) {
		api = _api;
	}
	void setWindowSize(const Math::Vector2u& _size) {
		windowSize = _size;
	}
	void setWindowName(const STRING _name) {
		windowName = _name;
	}

};

class Engine
{
public:

	const EngineConfig config;

	void init();
	void run();

	Engine(EngineConfig& _config) : config(_config) {}

	~Engine() {}

private:

	void handleSFML();
	void handleGL();

	std::unique_ptr<IRenderer> m_renderer = nullptr;

};

