#include "details/audio_manager.hpp"
#include "details/ev_bus.hpp"
#include "details/ecs_define.hpp"
#include "details/audio_define.hpp"

#include "details/audio_emitter_wrapper.hpp"
#include "details/audio_listener_wrapper.hpp"
#include "details/engine_events/ev_ecs_events.hpp"
#include "details/engine_events/ev_audio_events.hpp"

#include "utils/path_service.hpp"
#include "utils/string_hasher.hpp"

#include "systems/sys_audio.hpp"
#include "components/c_audio_emitter.hpp"
#include "components/c_audio_listener.hpp"

#include <fmod.hpp>
#include <fmod_studio.hpp>

namespace audio {

	////////////////////////////////////
	/// Public API
	////////////////////////////////////

	void AudioManager::Init( unsigned int maxchannels, FMOD_INITFLAGS flags, void* extradrivers ) {

		FMOD::System* sys = nullptr;

		FMOD::System_Create(&sys);
		FMOD_ASSERT(
			sys->init(maxchannels, flags, extradrivers);
		);
		m_audio_system.reset(sys);
		SubscribeEvents();


		LOG_AUDIO("initialized audio manager");
	}

	void AudioManager::LoadSound( string_view name, string_view path, FMOD_MODE mode ) {

		auto hashed_id = GetIDByName(name);

		if (hashed_id) return;

		auto id				= detail::GenerateAudioID::Get();
		auto hashed_new_id	= cstd::StringHasher::Hash(name);

		AudioClip clip;
		FMOD_ASSERT(
			m_audio_system->createSound(
				cstd::PathService::Resolve(path.data()).c_str(),
				mode, nullptr,
				&clip.sound
			);
		);

		m_sounds.append(clip, id);
		m_name_to_id.emplace(hashed_new_id, id);
		m_id_to_name.emplace(id, name.data());

		LOG_AUDIO("loaded audio - " << path);
	}

	AudioListenerWrapper	AudioManager::CreateListener( EntityID entityID ) {

		if (!m_listener) {

			m_listener = std::make_unique<AudioListenerWrapper>(entityID);
			
			m_listener->SetRawTransform( 
				m_entity_manager.GetComponent<TransformComponent>(entityID) 
			);

			LOG_AUDIO("created listener on entity " << entityID);

		}

		return *m_listener;
	}
	AudioEmitterWrapper		AudioManager::CreateEmitter	( AudioEmitterComponent* component ) {

		auto id = detail::GenerateEmitterID::Get();
		component->emitterID = id;
		detail::AudioEmitter emitter;
		AudioEmitterWrapper wrapper(*this, id);

		m_emitters.append(emitter, id);

		LOG_AUDIO("created emitter");

		return wrapper;
	}


	////////////////////////////////////
	/// Private API
	////////////////////////////////////

	void AudioManager::AddClipToEmitter			( EmitterID emitterID, AudioID audioID ) {

		ValidateEmitterAndAudioID(emitterID, audioID);

		auto& emitter = m_emitters[emitterID];

		if (emitter.clips.contains(audioID))
			return;

		detail::AudioInstance instance{};
		instance.id = audioID;

		emitter.clips.emplace(audioID, std::move(instance));

		LOG_AUDIO("added clip " << m_id_to_name[audioID] << " to emitter " << emitterID);

	}
	void AudioManager::RemoveClipFromEmitter	( EmitterID emitterID, AudioID audioID ) {

		ValidateEmitterAndAudioID(emitterID, audioID);

		auto& emitter = m_emitters[emitterID];
		emitter.clips.erase(audioID);

		LOG_AUDIO("removing clip " << m_id_to_name[audioID] << " from emitter " << emitterID);
	}
	void AudioManager::PlayEmitterClip			( EmitterID emitterID, AudioID audioID ) {

		ValidateEmitterAndAudioID(emitterID, audioID);

		auto& emitter = m_emitters[emitterID];

		if (!emitter.clips.contains(audioID))
			return;

		AudioChannel channel;
		channel.audio_clip = audioID;
		channel.emitter_id = emitterID;


		m_audio_system->playSound(m_sounds[audioID].sound, nullptr, false, &channel.channel);

		emitter.active_clips.push_back(std::move(channel));

		LOG_AUDIO("playing clip " << m_id_to_name[audioID] << " on emitter " << emitterID);

	}
	void AudioManager::StopEmitterClip			( EmitterID emitterID, AudioID audioID ) {

		ValidateEmitterAndAudioID(emitterID, audioID);

		auto& emitter = m_emitters[emitterID];

		if (!emitter.clips.contains(audioID))
			return;


		for (auto& active_clip : emitter.active_clips) {
			if (active_clip.audio_clip == audioID) {
				active_clip.channel->stop();
				std::swap(emitter.active_clips.back(), active_clip);
				emitter.active_clips.pop_back();
			}
		}

		LOG_AUDIO("stopped clip " << m_id_to_name[audioID] << " on emitter " << emitterID);

	}
	void AudioManager::SetEmitterClipVolume		( EmitterID emitterID, AudioID audioID, float volume ) {

		ValidateEmitterAndAudioID(emitterID, audioID);

		auto& emitter = m_emitters[emitterID];

		if (!emitter.clips.contains(audioID))
			return;

		emitter.clips[audioID].volume = volume;

	}
	void AudioManager::SetEmitterClipPitch		( EmitterID emitterID, AudioID audioID, float pitch ) {

		ValidateEmitterAndAudioID(emitterID, audioID);

		auto& emitter = m_emitters[emitterID];

		if (!emitter.clips.contains(audioID))
			return;

		emitter.clips[audioID].pitch = pitch;

	}
	void AudioManager::SetEmitterClipPause		( EmitterID emitterID, AudioID audioID, bool paused ) {

		ValidateEmitterAndAudioID(emitterID, audioID);

		auto& emitter = m_emitters[emitterID];

		if (!emitter.clips.contains(audioID))
			return;

		emitter.clips[audioID].paused = paused;

	}
	void AudioManager::SetEmitterClipLoop		( EmitterID emitterID, AudioID audioID, bool loop ) {

		ValidateEmitterAndAudioID(emitterID, audioID);

		auto& emitter = m_emitters[emitterID];

		if (!emitter.clips.contains(audioID))
			return;

		emitter.clips[audioID].loop = loop;

	}
	void AudioManager::DestroyEmitter			( EmitterID emitterID ) {

		ValidateEmitterID(emitterID);

		m_emitters.remove(emitterID);

		LOG_AUDIO("Destroying emitter " << emitterID);

	}
	void AudioManager::SubscribeEvents			( ) {

		ev::EventBus::Engine().SubscribePermamently<ev::ComponentAdded>(
			[this](const ev::ComponentAdded& ev)
			{
				if (ev.component_typeID == ecs::detail::ComponentTypeIndex::get<AudioListenerComponent>() or
					ev.component_typeID == ecs::detail::ComponentTypeIndex::get<AudioEmitterComponent>()) {

					m_entity_manager.RequireComponent<TransformComponent>(ev.entityID);
					
				}
					
			}
		);

	}

	AudioListenerWrapper* AudioManager::GetListener( ) {
		return m_listener.get();
	}


	////////////////////////////////////
	/// Debug
	////////////////////////////////////

	std::optional<detail::AudioID> AudioManager::GetIDByName( string_view name ) {
		auto it = m_name_to_id.find(cstd::StringHasher::Hash(name));
		if (it == m_name_to_id.end()) return std::nullopt;
		return it->second;
	}

};