#pragma once

#include "ahi/ahi_common.hpp"
#include "core/limits.hpp"
#include "ahi/core/ahi_clip.hpp"

namespace lum::ahi {

	class AudioDevice {
	public:

		virtual void Initialize() = 0;
		virtual void Finalize() = 0;

		virtual SoundHandle LoadSound(StringView path) = 0;
		virtual void UnloadSound(SoundHandle& sound) = 0;

		virtual ChannelGroupHandle CreateChannelGroup(StringView name) = 0;

		virtual void PlayOneShot(SoundHandle sound, const FPlaybackDescriptor& desc) = 0;
		virtual void Play(const FSoundInstance& inst) = 0;

		virtual void Update(std::vector<FSoundInstance>& instances) = 0;
		
	protected:

		cstd::HandlePool<SoundHandle, vptr> mSounds{ limits::gMaxAudioSounds };
		cstd::HandlePool<ChannelGroupHandle, vptr> mChannelGroups{ limits::gMaxChannelsGroup };
		std::unordered_map<InstID, vptr> mChannels;


	};

}