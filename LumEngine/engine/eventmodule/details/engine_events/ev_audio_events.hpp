#pragma once
#include "utils/e_define.hpp"
#include "eventmodule/details/ev_define.hpp"
#include "entitymodule/details/ecs_define.hpp"
#include "audiomodule/details/audio_define.hpp"
namespace ev {
	struct AddClipToEmitter 
	{
		LumEventTag;

		audio::detail::EmitterID emitterID{};
		std::string name{};

		float		volume{};
		float		pitch{};
		bool		loop{};
	};
	struct PlaySound 
	{
		LumEventTag;

		audio::detail::EmitterID emitterID{};
		std::string name{};

		float		volume{};
		float		pitch{};
		bool		loop{};
	};
}