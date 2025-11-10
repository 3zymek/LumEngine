#pragma once
#include "entitymodule/utils/ecs_define.hpp"
#include "audiomodule/audio_define.hpp"
struct AudioEmitterComponent {
	ComponentTag;
	audio::detail::EmitterID emitterID{};
};