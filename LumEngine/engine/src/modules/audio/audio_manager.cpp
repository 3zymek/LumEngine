#include "audio/audio_manager.hpp"
#include "ahi/core/ahi_device.hpp"
#include "core/utils/asset_loader.hpp"
#include "entity/entity_manager.hpp"
#include "entity/components/transform.hpp"
#include "entity/components/audio_emitter.hpp"

namespace lum::audio {

	void MAudioManager::Initialize( ahi::AudioDevice* device ) {

		mDevice = device;

	}

	void MAudioManager::LoadSound( StringView relativePath, StringView alias, SoundCategory cat ) {

		LUM_RETURN_IF(mSounds.contains(HashStr(alias)), LUM_SEV_WARN, "Audio aliased %s already exist", String(alias).c_str());

		String fullPath = AssetLoader::ResolvePath(RootID::External, relativePath);
		LUM_RETURN_IF(fullPath.empty(), LUM_SEV_ERROR, "Couldn't load audio file: %s", AssetLoader::GetErrorMessage());
		
		ahi::SoundHandle handle = mDevice->LoadSound(fullPath, GetSoundFlags(cat));
		
		mSounds.insert({ HashStr(alias), handle });
		
	}
	void MAudioManager::UnloadSound( StringView alias ) {

		LUM_RETURN_IF(!mSounds.contains(HashStr(alias)), LUM_SEV_WARN, "Loaded audio %s doesn't exist", String(alias).c_str());

		mSounds.erase(HashStr(alias));

	}

	ahi::SoundHandle MAudioManager::GetSound( StringView alias ) {

		uint64 hash = HashStr(alias);

		if (mSounds.contains(hash)) {

			return mSounds[hash];

		}
		
		return {};

	}

	void MAudioManager::Stop(ahi::InstID instanceID) {
		
		LUM_RETURN_IF(!mInstances.contains(instanceID), LUM_SEV_WARN, "Invalid instance id");

		ahi::FSoundInstance& inst = mInstances[instanceID];

		inst.bPlaying = false;
		//inst.bDirty = true;
		
	}
	void MAudioManager::SetPause(ahi::InstID instanceID, bool paused) {

		LUM_RETURN_IF(!mInstances.contains(instanceID), LUM_SEV_WARN, "Invalid instance id");

		ahi::FSoundInstance& inst = mInstances[instanceID];

		inst.bPaused = paused;
		//inst.bDirty = true;

	}
	void MAudioManager::SetVolume(ahi::InstID instanceID, float32 volume) {

		LUM_RETURN_IF(!mInstances.contains(instanceID), LUM_SEV_WARN, "Invalid instance id");

		ahi::FSoundInstance& inst = mInstances[instanceID];

		inst.mVolume = volume;
		//inst.bDirty = true;

	}
	void MAudioManager::SetPitch(ahi::InstID instanceID, float32 pitch) {

		LUM_RETURN_IF(!mInstances.contains(instanceID), LUM_SEV_WARN, "Invalid instance id");
		
		ahi::FSoundInstance& inst = mInstances[instanceID];

		inst.mPitch = pitch;
		//inst.bDirty = true;

	}
	void MAudioManager::SetPosition(ahi::InstID instanceID, glm::vec3 pos) {

		LUM_RETURN_IF(!mInstances.contains(instanceID), LUM_SEV_WARN, "Invalid instance id");

		ahi::FSoundInstance& inst = mInstances[instanceID];

		inst.mPosition = pos;
		//inst.bDirty = true;

	}

	void MAudioManager::Update( ecs::MEntityManager* mgr ) {

		
		mgr->EachWithID<CTransform, CAudioEmitter>([&]( ecs::EntityID id, CTransform& transf, CAudioEmitter& emitter ) {

			if (emitter.bMarked) {

				auto& inst = mInstances[id];

				inst.mVolume = emitter.mVolume;
				inst.mPitch = emitter.mPitch;
				inst.mMinDistance = emitter.mMinDistance;
				inst.mMaxDistance = emitter.mMaxDistance;
				inst.bPaused = emitter.bPaused;
				inst.bPlaying = emitter.bPlaying;
				inst.bLooped = emitter.bLooped;
				inst.mSound = emitter.mSound;

				inst.mPosition = transf.mPosition;

				mDevice->Update(inst);

			}

			emitter.bMarked = false;

		});
		

		
	}


}