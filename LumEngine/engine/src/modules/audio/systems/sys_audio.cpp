#include "audio/audio_system.hpp"
#include "audio/audio_emitter.hpp"
#include "audio/audio_listener_wrapper.hpp"
#include "core/core_pch.hpp"
#include "audio/audio_pch.hpp"
namespace lum {
	namespace systems {

		void AudioSystem::UpdateEmitters() noexcept {

			UpdateEmitterActions();

			for (auto& emitter : manager.mEmitters) {

				for (auto& active_clip : emitter.active_clips) {

					auto& clip = emitter.clips[active_clip.mAudioClip];

					bool isPlaying;
					active_clip.mChannel->isPlaying(&isPlaying);
					if (!isPlaying) {
						manager.StopEmitterClip(active_clip.mEmitterID, active_clip.mAudioClip);
					}

					FMOD_VECTOR transfered_pos = TransferEmitterCoordsToFMOD(emitter.transform->position);
					FMOD_VECTOR vel = { 0, 0, 0 };

					active_clip.mChannel->set3DAttributes(
						&transfered_pos,	// position
						&vel				// velocity
					);
					active_clip.mChannel->setPaused(clip.paused);
					active_clip.mChannel->setVolume(clip.volume);
					active_clip.mChannel->setPitch(clip.pitch);

				}

			}

		}
		void AudioSystem::UpdateEmitterActions() noexcept {

			for (size_t i = 0; i < manager.mCmdBuffer.Count(); i++) {
				auto& cmd = manager.mCmdBuffer[i];
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

			manager.mCmdBuffer.Clear();

		}
		void AudioSystem::UpdateListener() noexcept {

			auto* listener = manager.GetListener();

			LUM_HOTCHK_RETURN_VOID(!listener, LUM_SEV_WARN, "Listener doesn't exists");
			LUM_HOTCHK_RETURN_VOID(!listener->mTransform, LUM_SEV_WARN, "Listener doesn't have transform component");

			FMOD_VECTOR transfered_pos = TransferListenerCoordsToFMOD(listener->mTransform->position);

			FMOD_VECTOR forward = TransferListenerCoordsToFMOD(listener->mListener->forward);
			FMOD_VECTOR up		= TransferListenerCoordsToFMOD(listener->mListener->up);

			FMOD_VECTOR vel = { 0, 0, 0 };

			manager.mAudioSystem->set3DListenerAttributes(
				0,				// id
				&transfered_pos,// position
				&vel,			// velocity
				&forward,		// forward
				&up				// up
			);

		}

		FMOD_VECTOR AudioSystem::TransferEmitterCoordsToFMOD(const math::Vec3 v) noexcept {

			FMOD_VECTOR fmodv;
			fmodv.x = v.x;
			fmodv.y = v.y;
			fmodv.z = -v.z;
			return fmodv;

		}

		FMOD_VECTOR AudioSystem::TransferListenerCoordsToFMOD(const math::Vec3 v) noexcept {

			FMOD_VECTOR fmodv;
			fmodv.x = -v.x;
			fmodv.y = v.y;
			fmodv.z = -v.z;
			return fmodv;

		}
	}
}