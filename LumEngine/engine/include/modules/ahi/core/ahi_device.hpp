#pragma once

#include "ahi/ahi_common.hpp"
#include "core/limits.hpp"
#include "ahi/core/ahi_sound.hpp"
#include "ahi/core/ahi_effect.hpp"

namespace lum::ahi {

	class AudioDevice {
	public:

		virtual void Initialize() = 0;
		virtual void Finalize() = 0;

		virtual SoundHandle LoadSound(StringView path, Flags<SoundFlag> flags) = 0;
		virtual void UnloadSound(SoundHandle& sound) = 0;

		virtual AudioEffectHandle CreateEffect(const FAudioEffectDescriptor&) = 0;

		virtual ChannelGroupHandle CreateChannelGroup(StringView name) = 0;
		virtual void AddEffectToGroup(ChannelGroupHandle, AudioEffectHandle) = 0;

		virtual void PlayOneShot(SoundHandle sound, const FPlaybackDescriptor& desc) = 0;
		virtual void Play(FSoundInstance& inst, ChannelGroupHandle group = gDefaultGroup) = 0;

		virtual void StopAll() = 0;

		virtual void SetMasterVolume(float32) = 0;
		virtual void AddEffectToSound(SoundHandle, AudioEffectHandle) = 0;
		virtual void Set3DListenerPosition(glm::vec3, glm::vec3 forward, glm::vec3 up) = 0;

		bool IsValid(SoundHandle sound) const noexcept { return mSounds.Contains(sound); }
		bool IsValid(ChannelGroupHandle group) const noexcept { return mChannelGroups.Contains(group); }
		bool IsValid(AudioEffectHandle effect) const noexcept { return mEffects.Contains(effect); }

		virtual void Update(std::vector<FSoundInstance>& instances) = 0;
		
	protected:

		cstd::HandlePool<SoundHandle, vptr> mSounds{ limits::gMaxAudioSounds };
		cstd::HandlePool<ChannelGroupHandle, vptr> mChannelGroups{ limits::gMaxChannelsGroup };
		cstd::HandlePool<AudioEffectHandle, FAudioEffect> mEffects{ limits::gMaxAudioEffects };
		std::unordered_map<InstID, vptr> mChannels;


	};

}