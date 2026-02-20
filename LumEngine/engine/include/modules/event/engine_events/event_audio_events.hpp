#pragma once
#include "core/core_pch.hpp"
#include "event/event_common.hpp"
#include "audio/audio_common.hpp"
namespace lum {
	namespace ev {
		struct AddClipToEmitter
		{
			LumEventTag;
			

			audio::EmitterID emitterID;
			String name;

			float32		volume;
			float32		pitch;
			bool		loop;

		};
		struct RemoveClipFromEmitter
		{

			LumEventTag;

			audio::EmitterID emitterID;
			String name;

		};
		struct PlaySound
		{
			LumEventTag;

			audio::EmitterID emitterID;
			String name;

			float32		volume;
			float32		pitch;
			bool		loop;
		};

	}
}