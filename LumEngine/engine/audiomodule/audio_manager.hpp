#pragma once
#include <string>
#include <fmod.hpp>
#include <unordered_map>
#include "audio_define.hpp"
#include "utils/e_define.hpp"
#include "utils/sparse_set.hpp"
namespace audio {

	class AudioManager {

		AudioManager() {}

	public:

		force_inline static AudioManager& Global() {
			static AudioManager am;
			return am;
		}
		force_inline void Update() {
			m_audio_system->update();
		}

		void Init(
			unsigned int _maxchannels = 512, 
			FMOD_INITFLAGS _flags = FMOD_3D, 
			void* _extradrive = nullptr
		);

		void Play(
			detail::EmitterID _id,
			const std::string& _name, 
			float volume = 1.f, 
			float pitch = 1.f
		);

		void LoadSound(
			const std::string&,
			const std::string&, 
			float, 
			float, 
			FMOD_MODE
		);

		void Play(const std::string&);

		[[nodiscard]] detail::EmitterID		CreateEmitter	();
		[[nodiscard]] detail::AudioEmitter* GetEmitter		(detail::EmitterID);

	private:

		FMOD::System*										m_audio_system = nullptr;
		cstd::sparse_set<detail::AudioEmitter>				m_emitters{ detail::MAX_EMITTERS_COUNT};
		cstd::sparse_set<detail::SoundAsset>				m_sounds{ detail::MAX_SOUNDS_COUNT };
		std::unordered_map<std::string, detail::AudioID>	m_name_to_id;

	};

}