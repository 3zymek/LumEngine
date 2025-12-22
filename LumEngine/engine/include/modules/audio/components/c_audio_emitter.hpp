#pragma once
#include "include/modules/entity/ecs_common.hpp"
struct AudioEmitterComponent {

	LumComponentTag;

	audio::EmitterHandle emitterID{};

};