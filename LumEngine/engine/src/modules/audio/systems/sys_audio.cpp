#include "audio/audio_system.hpp"
#include "audio/audio_emitter.hpp"
#include "audio/audio_listener_wrapper.hpp"
#include "core/core_pch.hpp"
#include "audio/audio_pch.hpp"
namespace lum {
	namespace systems {

		void AudioSystem::UpdateEmitters() noexcept {

			UpdateEmitterActions();

			for (auto& emitter : manager.m_emitters) {

				for (auto& active_clip : emitter.active_clips) {

					auto& clip = emitter.clips[active_clip.audio_clip];

					bool isPlaying;
					active_clip.channel->isPlaying(&isPlaying);
					if (!isPlaying) {
						manager.StopEmitterClip(active_clip.emitter_id, active_clip.audio_clip);
					}

					FMOD_VECTOR transfered_pos = TransferEmitterCoordsToFMOD(emitter.transform->position);
					FMOD_VECTOR vel = { 0, 0, 0 };

					active_clip.channel->set3DAttributes(
						&transfered_pos,	// position
						&vel				// velocity
					);
					active_clip.channel->setPaused(clip.paused);
					active_clip.channel->setVolume(clip.volume);
					active_clip.channel->setPitch(clip.pitch);

				}

			}

		}
		void AudioSystem::UpdateEmitterActions() noexcept {

			for (size_t i = 0; i < manager.m_commands.Count(); i++) {
				auto& cmd = manager.m_commands[i];
				switch (cmd.type) {

				case Type::AddClip: {
					manager.AddClipToEmitter(cmd.emitterID, cmd.data.addClip.audioID);
					break;
				}
				case Type::Play: {
					manager.PlayEmitterClip(cmd.emitterID, cmd.data.play.audioID);
					break;
				}
				case Type::SetVolume: {
					manager.SetEmitterClipVolume(cmd.emitterID, cmd.data.setVolume.audioID, cmd.data.setVolume.volume);
					break;
				}
				case Type::SetPitch: {
					manager.SetEmitterClipVolume(cmd.emitterID, cmd.data.setVolume.audioID, cmd.data.setVolume.volume);
					break;
				}
				case Type::RemoveClip: {
					manager.RemoveClipFromEmitter(cmd.emitterID, cmd.data.removeClip.audioID);
					break;
				}
				case Type::SetPause: {
					manager.SetEmitterClipPause(cmd.emitterID, cmd.data.setPause.audioID, cmd.data.setPause.paused);
					break;
				}
				case Type::Stop: {
					manager.StopEmitterClip(cmd.emitterID, cmd.data.stop.audioID);
					break;
				}
				}
			}

			manager.m_commands.Clear();

		}
		void AudioSystem::UpdateListener() noexcept {

			auto* listener = manager.GetListener();

			HOTPATH_CHECK_RETURN_0(!listener, "Listener doesn't exists");
			HOTPATH_CHECK_RETURN_0(!listener->transform_component, "Listener doesn't have transform component");

			FMOD_VECTOR transfered_pos = TransferListenerCoordsToFMOD(listener->transform_component->position);

			FMOD_VECTOR forward = TransferListenerCoordsToFMOD(listener->listener_component->forward);
			FMOD_VECTOR up		= TransferListenerCoordsToFMOD(listener->listener_component->up);

			FMOD_VECTOR vel = { 0, 0, 0 };

			manager.m_audio_system->set3DListenerAttributes(
				0,				// id
				&transfered_pos,// position
				&vel,			// velocity
				&forward,		// forward
				&up				// up
			);

		}

		FMOD_VECTOR AudioSystem::TransferEmitterCoordsToFMOD(const glm::vec3 v) noexcept {

			FMOD_VECTOR fmodv;
			fmodv.x = v.x;
			fmodv.y = v.y;
			fmodv.z = -v.z;
			return fmodv;

		}

		FMOD_VECTOR AudioSystem::TransferListenerCoordsToFMOD(const glm::vec3 v) noexcept {

			FMOD_VECTOR fmodv;
			fmodv.x = -v.x;
			fmodv.y = v.y;
			fmodv.z = -v.z;
			return fmodv;

		}
	}
}