#include "audio/audio_manager.hpp"
#include "event/event_bus.hpp"
#include "entity/ecs_common.hpp"
#include "audio/audio_common.hpp"
#include "entity/entity.hpp"

#include "audio/audio_emitter_wrapper.hpp"
#include "audio/audio_listener_wrapper.hpp"
#include "core/utils/string_hasher.hpp"

#include "core/utils/asset_loader.hpp"
#include "modules/audio/audio_system.hpp"
#include "modules/audio/components/c_audio_emitter.hpp"
#include "modules/audio/components/c_audio_listener.hpp"
namespace lum {
	namespace audio {

		////////////////////////////////////
		/// Public API
		////////////////////////////////////

		void AudioManager::Init(uint32 maxchannels, AudioInitFlags flags, vptr extradrivers) {

			FMOD::System* sys = nullptr;
			static_cast<FMOD_INITFLAGS>(flags);
			FMOD::System_Create(&sys);
			sys->init(maxchannels, FMOD_INIT_3D_RIGHTHANDED, extradrivers);

			mAudioSystem.reset(sys);
			SubscribeEvents();

		}

		void AudioManager::LoadSound(StringView alias_name, StringView path, FMOD_MODE mode) {

			if (NameExists(alias_name)) {
				LUM_LOG_ERROR("Audio Clip with name {} already exist");
				return;
			}

			auto id = GenerateID<AudioID, detail::gAudioNullID>::Get();
			auto hashed_new_id = HashStr(alias_name);

			bool success;
			AudioClip clip;
			// TODO REPAIR
			/*
			FMOD_ASSERT(
				mAudioSystem->createSound(
					lum::AssetLoader::LoadAudio(path, success).c_str(),
					mode,
					nullptr,
					&clip.mSound
				)
			);
			*/

			mSounds.Append(clip, id);
			mNameToID.emplace(hashed_new_id, id);

			#ifdef LUM_ENABLE_LOGGER
				mIDToName.emplace(id, alias_name.data());
			#endif

			LUM_LOG_INFO("Audio {} created");

		}

		AudioListenerWrapper	AudioManager::CreateListener(EntityID entityID) {

			if (!mListener) {

				mListener = std::make_unique<AudioListenerWrapper>(entityID);

				//mListener->mTransform	= mEntityManager.GetComponent<CTransform>(entityID);
				//mListener->mListener	= mEntityManager.GetComponent<CAudioListener>(entityID);

				LUM_LOG_INFO("Audio Listener created");

			}

			return *mListener;
		}
		AudioEmitterWrapper		AudioManager::CreateEmitter(Entity entity) {

			//if (!entity.Has<AudioEmitterComponent>())
				//entity.AddComponent<AudioEmitterComponent>();

			//auto id = GenerateID<EmitterID, detail::gEmitterNullID>::Get();
			//entity.GetComponent<AudioEmitterComponent>()->emitterID = id;
			//detail::AudioEmitter emitter;
			//emitter.transform = mEntityManager.GetComponent<CTransform>(entity.GetID());
			//AudioEmitterWrapper wrapper(*this, id);

			//mEmitters.Emplace(emitter, id);

			LUM_LOG_INFO("Audio Emitter created");
			return {*this, 1};
			//return wrapper;
		}




		////////////////////////////////////
		/// Private API
		////////////////////////////////////

		void AudioManager::AddClipToEmitter(EmitterID emitterID, AudioID audioID) {

			ValidateEmitterAndAudioID(emitterID, audioID);

			AudioEmitter& emitter = mEmitters[emitterID];

			if (emitter.clips.contains(audioID))
				return;

			detail::AudioInstance instance{};
			instance.id = audioID;

			emitter.clips.emplace(audioID, std::move(instance));

			LUM_LOG_DEBUG("Added clip {} to emitter {}");

		}
		void AudioManager::RemoveClipFromEmitter(EmitterID emitterID, AudioID audioID) {

			ValidateEmitterAndAudioID(emitterID, audioID);

			AudioEmitter& emitter = mEmitters[emitterID];
			emitter.clips.erase(audioID);

			LUM_LOG_DEBUG("Removed clip {} from emitter {}");
		}
		void AudioManager::PlayEmitterClip(EmitterID emitterID, AudioID audioID) {

			ValidateEmitterAndAudioID(emitterID, audioID);

			AudioEmitter& emitter = mEmitters[emitterID];

			if (!emitter.clips.contains(audioID))
				return;

			AudioChannel channel;
			channel.mAudioClip = audioID;
			channel.mEmitterID = emitterID;


			mAudioSystem->playSound(mSounds[audioID].mSound, nullptr, false, &channel.mChannel);

			emitter.active_clips.push_back(std::move(channel));

			LUM_LOG_DEBUG("Played clip {} on emitter {}");

		}
		void AudioManager::StopEmitterClip(EmitterID emitterID, AudioID audioID) {

			ValidateEmitterAndAudioID(emitterID, audioID);

			AudioEmitter& emitter = mEmitters[emitterID];

			if (!emitter.clips.contains(audioID)) {
				LUM_LOG_WARN("Audio {} does not exists");
				return;
			}

			for (auto& active_clip : emitter.active_clips) {
				if (active_clip.mAudioClip == audioID) {
					active_clip.mChannel->stop();
					std::swap(emitter.active_clips.back(), active_clip);
					emitter.active_clips.pop_back();
					emitter.transform = nullptr;
				}
			}

			LUM_LOG_DEBUG("Stopped clip {} on emitter {}");

		}
		void AudioManager::SetEmitterClipVolume(EmitterID emitterID, AudioID audioID, float32 volume) {

			ValidateEmitterAndAudioID(emitterID, audioID);

			AudioEmitter& emitter = mEmitters[emitterID];

			if (!emitter.clips.contains(audioID)) {
				LUM_LOG_WARN("Audio {} does not exists");
				return;
			}

			emitter.clips[audioID].volume = volume;

			LUM_LOG_DEBUG("Set volume ({}) at clip {} on emitter {}");

		}
		void AudioManager::SetEmitterClipPitch(EmitterID emitterID, AudioID audioID, float32 pitch) {

			ValidateEmitterAndAudioID(emitterID, audioID);

			AudioEmitter& emitter = mEmitters[emitterID];

			if (!emitter.clips.contains(audioID)) {
				LUM_LOG_WARN("Audio {} does not exists");
				return;
			}

			emitter.clips[audioID].pitch = pitch;

			LUM_LOG_DEBUG("Set pitch ({}) at clip {} on emitter {}");

		}
		void AudioManager::SetEmitterClipPause(EmitterID emitterID, AudioID audioID, bool paused) {

			ValidateEmitterAndAudioID(emitterID, audioID);

			AudioEmitter& emitter = mEmitters[emitterID];

			if (!emitter.clips.contains(audioID)) {
				LUM_LOG_WARN("Audio {} does not exists");
				return;
			}

			emitter.clips[audioID].paused = paused;

			LUM_LOG_DEBUG("Set paused ({}) at clip {} on emitter {}");

		}
		void AudioManager::SetEmitterClipLoop(EmitterID emitterID, AudioID audioID, bool loop) {

			ValidateEmitterAndAudioID(emitterID, audioID);

			AudioEmitter& emitter = mEmitters[emitterID];

			if (!emitter.clips.contains(audioID))
				return;

			emitter.clips[audioID].loop = loop;

			LUM_LOG_DEBUG("Set loop ({}) at clip {} on emitter {}");

		}
		void AudioManager::DestroyEmitter(EmitterID emitterID) {

			validate_emitter_id(emitterID);

			mEmitters.Remove(emitterID);

			LUM_LOG_INFO("Destroyed emitter {}");

		}
		float32 AudioManager::GetEmitterClipVolume(EmitterID emitterID, AudioID audioID) {
			return mEmitters[emitterID].clips[audioID].volume;
		}
		float32 AudioManager::GetEmitterClipPitch(EmitterID emitterID, AudioID audioID) {
			return mEmitters[emitterID].clips[audioID].pitch;
		}
		bool AudioManager::GetEmitterClipLooped(EmitterID emitterID, AudioID audioID) {
			return mEmitters[emitterID].clips[audioID].loop;
		}
		bool AudioManager::GetEmitterClipPaused(EmitterID emitterID, AudioID audioID) {
			return mEmitters[emitterID].clips[audioID].paused;
		}
		void AudioManager::SubscribeEvents() {
			/*
			mEventBus.SubscribePermanently<ev::ComponentAdded>(
					[this](const ev::ComponentAdded& ev)
					{
						if (ev.component_typeID == ecs::detail::ComponentTypeID::Get<CAudioListener>() or
							ev.component_typeID == ecs::detail::ComponentTypeID::Get<CAudioEmitter>()) {

							mEntityManager.RequireComponent<CTransform>(ev.entityID);

						}

					}
				);
				*/
		}

		AudioListenerWrapper* AudioManager::GetListener() {
			return mListener.get();
		}

		std::optional<AudioID> AudioManager::GetIDByName(StringView name) {
			auto it = mNameToID.find(HashStr(name));
			if (it == mNameToID.end()) {
				LUM_LOG_WARN("Audio file named {} does not exists");
				return std::nullopt;
			}
			return it->second;
		}

		bool AudioManager::NameExists(StringView name) {
			return mNameToID.find(HashStr(name)) != mNameToID.end();
		}
	};
}