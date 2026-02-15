#include "audio/audio_emitter_wrapper.hpp"
#include "audio/command/audio_commands.hpp"
#include "audio/audio_manager.hpp"

namespace lum {
	namespace audio {

		////////////////////////////////////
		/// Public API
		////////////////////////////////////

		AudioEmitterWrapper& AudioEmitterWrapper::Add(StringView name) {

			auto id = manager.GetIDByName(name);
			if (!id.has_value()) return *this;

			AudioCmd cmd;
			cmd.type = Type::AddClip;
			cmd.emitterID = emitterID;
			cmd.data.addClip.audioID = *id;

			manager.mCmdBuffer.Push(std::move(cmd));

			return *this;
		}
		AudioEmitterWrapper& AudioEmitterWrapper::Play(StringView name) {

			auto id = manager.GetIDByName(name);
			if (!id.has_value()) return *this;

			AudioCmd cmd;
			cmd.type = Type::Play;
			cmd.emitterID = emitterID;
			cmd.data.play.audioID = *id;

			manager.mCmdBuffer.Push(std::move(cmd));

			return *this;
		}
		AudioEmitterWrapper& AudioEmitterWrapper::Stop(StringView name) {

			auto id = manager.GetIDByName(name);
			if (!id.has_value()) return *this;

			AudioCmd cmd;
			cmd.type = Type::Stop;
			cmd.emitterID = emitterID;
			cmd.data.stop.audioID = *id;

			manager.mCmdBuffer.Push(std::move(cmd));

			return *this;

		}
		AudioEmitterWrapper& AudioEmitterWrapper::SetVolume(StringView name, float32 volume) {

			auto id = manager.GetIDByName(name);
			if (!id.has_value()) return *this;

			AudioCmd cmd;
			cmd.type = Type::SetVolume;
			cmd.emitterID = emitterID;
			cmd.data.setVolume.audioID = *id;
			cmd.data.setVolume.volume = std::clamp(volume, 0.f, 1.f);

			manager.mCmdBuffer.Push(std::move(cmd));

			return *this;
		}
		AudioEmitterWrapper& AudioEmitterWrapper::SetPitch(StringView name, float32 pitch) {

			auto id = manager.GetIDByName(name);
			if (!id.has_value()) return *this;

			AudioCmd cmd;
			cmd.type = Type::SetPitch;
			cmd.emitterID = emitterID;
			cmd.data.setPitch.audioID = *id;
			cmd.data.setPitch.pitch = std::clamp(pitch, 0.f, 1.f);

			manager.mCmdBuffer.Push(std::move(cmd));

			return *this;
		}
		AudioEmitterWrapper& AudioEmitterWrapper::SetPaused(StringView name, bool paused) {

			auto id = manager.GetIDByName(name);
			if (!id.has_value()) return *this;

			AudioCmd cmd;
			cmd.type = Type::SetPause;
			cmd.emitterID = emitterID;
			cmd.data.setPause.audioID = *id;
			cmd.data.setPause.paused = paused;

			manager.mCmdBuffer.Push(std::move(cmd));

			return *this;

		}
		AudioEmitterWrapper& AudioEmitterWrapper::SetLoop(StringView name, bool loop) {

			auto id = manager.GetIDByName(name);
			if (!id.has_value()) return *this;

			AudioCmd cmd;
			cmd.type = Type::SetLoop;
			cmd.emitterID = emitterID;
			cmd.data.setLoop.audioID = *id;
			cmd.data.setLoop.loop = loop;

			manager.mCmdBuffer.Push(std::move(cmd));

			return *this;

		}

		void AudioEmitterWrapper::Remove(StringView name) {

			auto id = manager.GetIDByName(name);
			if (!id.has_value())
				return;

			AudioCmd cmd;
			cmd.type = cmd::Type::RemoveClip;
			cmd.emitterID = emitterID;
			cmd.data.removeClip.audioID = *id;

			manager.mCmdBuffer.Push(std::move(cmd));
		}

		float AudioEmitterWrapper::GetVolume(StringView name) {
			auto id = manager.GetIDByName(name);
			if (!id.has_value())
				return std::numeric_limits<float32>::max();
			return manager.GetEmitterClipVolume(emitterID, *id);
		}

		float AudioEmitterWrapper::GetPitch(StringView name) {
			auto id = manager.GetIDByName(name);
			if (!id.has_value())
				return std::numeric_limits<float32>::max();
			return manager.GetEmitterClipPitch(emitterID, *id);
		}

		bool AudioEmitterWrapper::GetPaused(StringView name) {
			auto id = manager.GetIDByName(name);
			if (!id.has_value())
				return std::numeric_limits<float32>::max();
			return manager.GetEmitterClipPaused(emitterID, *id);
		}

		bool AudioEmitterWrapper::GetLooped(StringView name) {
			auto id = manager.GetIDByName(name);
			if (!id.has_value())
				return std::numeric_limits<float32>::max();
			return manager.GetEmitterClipLooped(emitterID, *id);
		}

	}
}