#pragma once
#include "core/core_pch.hpp"
#include "event/event_common.hpp"
#include "audio/audio_common.hpp"
namespace lum {
	namespace ev {
		struct AddClipToEmitter
		{
			LUM_EVENT_TAG;
			

			audio::EmitterID emitterID;
			String name;

			float32		volume;
			float32		pitch;
			bool		loop;

		};
		struct RemoveClipFromEmitter
		{

			LUM_EVENT_TAG;

			audio::EmitterID emitterID;
			String name;

		};
		struct PlaySound
		{
			LUM_EVENT_TAG;

			audio::EmitterID emitterID;
			String name;

			float32		volume;
			float32		pitch;
			bool		loop;
		};

	}
}