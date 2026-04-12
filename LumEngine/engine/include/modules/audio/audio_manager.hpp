//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: High-level audio manager — sound loading, playback,
//          channel groups, effects and 3D listener management.
//
//=============================================================================//
#pragma once
#include "audio_common.hpp"
#include "ahi/core/ahi_device.hpp"
#include "entity/entity_manager.hpp"

namespace lum {

	namespace ev { class EventBus; }

	/* @brief High-level audio manager built on top of the AHI abstraction layer.
	*
	* Owns sound assets, channel groups, effects and active sound instances.
	* Wraps AHI device calls with string-based lookups via string hashing.
	* Sound instances are tracked per-entity and updated each frame via Update().
	*/
	class MAudioManager {
	public:

		MAudioManager( ) = default;
		~MAudioManager( ) { }

		/* @brief Initializes the audio manager and binds it to an AHI device.
		* @param device Pointer to an initialized AudioDevice backend.
		*/
		void Initialize( ahi::AudioDevice* device, ev::EventBus* bus );

		// -------------------------------------------------------------------------
		// Sound assets
		// -------------------------------------------------------------------------

		/* @brief Returns a handle to the sound at the given path, loading it if necessary.
		* @param relativePath Path relative to the project asset root.
		* @param cat          Sound category controlling load flags and routing.
		*/
		ahi::SoundHandle GetSound( StringView relativePath, SoundCategory cat = SoundCategory::Music );
		ahi::SoundHandle GetSound( StringView relativePath, StringView cat );

		/* @brief Unloads the sound asset at the given path and frees its handle. */
		void UnloadSound( StringView relativePath );

		/* @brief Plays a fire-and-forget sound with no instance tracking.
		* @param relativePath Path to the sound asset.
		* @param desc         Playback parameters (volume, pitch).
		*/
		void PlayOneShot( StringView relativePath, const ahi::FPlaybackDescriptor& desc );

		/* @brief Updates the 3D listener position and orientation for spatial audio.
		* Should be called once per frame with the active camera transform.
		*/
		void Set3DListenerAttributes( const ahi::FListenerAttributes& );

		/* @brief Immediately stops all currently playing sounds. */
		void StopAll( );

		// -------------------------------------------------------------------------
		// Effects
		// -------------------------------------------------------------------------

		/* @brief Creates and registers a named effect from a preset.
		* @param name   Identifier used for later lookup.
		* @param preset Built-in effect preset.
		*/
		ahi::AudioEffectHandle CreateEffect( StringView name, ahi::EffectPreset preset );

		/* @brief Creates and registers a named effect from a descriptor.
		* @param name Identifier used for later lookup.
		* @param desc Effect descriptor with per-effect parameters.
		*/
		ahi::AudioEffectHandle CreateEffect( StringView name, const ahi::FAudioEffectDescriptor& desc );

		/* @brief Destroys a registered effect by name or handle. */
		void DeleteEffect( StringView name );
		void DeleteEffect( ahi::AudioEffectHandle effect );

		/* @brief Returns the handle of a previously created effect.
		* Logs a warning and returns an invalid handle if not found.
		*/
		ahi::AudioEffectHandle GetEffect( StringView name );

		// -------------------------------------------------------------------------
		// Channel groups
		// -------------------------------------------------------------------------

		/* @brief Returns the handle of a channel group by name, creating it if necessary. */
		ahi::ChannelGroupHandle GetGroup( StringView name );

		/* @brief Attaches an effect to a channel group.
		* All sounds routed through the group will be processed by the effect.
		*/
		void SetGroupEffect( ahi::ChannelGroupHandle group, ahi::AudioEffectHandle effect );
		void SetGroupEffect( ahi::ChannelGroupHandle group, StringView effect );
		void SetGroupEffect( StringView group, ahi::AudioEffectHandle effect );
		void SetGroupEffect( StringView group, StringView effect );

		/* @brief Sets the volume of a channel group.
		* @param volume Volume scalar (0.0 = silent, 1.0 = full).
		*/
		void SetGroupVolume( StringView group, float32 volume );
		void SetGroupVolume( ahi::ChannelGroupHandle group, float32 volume );

		/* @brief Sets the pitch multiplier of a channel group. */
		void SetGroupPitch( StringView group, float32 pitch );
		void SetGroupPitch( ahi::ChannelGroupHandle group, float32 pitch );

		/* @brief Sets the global master volume for all audio output.
		* @param volume Volume scalar (0.0 = silent, 1.0 = full).
		*/
		void SetMasterVolume( float32 volume );

		// -------------------------------------------------------------------------
		// Per-frame update
		// -------------------------------------------------------------------------

		/* @brief Updates active sound instances based on entity audio components.
		* Must be called once per frame.
		*/
		void Update( ecs::MEntityManager* mgr );

	private:

		ahi::AudioDevice* mDevice = nullptr;
		ev::EventBus* mEventBus = nullptr;

		std::unordered_map<HashedStr, ahi::SoundHandle>        mSounds;
		std::unordered_map<HashedStr, ahi::ChannelGroupHandle> mGroups;
		std::unordered_map<HashedStr, ahi::AudioEffectHandle>  mEffects;
		std::unordered_map<EntityID, ahi::FSoundInstance>     mInstances;

	};

} // namespace lum