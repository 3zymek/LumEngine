#pragma once
#include "entitymodule/details/ecs_define.hpp"
struct AudioEmitterComponent {
	ComponentTag;
	audio::detail::EmitterID emitterID{};

};