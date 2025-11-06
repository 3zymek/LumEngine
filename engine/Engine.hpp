#pragma once
#include "Math.hpp"
#include "e_define.hpp"
<<<<<<< HEAD
#include "ev_manager.hpp"
=======
#include "ev_bus.hpp"
>>>>>>> 312f60e (backup)
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

