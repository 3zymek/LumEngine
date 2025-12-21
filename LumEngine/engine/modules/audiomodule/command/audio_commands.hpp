#pragma once
#include "details/audio_define.hpp"
#include "core/command/command_buffer.hpp"
namespace cmd {
	
	namespace detail {
		struct AddClip		{ audio::AudioHandle audioID; };
		struct RemoveClip	{ audio::AudioHandle audioID; };
		struct SetVolume	{ audio::AudioHandle audioID; float volume; };
		struct SetPitch		{ audio::AudioHandle audioID; float pitch; };
		struct SetLoop		{ audio::AudioHandle audioID; bool loop; };
		struct SetPause		{ audio::AudioHandle audioID; bool paused; };
		struct Stop			{ audio::AudioHandle audioID; };
		struct Play			{ audio::AudioHandle audioID; };

		union Payload {
			AddClip addClip;
			SetVolume setVolume;
			SetPitch setPitch;
			SetLoop setLoop;
			Play play;
			Stop stop;
			SetPause setPause;
			RemoveClip removeClip;
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

		audio::EmitterHandle emitterID;
		Type type;
		detail::Payload data;

	};

}
