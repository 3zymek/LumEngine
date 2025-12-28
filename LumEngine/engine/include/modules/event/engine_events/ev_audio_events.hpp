#pragma once
#include "include/core/core_defines.hpp"
#include "include/modules/event/ev_common.hpp"
#include "include/modules/entity/ecs_common.hpp"
#include "include/modules/audio/audio_common.hpp"
namespace ev {
	struct AddClipToEmitter
	{
		LumEventTag;

		audio::EmitterHandle emitterID{};
		std::string name{};

		float		volume{};
		float		pitch{};
		bool		loop{};

	};
	struct RemoveClipFromEmitter
	{
		LumEventTag;

		audio::EmitterHandle emitterID{};
		std::string name{};

	};
	struct PlaySound
	{
		LumEventTag;

		audio::EmitterHandle emitterID{};
		std::string name{};

		float		volume{};
		float		pitch{};
		bool		loop{};
	};
	struct DestroyEmitter
	{
		LumEventTag;

		audio::EmitterHandle emitterID{};
	};
	struct GetAllEmitterClips
	{
		LumEventTag;

		audio::EmitterHandle emitterID{};

	};
	struct RequestAllEmitteClips
	{
		LumEventTag;

		audio::EmitterHandle emitterID{};

		std::unordered_map<std::string, audio::AudioHandle> clips;

	};

}