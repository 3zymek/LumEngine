#pragma once
#include "audio/audio_common.hpp"
#include "core/utils/command_buffer.hpp"
namespace lum {
	namespace cmd {
		namespace detail {
			struct AddClip		{ audio::AudioID audioID; };
			struct RemoveClip	{ audio::AudioID audioID; };
			struct SetVolume	{ audio::AudioID audioID; float32 volume; };
			struct SetPitch		{ audio::AudioID audioID; float32 pitch; };
			struct SetLoop		{ audio::AudioID audioID; bool loop; };
			struct SetPause		{ audio::AudioID audioID; bool paused; };
			struct Stop			{ audio::AudioID audioID; };
			struct Play			{ audio::AudioID audioID; };

			union Payload {
				AddClip		addClip;
				SetVolume	setVolume;
				SetPitch	setPitch;
				SetLoop		setLoop;
				Play		play;
				Stop		stop;
				SetPause	setPause;
				RemoveClip	removeClip;
			};
		}

		enum class Type {
			AddClip,
			RemoveClip,
			SetVolume,
			SetPitch,
			SetLoop,
			SetPause,
			Stop,
			Play
		};

		struct AudioCmd {

			LumCmdTag;

			audio::EmitterID emitterID;
			Type type;
			detail::Payload data;

		};
	}
}