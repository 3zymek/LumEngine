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
	struct RemoveClipFromEmitter
	{
		LumEventTag;

		audio::detail::EmitterID emitterID{};
		std::string name{};

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
	struct DestroyEmitter 
	{
		LumEventTag;

		audio::detail::EmitterID emitterID{};
	};
	struct GetAllEmitterClips 
	{
		LumEventTag;

		audio::detail::EmitterID emitterID{};

	};
	struct RequestAllEmitteClips 
	{
		LumEventTag;
		
		audio::detail::EmitterID emitterID{};

		std::unordered_map<std::string, audio::detail::AudioID> clips;

	};

}