#pragma once
#include "entity/ecs_common.hpp"
namespace lum::ecs::components {
	struct AudioEmitterComponent {

		LumComponentTag;

		audio::EmitterHandle emitterID{};

	};
}