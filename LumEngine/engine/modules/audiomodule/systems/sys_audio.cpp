#include "sys_audio.hpp"
#include "details/audio_emitter.hpp"
#include "details/audio_listener_wrapper.hpp"
namespace audio {




	void AudioSystem::UpdateEmitters		( ) noexcept {

		UpdateEmitterActions( );

		for (auto& emitter : manager.m_emitters) {

			for (auto& active_clip : emitter.active_clips) {

				auto& clip = emitter.clips[active_clip.audio_clip];

				active_clip.channel->setPaused(clip.paused);
				active_clip.channel->setVolume(clip.volume);
				active_clip.channel->setPitch(clip.pitch);

			}

		}

	}
	void AudioSystem::UpdateEmitterActions	( ) {

		for (size_t i = 0; i < manager.m_commands.Count(); i++) {
			auto& cmd = manager.m_commands[i];
			switch (cmd.type) {

			case cmd::Type::AddClip: {
				manager.AddClipToEmitter(cmd.emitterID, cmd.data.addClip.audioID);
				break;
			}
			case cmd::Type::Play: {
				manager.PlayEmitterClip(cmd.emitterID, cmd.data.play.audioID);
				break;
			}
			case cmd::Type::SetVolume: {
				manager.SetEmitterClipVolume(cmd.emitterID, cmd.data.setVolume.audioID, cmd.data.setVolume.volume);
				break;
			}
			case cmd::Type::SetPitch: {
				manager.SetEmitterClipVolume(cmd.emitterID, cmd.data.setVolume.audioID, cmd.data.setVolume.volume);
				break;
			}
			case cmd::Type::RemoveClip: {
				manager.RemoveClipFromEmitter(cmd.emitterID, cmd.data.removeClip.audioID);
				break;
			}
			case cmd::Type::SetPause: {
				manager.SetEmitterClipPause(cmd.emitterID, cmd.data.setPause.audioID, cmd.data.setPause.paused);
				break;
			}

			}
		}

		manager.m_commands.Clear();

	}
	void AudioSystem::UpdateListener		( ) noexcept {

		auto* listener = manager.GetListener();

		FMOD_VECTOR transfered_pos = TransferCoordsToFMOD(listener->GetPosition());

		manager.m_audio_system->set3DListenerAttributes(
			0,
			&transfered_pos,
			nullptr,
			nullptr,
			nullptr
		);

	}

	FMOD_VECTOR AudioSystem::TransferCoordsToFMOD( const Vec3f& v ) noexcept {

		FMOD_VECTOR fmodv;
		fmodv.x = v.x();
		fmodv.y = v.y();
		fmodv.z = v.z();
		return fmodv;

	}

}