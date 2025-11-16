#pragma once
#include "details/audio_define.hpp"

#include "utils/sparse_set.hpp"

#include "command/audio_commands.hpp"
#include "components/c_audio_emitter.hpp"
#include "details/audio_listener_wrapper.hpp"

#include <unordered_map>

#include <fmod.hpp>

namespace audio {

	namespace detail {
		struct AudioClip;
		struct AudioEmitter;
		struct AudioInstance;
		struct FMODDestructor {
			void operator()(FMOD::System* sys) {
				if (sys) sys->release();
			}
		};
	}

	struct AudioEmitterWrapper;
	struct AudioListenerWrapper;

	class AudioManager {

		using AudioID		= detail::AudioID;
		using AudioInstance = detail::AudioInstance;
		using AudioClip		= detail::AudioClip;
		using AudioEmitter	= detail::AudioEmitter;
		using AudioChannel	= detail::AudioChannel;
		using EmitterID		= detail::EmitterID;
		using EntityID		= ecs::EntityID;
		using FDestructor	= detail::FMODDestructor;
		using string_view	= std::string_view;
		using string		= std::string;

	public:

		AudioManager() {}
		~AudioManager() = default;

		void Init(
			unsigned int _maxchannels = 512, 
			FMOD_INITFLAGS _flags = FMOD_INIT_3D_RIGHTHANDED, 
			void* _extradrivers = nullptr
		);

		void LoadSound( string_view, string_view, FMOD_MODE _mode = FMOD_3D );

		AudioListenerWrapper	CreateListener	( EntityID );
		AudioEmitterWrapper		CreateEmitter	( AudioEmitterComponent* );


		// Debug

		std::optional<AudioID> GetIDByName( string_view );

		void PrintCPUUsage();


	private:

		friend class AudioSystem;
		friend class AudioEmitterWrapper;


		// Private helpers

		void AddClipToEmitter				( EmitterID, AudioID );
		void RemoveClipFromEmitter			( EmitterID, AudioID );
		void PlayEmitterClip				( EmitterID, AudioID );
		void StopEmitterClip				( EmitterID, AudioID );
		void SetEmitterClipVolume			( EmitterID, AudioID, float );
		void SetEmitterClipPitch			( EmitterID, AudioID, float );
		void SetEmitterClipPause			( EmitterID, AudioID, bool );
		void SetEmitterClipLoop				( EmitterID, AudioID, bool );
		void DestroyEmitter					( EmitterID );
		void SubscribeEvents				( );
		AudioListenerWrapper* GetListener	( );

		cmd::CommandBuffer<cmd::AudioCmd, 400> m_commands;

		std::unique_ptr< AudioListenerWrapper >			m_listener;
		std::unique_ptr< FMOD::System, FDestructor >	m_audio_system;

		cstd::sparse_set< AudioClip >		m_sounds	{ detail::MAX_SOUNDS_COUNT };
		cstd::sparse_set< AudioEmitter >	m_emitters	{ ecs::detail::MAX_ENTITY_COUNT };

		std::unordered_map< string, AudioID > m_name_to_id;
		std::unordered_map< AudioID, string > m_id_to_name;

	};

}