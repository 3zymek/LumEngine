#pragma once
#include "entity/ecs_common.hpp"
namespace lum::ecs::components {
	struct AudioEmitterComponent {

		LUM_COMPONENT_TAG;

		audio::EmitterHandle emitterID{};

	};
}