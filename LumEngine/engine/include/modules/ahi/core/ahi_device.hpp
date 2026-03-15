//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Abstract audio device interface (AHI).
//          Defines the backend-agnostic API for sound loading,
//          playback, effects, and 3D audio.
//
//=============================================================================//
#pragma once

#include "ahi/ahi_common.hpp"
#include "core/limits.hpp"
#include "ahi/core/ahi_sound.hpp"
#include "ahi/core/ahi_effect.hpp"
#include "ahi/ahi_presets.hpp"

namespace lum::ahi {

	/* @brief Abstract audio device interface (AHI).
	*
	* Provides a backend-agnostic API for sound loading, playback, effects, and 3D audio.
	* Concrete implementations (e.g. FMODDevice) wrap a specific audio backend.
	* All heavy lifting is deferred to Update() which must be called once per frame.
	*/
	class AudioDevice {
	public:

		/* @brief Initializes the audio backend. Must be called before any other method.
		* @param maxChannels Maximum number of sounds that can play simultaneously.
		* @param flags       Initialization flags controlling system behaviour.
		*/
		virtual void Initialize( int32 maxChannels = 512, Flags<InitFlag> flags = InitFlag::Normal ) = 0;

		/* @brief Shuts down the audio backend and releases all resources. */
		virtual void Finalize( ) = 0;

		/* @brief Loads a sound asset from disk.
		* @param path   Path to the audio file.
		* @param flags  Loading and playback flags (stream, sample, 3D, loop etc.)
		* @return Handle to the loaded sound.
		*/
		virtual SoundHandle LoadSound( StringView path, Flags<SoundFlag> flags ) = 0;

		/* @brief Unloads a sound and invalidates its handle.
		* @param sound Handle to the sound to unload.
		*/
		virtual void UnloadSound( SoundHandle& sound ) = 0;

		/* @brief Creates an audio effect chain from a descriptor.
		* @param desc Descriptor containing enabled effects and their parameters.
		* @return Handle to the created effect.
		*/
		virtual AudioEffectHandle CreateEffect( const FAudioEffectDescriptor& desc ) = 0;

		/* @brief Creates an audio effect chain from a built-in preset.
		* @param preset  Named preset from AudioPreset — pass AudioPreset::Off for a no-op chain.
		* @return        Handle to the created effect, or an invalid handle if instantiation failed.
		*/
		virtual AudioEffectHandle CreateEffect( ahi::EffectPreset preset ) = 0;

		/* @brief Destroys an effect, detaching it from all groups and releasing DSP resources.
		* @param effect Handle to the effect to destroy. Invalidated after this call.
		*/
		virtual void DeleteEffect( AudioEffectHandle& effect ) = 0;

		/* @brief Creates a named channel group for routing and effect processing.
		* @param name Identifier for the group (e.g. "Music", "SFX", "Ambient").
		* @return Handle to the created channel group.
		*/
		virtual ChannelGroupHandle CreateChannelGroup( StringView name ) = 0;

		/* @brief Attaches an effect chain to a channel group.
		* All sounds routed through this group will be processed by the effect.
		* @param group  Target channel group.
		* @param effect Effect to attach.
		*/
		virtual void AddGroupEffect( ChannelGroupHandle group, AudioEffectHandle effect ) = 0;

		/* @brief Detaches an effect chain from a channel group.
		* @param group  Target channel group.
		* @param effect Effect to detach.
		*/
		virtual void RemoveGroupEffect( ChannelGroupHandle group, AudioEffectHandle effect ) = 0;

		/* @brief Plays a sound once with no instance tracking. Fire and forget.
		* @param sound Handle to the sound to play.
		* @param desc  Playback parameters (volume, pitch).
		*/
		virtual void PlayOneShot( SoundHandle sound, const FPlaybackDescriptor& desc ) = 0;

		/* @brief Plays a sound as a tracked instance, allowing runtime control.
		* @param inst  Sound instance carrying playback state (volume, pitch, position, loop).
		* @param group Channel group to route the sound through. Defaults to master.
		*/
		virtual void Play( FSoundInstance& inst, ChannelGroupHandle group = gDefaultGroup ) = 0;

		/* @brief Immediately stops all currently playing sounds. */
		virtual void StopAll( ) = 0;

		/* @brief Sets the master volume for all audio output.
		* @param volume Volume scalar (0.0 = silent, 1.0 = full).
		*/
		virtual void SetMasterVolume( float32 volume ) = 0;

		/* @brief Updates the 3D listener transform. Call once per frame with camera data.
		* @param pos     Listener position in world space.
		* @param vel     Listener velocity (used for Doppler). Pass zero if unused.
		* @param forward Forward direction vector.
		* @param up      Up direction vector.
		*/
		virtual void Set3DListenerAttributes( glm::vec3 pos, glm::vec3 vel, glm::vec3 forward, glm::vec3 up ) = 0;

		/* @brief Configures global 3D audio behaviour.
		* @param dopplerScale    Doppler effect intensity. 0 = off, 1 = realistic.
		* @param distanceFactor  How many engine units equal one metre. Default 1.0.
		* @param rolloffScale    How fast sounds attenuate with distance. Default 1.0.
		*/
		virtual void Set3DSettings(float32 dopplerScale, float32 distanceFactor, float32 rolloffScale) = 0;

		bool IsValid( SoundHandle sound )			const noexcept { return mSounds.Contains(sound); }
		bool IsValid( ChannelGroupHandle group )	const noexcept { return mChannelGroups.Contains(group); }
		bool IsValid( AudioEffectHandle effect )	const noexcept { return mEffects.Contains(effect); }

		/* @brief Synchronises all active sound instances with the audio backend.
		* Must be called once per frame. Applies dirty state (volume, pitch, position, loop, pause, stop).
		* @param instances All currently active sound instances managed by the engine.
		*/
		virtual void Update( std::vector<FSoundInstance>& instances ) = 0;

	protected:

		cstd::HandlePool<SoundHandle, vptr>	mSounds{ limits::gMaxAudioSounds };
		cstd::HandlePool<ChannelGroupHandle, vptr> mChannelGroups{ limits::gMaxChannelsGroup };
		cstd::HandlePool<AudioEffectHandle, FAudioEffect> mEffects{ limits::gMaxAudioEffects };
		std::unordered_map<InstID, vptr> mChannels;

	};

	AudioDevice* CreateDevice( AudioBackend backend );

} // namespace lum::ahi