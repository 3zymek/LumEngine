#pragma once
#include "audio/audio_manager.hpp"
#include "audio/audio_emitter.hpp"
namespace lum {
	namespace audio {

		// Wrapper to secure audio emitter and implement user-friendly API
		class AudioEmitterWrapper {

			using Type = cmd::Type;
			using AudioCmd = cmd::AudioCmd;
			using AudioManager = audio::AudioManager;

			AudioManager& manager;

		public:

			AudioEmitterWrapper( AudioManager& m, EmitterHandle emitterID ) : emitterID(emitterID), manager(m) {}

			~AudioEmitterWrapper() { manager.DestroyEmitter(emitterID); }

			/*! @brief Adds a new sound to emitter.
			*
			*  Creates and registers an sound identified by the given name (must be loaded from audio_manager.LoadFromFile()).
			*
			*  @param name Alias name of the sound to add.
			*  @return Reference to the created AudioEmitterWrapper for chaining.
			*
			*  @thread_safety Call from the main thread or audio system context.
			*/
			AudioEmitterWrapper& Add( std::string_view name );

			/*! @brief Plays the sound on emitter.
			*
			*  Starts playback of the emitter identified by the given name.
			*
			*  @param name Alias name of the sound to play.
			*  @return Reference to the AudioEmitterWrapper for chaining.
			*
			*  @thread_safety Call from the main thread or audio system context.
			*/
			AudioEmitterWrapper& Play( std::string_view name );

			/*! @brief Stops the sound.
			*
			*  Stops playback of the emitter identified by the given name.
			*
			*  @param name Alias name of the sound to stop.
			*  @return Reference to the AudioEmitterWrapper for chaining.
			*
			*  @thread_safety Call from the main thread or audio system context.
			*/
			AudioEmitterWrapper& Stop( std::string_view name );

			/*! @brief Sets the volume of a sound.
			*
			*  @param name Alias name of the Sound.
			*  @param value Volume multiplier (1.0 = original volume, 0.0 = muted).
			*  @return Reference to the AudioEmitterWrapper for chaining.
			*
			*  @thread_safety Call from the main thread or audio system context.
			*/
			AudioEmitterWrapper& SetVolume( std::string_view name, float32 value );

			/*! @brief Sets the pitch of a sound.
			*
			*  @param name Alias name of the sound.
			*  @param value Pitch multiplier (1.0 = original pitch).
			*  @return Reference to the AudioEmitterWrapper for chaining.
			*
			*  @thread_safety Call from the main thread or audio system context.
			*/
			AudioEmitterWrapper& SetPitch( std::string_view name, float32 value );

			/*! @brief Pauses or resumes a sound.
			*
			*  @param name Name of the sound.
			*  @param value True to pause, false to resume playback.
			*  @return Reference to the AudioEmitterWrapper for chaining.
			*
			*  @thread_safety Call from the main thread or audio system context.
			*/
			AudioEmitterWrapper& SetPaused( std::string_view name, bool value );

			/*! @brief Sets looping for a sound.
			*
			*  @param name Alias name of the sound.
			*  @param value True to loop indefinitely, false to play once.
			*  @return Reference to the AudioEmitterWrapper for chaining.
			*
			*  @thread_safety Call from the main thread or audio system context.
			*/
			AudioEmitterWrapper& SetLoop( std::string_view name, bool value );

			/*! @brief Gets the unique ID of this emitter.
			*
			*  @return EmitterHandle representing the internal identifier.
			*
			*  @thread_safety Safe to call from any thread.
			*/
			inline EmitterHandle GetID( ) const { return emitterID; }

			/*! @brief Removes a sound by name.
			*
			*  Removes audio clip from this emitter.
			*
			*  @param name Alias name of the sound to remove.
			*
			*  @thread_safety Call from the main thread or audio system context.
			*/
			void Remove( std::string_view name );

			/*! @brief Removes a sound by name.
			*
			*  Returns current volume of a clip on this emitter.
			*
			*  @param name Alias name of the sound.
			*  @return volume value of a clip on this emitter.
			*
			*/
			float GetVolume	( std::string_view name );

			/*! @brief Removes a sound by name.
			*
			*  Returns current pitch of a clip on this emitter.
			*
			*  @param name Alias name of the sound.
			*  @return pitch value of a clip on this emitter.
			*
			*/
			float GetPitch	( std::string_view name );

			/*! @brief Removes a sound by name.
			*
			*  Returns current paused value of a clip on this emitter.
			*
			*  @param name Alias name of the sound.
			*  @return paused value of a clip on this emitter.
			*
			*/
			bool GetPaused	( std::string_view name );

			/*! @brief Removes a sound by name.
			*
			*  Returns current looped value of a clip on this emitter.
			*
			*  @param name Alias name of the sound.
			*  @return looped value of a clip on this emitter.
			*
			*/
			bool GetLooped	( std::string_view name );


		private:

			EmitterHandle emitterID{};

		};
	}
}