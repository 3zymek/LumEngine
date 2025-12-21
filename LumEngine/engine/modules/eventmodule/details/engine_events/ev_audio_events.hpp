#pragma once
#include "core/details/e_define.hpp"
#include "eventmodule/details/ev_define.hpp"
#include "entitymodule/details/ecs_define.hpp"
#include "audiomodule/details/audio_define.hpp"
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