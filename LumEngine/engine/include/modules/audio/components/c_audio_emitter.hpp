#pragma once
#include "entity/ecs_common.hpp"
struct AudioEmitterComponent {

	LumComponentTag;

	audio::EmitterHandle emitterID{};

};