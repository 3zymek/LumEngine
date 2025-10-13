#include <iostream>
#include "namespaces/Math.hpp"
#include "core/engine/Engine.hpp"
#include "core/managers/renderers/SFMLRenderer.hpp"
#include "core/managers/renderers/IRenderer.hpp"
int main()
{
	EngineConfig conf;
	conf.setAPI(API::SFML);
	//conf.setWindowName("test");
	//conf.setWindowSize({ 1000, 1000 });

	Engine engine(conf);
	engine.init();
	engine.run();
}