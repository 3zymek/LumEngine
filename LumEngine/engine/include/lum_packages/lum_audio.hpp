#pragma once
#include "audio/audio_common.hpp"
#include "audio/audio_manager.hpp"
#include "audio/audio_emitter_wrapper.hpp"
#include "audio/audio_listener_wrapper.hpp"
#include "audio/systems/sys_audio.hpp"
#include "audio/command/audio_commands.hpp"

namespace lum {
	using AudioManager = audio::AudioManager;
	using AudioSystem = audio::AudioSystem;
	using AudioEmitterWrapper = audio::AudioEmitterWrapper;
	using AudioListenerWrapper = audio::AudioListenerWrapper;
}