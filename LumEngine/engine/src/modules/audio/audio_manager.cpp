#include "audio/audio_manager.hpp"
#include "ahi/core/ahi_device.hpp"
#include "core/utils/asset_loader.hpp"

namespace lum::audio {

	void AudioManager::Initialize( ahi::AudioDevice* device ) {

		mDevice = device;

	}

	void AudioManager::LoadSound( StringView relativePath, StringView alias, SoundCategory cat ) {

		LUM_RETURN_IF(mSounds.contains(HashStr(alias)), LUM_SEV_WARN, "Audio aliased %s already exist", String(alias).c_str());

		String fullPath = AssetLoader::ResolvePath(RootID::External, relativePath);
		LUM_RETURN_IF(fullPath.empty(), LUM_SEV_ERROR, "Couldn't load audio file: %s", AssetLoader::GetErrorMessage());
		
		ahi::SoundHandle handle = mDevice->LoadSound(fullPath, GetSoundFlags(cat));
		
		mSounds.insert({ HashStr(alias), handle });
		
	}
	void AudioManager::UnloadSound(StringView alias) {

		LUM_RETURN_IF(!mSounds.contains(HashStr(alias)), LUM_SEV_WARN, "Loaded audio doesn't exist", String(alias).c_str());

		mSounds.erase(HashStr(alias));

	}

	void AudioManager::CreateInstance(StringView alias) {

	}

	void AudioManager::SetVolume() {}
	void AudioManager::SetPitch() {}
	void AudioManager::SetPosition() {}


}