#pragma once
#include "Math.hpp"
#include "e_define.hpp"
#include "ev_manager.hpp"
class Engine
{ 
private:

public:

	Engine() { ENGINE_LOG("Engine - initialized") }

	void init();
	void run();

#ifdef DEBUG_ENGINE

#endif // DEBUG

};

