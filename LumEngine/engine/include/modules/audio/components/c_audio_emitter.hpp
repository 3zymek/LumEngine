#pragma once
#include "entity/ecs_common.hpp"
namespace lum {
	struct AudioEmitterComponent {

		LumComponentTag;

		audio::EmitterHandle emitterID{};

	};
}