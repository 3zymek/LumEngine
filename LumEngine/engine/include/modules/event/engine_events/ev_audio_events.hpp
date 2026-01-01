#pragma once
#include "core/core_pch.hpp"
#include "event/ev_common.hpp"
#include "audio/audio_common.hpp"
namespace lum {
	namespace ev {
		struct AddClipToEmitter
		{
			LumEventTag;

			audio::EmitterHandle emitterID;
			std::string name;

			float		volume;
			float		pitch;
			bool		loop;

		};
		struct RemoveClipFromEmitter
		{

			LumEventTag;

			audio::EmitterHandle emitterID;
			std::string name;

		};
		struct PlaySound
		{
			LumEventTag;

			audio::EmitterHandle emitterID;
			std::string name;

			float		volume;
			float		pitch;
			bool		loop;
		};

	}
}