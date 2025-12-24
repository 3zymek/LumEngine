#pragma once
#include "include/modules/audio/audio_common.hpp"
#include "include/modules/entity/ecs_manager.hpp"

#include "include/core/utils/sparse_set.hpp"
#include "include/modules/audio/command/audio_commands.hpp"
#include "include/modules/audio/components/c_audio_emitter.hpp"
#include "include/modules/audio/audio_listener_wrapper.hpp"

#include "include/core/core_common.hpp"
#include "include/core/core_pch.hpp"
#include "include/modules/audio/audio_pch.hpp"
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

		using AudioInstance = detail::AudioInstance;
		using AudioClip		= detail::AudioClip;
		using AudioEmitter	= detail::AudioEmitter;
		using AudioChannel	= detail::AudioChannel;
		using EntityID		= ecs::EntityID;
		using FDestructor	= detail::FMODDestructor;
		using string_view	= std::string_view;
		using string		= std::string;

	public:

		AudioManager(ecs::EntityManager& m) : m_entity_manager(m) {}
		~AudioManager() = default;


		/*! @brief Initializes the audio system.
		*
		*  Sets up FMOD with a maximum number of channels, initialization flags,
		*  and optional extra drivers. Must be called before loading or playing sounds.
		*
		*  @param maxchannels Maximum number of simultaneous audio channels (default: 512).
		*  @param flags FMOD initialization flags (default: FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED).
		*  @param extradrivers Pointer to additional platform-specific drivers (default: nullptr).
		*
		*  @thread_safety Call from main thread before any audio operations.
		*/
		void Init(
			unsigned int maxchannels = 512, 
			FMOD_INITFLAGS flags = FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED,
			void* extradrivers = nullptr
		);


		/*! @brief Loads a sound for later playback.
		 *
		 *  Loads an audio file and registers it under a name. The sound can be 2D, 3D,
		 *  looping, or streamed depending on the mode. Does not start playback.
		 *
		 *  @param alias_name Identifier to reference the sound.
		 *  @param path File path to the sound resource (WAV, MP3, OGG, etc.).
		 *  @param _mode FMOD mode flags controlling playback/streaming (default: FMOD_3D).
		 *
		 *  @remark Replaces any existing sound with the same name. 3D sounds require a listener.
		 *  @thread_safety Call from main audio thread or FMOD-safe thread.
		 */
		void LoadSound( string_view alias_name, string_view path, FMOD_MODE mode = FMOD_3D );


		/*! @brief Creates an audio listener attached to an entity.
		*
		*  Initializes a listener component for 3D audio. The listener defines
		*  the position, orientation, and velocity for 3D sound perception.
		*
		*  @param entity The entity ID to attach the listener to.
		*  @return A wrapper to manipulate the listener.
		*
		*  @thread_safety Call from main thread or audio-safe thread.
		*/
		AudioListenerWrapper	CreateListener	( EntityID );


		/*! @brief Creates an audio emitter.
		*
		*  Initializes an emitter that can play sounds in 3D space. The emitter
		*  can be attached to an entity or used standalone.
		*
		*  @param emitterComponent Pointer to the emitter component to initialize.
		*  @return A wrapper to manipulate the emitter.
		*
		*  @thread_safety Call from main thread or audio-safe thread.
		*/
		AudioEmitterWrapper		CreateEmitter	( AudioEmitterComponent* );
		AudioEmitterWrapper		CreateEmitter	( Entity );

		std::optional<AudioHandle>	GetIDByName	( string_view );
		bool						NameExists	( string_view );

	private:

		AudioManager(AudioManager&&) = delete;
		AudioManager&& operator=(AudioManager&&) = delete;

		friend class AudioSystem;
		friend class AudioEmitterWrapper;

		////////////////////////////////////
		/// Private Helpers
		////////////////////////////////////

		void AddClipToEmitter				( EmitterHandle, AudioHandle );
		void RemoveClipFromEmitter			( EmitterHandle, AudioHandle );
		void PlayEmitterClip				( EmitterHandle, AudioHandle );
		void StopEmitterClip				( EmitterHandle, AudioHandle );
		void SetEmitterClipVolume			( EmitterHandle, AudioHandle, float );
		void SetEmitterClipPitch			( EmitterHandle, AudioHandle, float );
		void SetEmitterClipPause			( EmitterHandle, AudioHandle, bool );
		void SetEmitterClipLoop				( EmitterHandle, AudioHandle, bool );
		void DestroyEmitter					( EmitterHandle );
		void SubscribeEvents				( );
		AudioListenerWrapper* GetListener	( );

		inline bool ValidateEmitterID			( EmitterHandle id ) noexcept {

			if (!m_emitters.contains(id)) {
				LOG_WARN("Emitter does not exist");
				return false;
			}
			return true;

		}
		inline bool ValidateAudioID				( AudioHandle id ) noexcept {

			if (!m_sounds.contains(id)) {
				LOG_WARN("Sound does not exist");
				return false;
			}
			return true;

		}
		inline bool ValidateEmitterAndAudioID	( EmitterHandle emitterID, AudioHandle audioID ) noexcept {

			return ValidateEmitterID(emitterID) && ValidateAudioID(audioID);

		}

		ecs::EntityManager& m_entity_manager;

		cmd::CommandBuffer<cmd::AudioCmd, 400> m_commands;

		std::unique_ptr< AudioListenerWrapper >			m_listener;
		std::unique_ptr< FMOD::System, FDestructor >	m_audio_system;

		cstd::sparse_set< AudioClip >		m_sounds	{ MAX_SOUNDS_COUNT };
		cstd::sparse_set< AudioEmitter >	m_emitters	{ ecs::detail::MAX_ENTITY_COUNT };

		std::unordered_map< uint64_t, AudioHandle > m_name_to_id;
		#ifdef ENABLE_LOGGER
			std::unordered_map< AudioHandle, string >	m_id_to_name;
		#endif
	};

}