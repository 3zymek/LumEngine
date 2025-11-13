#pragma once
#include <string>
#include <fmod.hpp>
#include <unordered_map>
#include "utils/e_define.hpp"
#include "utils/sparse_set.hpp"
#include "entitymodule/details/ecs_define.hpp"
#include "audiomodule/details/audio_define.hpp"
#include "audiomodule/details/audio_emitter.hpp"
#include "audiomodule/details/audio_listener_wrapper.hpp"
#include "audiomodule/components/c_audio_emitter.hpp"
namespace audio {

	struct AudioEmitterWrapper;
	

	class AudioManager {
	public:

		force_inline static AudioManager& Global() {
			static AudioManager am;
			return am;
		}

		void Init(
			unsigned int _maxchannels = 512, 
			FMOD_INITFLAGS _flags = FMOD_DEFAULT, 
			void* _extradrivers = nullptr
		);

		void LoadSound(
			std::string_view,
			std::string_view,
			FMOD_MODE _mode = FMOD_3D
		);

		void AddClipToEmitter(
			detail::EmitterID, 
			detail::AudioID,
			float _volume = 1.f,
			float _pitch = 1.f,
			bool _loop = false
		);

		void PlayClip(
			detail::EmitterID,
			detail::AudioID,
			float _volume = 1.f,
			float _pitch = 1.f,
			bool _loop = false
		);

		void RemoveClipFromEmitter(
			detail::EmitterID _id,
			detail::AudioID
		);

		void DestroyEmitter(detail::EmitterID);

		cstd::sparse_set<detail::AudioEmitter>& GetAllEmitters();
		cstd::sparse_set<detail::AudioClip>&	GetAllSounds();

		AudioEmitterWrapper	CreateEmitter(AudioEmitterComponent*);
		AudioListenerWrapper* GetListener();
		FMOD::System* GetSystem();

	private:

		void CreateListener(ecs::EntityID);
		void SubscribeEvents();

		AudioListenerWrapper*	listener		= nullptr;
		FMOD::System*			m_audio_system	= nullptr;
		
		cstd::sparse_set<detail::AudioClip>		m_sounds	{ detail::MAX_SOUNDS_COUNT };
		cstd::sparse_set<detail::AudioEmitter>	m_emitters	{ ecs::detail::MAX_ENTITY_COUNT };

		std::unordered_map<std::string, detail::AudioID>	m_name_to_id;
		std::unordered_map<detail::AudioID, std::string>	m_id_to_name;

	};

}