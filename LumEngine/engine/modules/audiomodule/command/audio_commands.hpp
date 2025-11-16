#pragma once
#include "details/audio_define.hpp"
#include "core/command/command_buffer.hpp"
namespace cmd {
	
	namespace detail {
		struct AddClip		{ audio::detail::AudioID audioID; };
		struct RemoveClip	{ audio::detail::AudioID audioID; };
		struct SetVolume	{ audio::detail::AudioID audioID; float volume; };
		struct SetPitch		{ audio::detail::AudioID audioID; float pitch; };
		struct SetLoop		{ audio::detail::AudioID audioID; bool loop; };
		struct SetPause		{ audio::detail::AudioID audioID; bool paused; };
		struct Stop			{ audio::detail::AudioID audioID; };
		struct Play			{ audio::detail::AudioID audioID; };

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

		audio::detail::EmitterID emitterID;
		Type type;
		detail::Payload data;

	};

}
