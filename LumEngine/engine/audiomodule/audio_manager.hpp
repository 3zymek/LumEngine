#pragma once
#include <vector>
#include <sparse_set.hpp>
#include <unordered_map>
#include <string>
#include "audio_define.hpp"
#include "e_define.hpp"
#include <optional>
#include "path_service.hpp"
using namespace audio::detail;
namespace audio {

	class AudioManager {

		AudioManager() {}

	public:

		static force_inline AudioManager& Global() {
			static AudioManager am;
			return am;
		}

		force_inline void Init(unsigned int _maxchannels = 512, FMOD_INITFLAGS _flags = FMOD_3D, void* _extradrive = nullptr) {
			FMOD::System_Create(&m_audio_system);
			m_audio_system->init(_maxchannels, _flags, _extradrive);
		}

		force_inline AudioID LoadSound(const std::string& _file, FMOD_MODE _mode) {
			if (!m_name_to_id.contains(_file)) {
				FMOD::Sound* sound = nullptr;
				auto path = PathService::Resolve(_file);
				m_audio_system->createSound(path.c_str(), _mode, nullptr, &sound);
				AudioID id = GenerateAudioID::Get();
				m_sounds.append(sound, id);
				m_name_to_id[_file] = id;
			}
			return m_name_to_id[_file];
		}

		force_inline void Update() {
			m_audio_system->update();
		}

		[[nodiscard]] force_inline EmitterID CreateEmitter() {
			EmitterID id = GenerateEmitterID::Get();
			AudioEmitter emitter;
			m_emitters.append(emitter, id);
			return id;
		}

		[[nodiscard]] force_inline AudioEmitter* GetEmitter(const EmitterID& _id) {
			if (!m_emitters.contains(_id)) return nullptr;
			return &m_emitters[_id];
		}

		force_inline AudioID GetIDByName(const std::string& _id) {
			auto it = m_name_to_id.find(_id);
			if (it == m_name_to_id.end()) return NULL;
			return it->second;
		}


		force_inline void Play(const EmitterID& _id, const std::string& _name, float volume, float pitch) {

		}

		force_inline void Play(const EmitterID& _id, const std::string& _name) {

		}

		force_inline void Play(const std::string& _name) {
			if (!m_name_to_id.contains(_name)) return;
			FMOD::Channel* channel = nullptr;
			c.push_back(channel);
			m_audio_system->playSound(m_sounds[m_name_to_id[_name]], nullptr, false, &channel);
		}

		force_inline void Play(const AudioID& _id) {
		
		}

	private:

		std::vector<FMOD::Channel*> c;
		FMOD::System*								m_audio_system = nullptr;
		cstd::sparse_set<AudioEmitter>				m_emitters{ MAX_EMITTERS_COUNT };
		cstd::sparse_set<FMOD::Sound*>				m_sounds{ MAX_SOUNDS_COUNT };
		std::unordered_map<std::string, AudioID>	m_name_to_id;

	};

}