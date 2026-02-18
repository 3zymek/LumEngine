#pragma once
#include "entity/ecs_common.hpp"
namespace lum {
	struct CAudioEmitter {

		LUM_COMPONENT_TAG;

		audio::EmitterHandle emitterID{};

	};
}