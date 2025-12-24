#include "include/modules/audio/audio_manager.hpp"
#include "include/modules/event/ev_bus.hpp"
#include "include/modules/entity/ecs_common.hpp"
#include "include/modules/audio/audio_common.hpp"
#include "include/modules/entity/entity.hpp"

#include "include/modules/audio/audio_emitter_wrapper.hpp"
#include "include/modules/audio/audio_listener_wrapper.hpp"
#include "include/core/utils/path_service.hpp"
#include "include/core/utils/string_hasher.hpp"

#include "include/modules/audio/systems/sys_audio.hpp"
#include "include/modules/audio/components/c_audio_emitter.hpp"
#include "include/modules/audio/components/c_audio_listener.hpp"

namespace audio {

	////////////////////////////////////
	/// Public API
	////////////////////////////////////

	void AudioManager::Init( unsigned int maxchannels, FMOD_INITFLAGS flags, void* extradrivers ) {

		FMOD::System* sys = nullptr;

		FMOD::System_Create(&sys);
		sys->init(maxchannels, flags, extradrivers);

		if (!sys) {
			LOG_INIT_FAIL("Audio manager initialization");
		}
		else {
			LOG_INIT_OK("Audio manager initialization");
		}

		m_audio_system.reset(sys);
		SubscribeEvents();


		
	}

	void AudioManager::LoadSound( string_view alias_name, string_view path, FMOD_MODE mode ) {

		if (NameExists(alias_name)) {
			LOG_ERROR(std::format("Audio Clip with name {} already exists", alias_name.data()));
			return;
		}

		auto id				= GenerateID<AudioHandle, detail::AUDIO_ID_NULL>::Get();
		auto hashed_new_id	= cstd::StringHasher::Hash(alias_name);

		AudioClip clip;
		FMOD_ASSERT(
			m_audio_system->createSound(
				cstd::PathService::GetPath(path.data()).c_str(),
				mode,
				nullptr,
				&clip.sound
			)
		);

		m_sounds.emplace(clip, id);
		m_name_to_id.emplace(hashed_new_id, id);

		#ifdef ENABLE_LOGGER
			m_id_to_name.emplace(id, alias_name.data());
		#endif

		LOG_INFO(std::format("Audio {} created", alias_name.data()));

	}

	AudioListenerWrapper	AudioManager::CreateListener( EntityID entityID ) {

		if (!m_listener) {

			m_listener = std::make_unique<AudioListenerWrapper>(entityID);
			
			m_listener->SetRawTransform( 
				m_entity_manager.GetComponent<TransformComponent>(entityID) 
			);

			LOG_INFO("Audio Listener created");

		}

		return *m_listener;
	}
	AudioEmitterWrapper		AudioManager::CreateEmitter	( AudioEmitterComponent* component ) {

		auto id = GenerateID<EmitterHandle, detail::EMITTER_ID_NULL>::Get();
		component->emitterID = id;
		detail::AudioEmitter emitter;
		AudioEmitterWrapper wrapper(*this, id);

		m_emitters.emplace(emitter, id);

		LOG_INFO("Audio Emitter created");

		return wrapper;
	}
	AudioEmitterWrapper		AudioManager::CreateEmitter( Entity entity ) {

		if (!entity.Has<AudioEmitterComponent>())
			entity.AddComponent<AudioEmitterComponent>();

		auto id = GenerateID<EmitterHandle, detail::EMITTER_ID_NULL>::Get();
		entity.GetComponent<AudioEmitterComponent>()->emitterID = id;
		detail::AudioEmitter emitter;
		AudioEmitterWrapper wrapper(*this, id);

		m_emitters.emplace(emitter, id);

		LOG_INFO("Audio Emitter created");

		return wrapper;
	}




	////////////////////////////////////
	/// Private API
	////////////////////////////////////

	void AudioManager::AddClipToEmitter			( EmitterHandle emitterID, AudioHandle audioID ) {

		ValidateEmitterAndAudioID(emitterID, audioID);

		auto& emitter = m_emitters[emitterID];

		if (emitter.clips.contains(audioID))
			return;

		detail::AudioInstance instance{};
		instance.id = audioID;

		emitter.clips.emplace(audioID, std::move(instance));

		LOG_DEBUG(std::format("Added clip {} to emitter {}", m_id_to_name[audioID], emitterID));

	}
	void AudioManager::RemoveClipFromEmitter	( EmitterHandle emitterID, AudioHandle audioID ) {

		ValidateEmitterAndAudioID(emitterID, audioID);

		auto& emitter = m_emitters[emitterID];
		emitter.clips.erase(audioID);

		LOG_DEBUG(std::format("Removed clip {} from emitter {}", m_id_to_name[audioID], emitterID));
	}
	void AudioManager::PlayEmitterClip			( EmitterHandle emitterID, AudioHandle audioID ) {

		ValidateEmitterAndAudioID(emitterID, audioID);

		auto& emitter = m_emitters[emitterID];

		if (!emitter.clips.contains(audioID))
			return;

		AudioChannel channel;
		channel.audio_clip = audioID;
		channel.emitter_id = emitterID;


		m_audio_system->playSound(m_sounds[audioID].sound, nullptr, false, &channel.channel);

		emitter.active_clips.push_back(std::move(channel));

		LOG_DEBUG(std::format("Played clip {} on emitter {}", m_id_to_name[audioID], emitterID));

	}
	void AudioManager::StopEmitterClip			( EmitterHandle emitterID, AudioHandle audioID ) {

		ValidateEmitterAndAudioID(emitterID, audioID);

		auto& emitter = m_emitters[emitterID];

		if (!emitter.clips.contains(audioID)) {
			LOG_WARN(std::format("Audio {} does not exists", m_id_to_name[audioID]));
			return;
		}


		for (auto& active_clip : emitter.active_clips) {
			if (active_clip.audio_clip == audioID) {
				active_clip.channel->stop();
				std::swap(emitter.active_clips.back(), active_clip);
				emitter.active_clips.pop_back();
			}
		}

		LOG_DEBUG(std::format("Stopped clip {} on emitter {}", m_id_to_name[audioID], emitterID));

	}
	void AudioManager::SetEmitterClipVolume		( EmitterHandle emitterID, AudioHandle audioID, float volume ) {

		ValidateEmitterAndAudioID(emitterID, audioID);

		auto& emitter = m_emitters[emitterID];

		if (!emitter.clips.contains(audioID)) {
			LOG_WARN(std::format("Audio {} does not exists", m_id_to_name[audioID]));
			return;
		}

		emitter.clips[audioID].volume = volume;

		LOG_DEBUG(std::format("Set volume ({}) at clip {} on emitter {}", volume, m_id_to_name[audioID], emitterID));

	}
	void AudioManager::SetEmitterClipPitch		( EmitterHandle emitterID, AudioHandle audioID, float pitch ) {

		ValidateEmitterAndAudioID(emitterID, audioID);

		auto& emitter = m_emitters[emitterID];

		if (!emitter.clips.contains(audioID)) {
			LOG_WARN(std::format("Audio {} does not exists", m_id_to_name[audioID]));
			return;
		}

		emitter.clips[audioID].pitch = pitch;

		LOG_DEBUG(std::format("Set pitch ({}) at clip {} on emitter {}", pitch, m_id_to_name[audioID], emitterID));

	}
	void AudioManager::SetEmitterClipPause		( EmitterHandle emitterID, AudioHandle audioID, bool paused ) {

		ValidateEmitterAndAudioID(emitterID, audioID);

		auto& emitter = m_emitters[emitterID];

		if (!emitter.clips.contains(audioID)) {
			LOG_WARN(std::format("Audio {} does not exists", m_id_to_name[audioID]));
			return;
		}

		emitter.clips[audioID].paused = paused;

		LOG_DEBUG(std::format("Set paused ({}) at clip {} on emitter {}", paused, m_id_to_name[audioID], emitterID));

	}
	void AudioManager::SetEmitterClipLoop		( EmitterHandle emitterID, AudioHandle audioID, bool loop ) {

		ValidateEmitterAndAudioID(emitterID, audioID);

		auto& emitter = m_emitters[emitterID];

		if (!emitter.clips.contains(audioID))
			return;

		emitter.clips[audioID].loop = loop;

		LOG_DEBUG(std::format("Set loop ({}) at clip {} on emitter {}", loop, m_id_to_name[audioID], emitterID));

	}
	void AudioManager::DestroyEmitter			( EmitterHandle emitterID ) {

		ValidateEmitterID(emitterID);

		m_emitters.remove(emitterID);

		LOG_INFO(std::format("Destroyed emitter {}", emitterID));

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

	std::optional<AudioHandle> AudioManager::GetIDByName( string_view name ) {
		auto it = m_name_to_id.find(cstd::StringHasher::Hash(name));
		if (it == m_name_to_id.end()) {
			LOG_WARN(std::format("Audio file named {} does not exists", name));
			return std::nullopt;
		}
		return it->second;
	}

	bool AudioManager::NameExists( string_view name ) {
		return m_name_to_id.find(cstd::StringHasher::Hash(name)) != m_name_to_id.end();
	}

};