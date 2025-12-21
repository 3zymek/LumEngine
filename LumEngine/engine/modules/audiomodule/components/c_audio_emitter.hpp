#pragma once
#include "entitymodule/details/ecs_define.hpp"
struct AudioEmitterComponent {

	LumComponentTag;

	audio::EmitterHandle emitterID{};

};