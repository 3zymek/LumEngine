#include "include/modules/audio/audio_emitter_wrapper.hpp"
#include "include/modules/audio/command/audio_commands.hpp"
#include "include/modules/audio/audio_manager.hpp"

namespace audio {
	
	////////////////////////////////////
	/// Public API
	////////////////////////////////////

	AudioEmitterWrapper& AudioEmitterWrapper::Add		( string_view name ) {

		auto id = manager.GetIDByName(name);
		if (!id.has_value()) return *this;

		AudioCmd cmd;
		cmd.type = Type::AddClip;
		cmd.emitterID = emitterID;
		cmd.data.addClip.audioID = *id;

		manager.m_commands.Push(std::move(cmd));

		return *this;
	}
	AudioEmitterWrapper& AudioEmitterWrapper::Play		( string_view name ) {

		auto id = manager.GetIDByName(name);
		if (!id.has_value()) return *this;

		AudioCmd cmd;
		cmd.type = Type::Play;
		cmd.emitterID = emitterID;
		cmd.data.play.audioID = *id;

		manager.m_commands.Push(std::move(cmd));

		return *this;
	}
	AudioEmitterWrapper& AudioEmitterWrapper::Stop		( string_view name ) {

		auto id = manager.GetIDByName(name);
		if (!id.has_value()) return *this;

		AudioCmd cmd;
		cmd.type = Type::Stop;
		cmd.emitterID = emitterID;
		cmd.data.stop.audioID = *id;

		manager.m_commands.Push(std::move(cmd));

		return *this;

	}
	AudioEmitterWrapper& AudioEmitterWrapper::SetVolume	( string_view name, float volume ) {

		auto id = manager.GetIDByName(name);
		if (!id.has_value()) return *this;

		AudioCmd cmd;
		cmd.type = Type::SetVolume;
		cmd.emitterID = emitterID;
		cmd.data.setVolume.audioID = *id;
		cmd.data.setVolume.volume = volume;

		manager.m_commands.Push(std::move(cmd));

		return *this;
	}
	AudioEmitterWrapper& AudioEmitterWrapper::SetPitch	( string_view name, float pitch ) {

		auto id = manager.GetIDByName(name);
		if (!id.has_value()) return *this;

		AudioCmd cmd;
		cmd.type = Type::SetPitch;
		cmd.emitterID = emitterID;
		cmd.data.setPitch.audioID = *id;
		cmd.data.setPitch.pitch = pitch;

		manager.m_commands.Push(std::move(cmd));

		return *this;
	}
	AudioEmitterWrapper& AudioEmitterWrapper::SetPaused	( string_view name, bool paused ) {

		auto id = manager.GetIDByName(name);
		if (!id.has_value()) return *this;

		AudioCmd cmd;
		cmd.type = Type::SetPause;
		cmd.emitterID = emitterID;
		cmd.data.setPause.audioID = *id;
		cmd.data.setPause.paused = paused;

		manager.m_commands.Push(std::move(cmd));

		return *this;

	}
	AudioEmitterWrapper& AudioEmitterWrapper::SetLoop	( string_view name, bool loop ) {

		auto id = manager.GetIDByName(name);
		if (!id.has_value()) return *this;

		AudioCmd cmd;
		cmd.type = Type::SetLoop;
		cmd.emitterID = emitterID;
		cmd.data.setLoop.audioID = *id;
		cmd.data.setLoop.loop = loop;

		manager.m_commands.Push(std::move(cmd));

		return *this;

	}

	void AudioEmitterWrapper::Remove( string_view name ) {

		auto id = manager.GetIDByName(name);
		if (!id.has_value()) return;

		AudioCmd cmd;
		cmd.type = cmd::Type::RemoveClip;
		cmd.emitterID = emitterID;
		cmd.data.removeClip.audioID = *id;

		manager.m_commands.Push(std::move(cmd));
	}

}
