#include "audio/audio_manager.hpp"
#include "ahi/core/ahi_device.hpp"
#include "core/utils/asset_loader.hpp"
#include "entity/entity_manager.hpp"

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
	void AudioManager::UnloadSound( StringView alias ) {

		LUM_RETURN_IF(!mSounds.contains(HashStr(alias)), LUM_SEV_WARN, "Loaded audio %s doesn't exist", String(alias).c_str());

		mSounds.erase(HashStr(alias));

	}

	ahi::InstID AudioManager::CreateInstance( StringView alias ) {

		uint64 hash = HashStr(alias);

		LUM_RETURN_IF(!mSounds.contains(hash), LUM_SEV_WARN, "Audio %s doesn't exist", String(alias).c_str());

		ahi::FSoundInstance inst;
		inst.mSound = mSounds[hash];
		mInstances.insert({ inst.mInstanceID, inst });
		
		return inst.mInstanceID;

	}

	void AudioManager::Stop(ahi::InstID instanceID) {
		
		LUM_RETURN_IF(!mInstances.contains(instanceID), LUM_SEV_WARN, "Invalid instance id");

		ahi::FSoundInstance& inst = mInstances[instanceID];

		inst.bPlaying = false;
		//inst.bDirty = true;
		
	}
	void AudioManager::SetPause(ahi::InstID instanceID, bool paused) {

		LUM_RETURN_IF(!mInstances.contains(instanceID), LUM_SEV_WARN, "Invalid instance id");

		ahi::FSoundInstance& inst = mInstances[instanceID];

		inst.bPaused = paused;
		//inst.bDirty = true;

	}
	void AudioManager::SetVolume(ahi::InstID instanceID, float32 volume) {

		LUM_RETURN_IF(!mInstances.contains(instanceID), LUM_SEV_WARN, "Invalid instance id");

		ahi::FSoundInstance& inst = mInstances[instanceID];

		inst.mVolume = volume;
		//inst.bDirty = true;

	}
	void AudioManager::SetPitch(ahi::InstID instanceID, float32 pitch) {

		LUM_RETURN_IF(!mInstances.contains(instanceID), LUM_SEV_WARN, "Invalid instance id");
		
		ahi::FSoundInstance& inst = mInstances[instanceID];

		inst.mPitch = pitch;
		//inst.bDirty = true;

	}
	void AudioManager::SetPosition(ahi::InstID instanceID, glm::vec3 pos) {

		LUM_RETURN_IF(!mInstances.contains(instanceID), LUM_SEV_WARN, "Invalid instance id");

		ahi::FSoundInstance& inst = mInstances[instanceID];

		inst.mPosition = pos;
		//inst.bDirty = true;

	}

	void AudioManager::Update( ecs::EntityManager* mgr ) {
		
		for (auto& [id, instance] : mInstances) {
			
			
			
		}
		

		
	}


}