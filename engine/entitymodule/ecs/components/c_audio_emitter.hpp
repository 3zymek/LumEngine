#pragma once
#include "ecs_define.hpp"
#include "audio_define.hpp"
struct AudioEmitterComponent {
	ComponentTag;
	audio::detail::EmitterID id{};
};