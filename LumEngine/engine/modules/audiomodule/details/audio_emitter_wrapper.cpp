#include "audio_emitter_wrapper.hpp"
#include "command/audio_commands.hpp"
#include "audiomodule/details/audio_manager.hpp"

namespace audio {
	
	////////////////////////////////////
	/// Public API
	////////////////////////////////////

	AudioEmitterWrapper& AudioEmitterWrapper::Add		( string_view name ) {

		AudioCmd cmd;
		cmd.type = Type::AddClip;
		cmd.emitterID = emitterID;
		cmd.data.addClip.audioID = *manager.GetIDByName(name);

		manager.m_commands.Push(std::move(cmd));

		return *this;
	}
	AudioEmitterWrapper& AudioEmitterWrapper::Play		( string_view name ) {

		AudioCmd cmd;
		cmd.type = Type::Play;
		cmd.emitterID = emitterID;
		cmd.data.play.audioID = *manager.GetIDByName(name);

		manager.m_commands.Push(std::move(cmd));

		return *this;
	}
	AudioEmitterWrapper& AudioEmitterWrapper::SetVolume	( string_view name, float volume ) {

		AudioCmd cmd;
		cmd.type = Type::SetVolume;
		cmd.emitterID = emitterID;
		cmd.data.setVolume.audioID = *manager.GetIDByName(name);
		cmd.data.setVolume.volume = volume;

		manager.m_commands.Push(std::move(cmd));

		return *this;
	}
	AudioEmitterWrapper& AudioEmitterWrapper::SetPitch	( string_view name, float pitch ) {

		AudioCmd cmd;
		cmd.type = Type::SetPitch;
		cmd.emitterID = emitterID;
		cmd.data.setPitch.audioID = *manager.GetIDByName(name);
		cmd.data.setPitch.pitch = pitch;

		manager.m_commands.Push(std::move(cmd));

		return *this;
	}
	AudioEmitterWrapper& AudioEmitterWrapper::SetPaused	( string_view name, bool paused ) {

		AudioCmd cmd;
		cmd.type = Type::SetPause;
		cmd.emitterID = emitterID;
		cmd.data.setPause.audioID = *manager.GetIDByName(name.data());
		cmd.data.setPause.paused = paused;

		manager.m_commands.Push(std::move(cmd));

		return *this;

	}
	AudioEmitterWrapper& AudioEmitterWrapper::SetLoop	( string_view name, bool loop ) {

		AudioCmd cmd;
		cmd.type = Type::SetLoop;
		cmd.emitterID = emitterID;
		cmd.data.setLoop.audioID = *manager.GetIDByName(name.data());
		cmd.data.setLoop.loop = loop;

		manager.m_commands.Push(std::move(cmd));

		return *this;

	}

	void AudioEmitterWrapper::Remove( string_view name ) {

		AudioCmd cmd;
		cmd.type = cmd::Type::RemoveClip;
		cmd.emitterID = emitterID;
		cmd.data.removeClip.audioID = *manager.GetIDByName(name);

		manager.m_commands.Push(std::move(cmd));
	}

}
