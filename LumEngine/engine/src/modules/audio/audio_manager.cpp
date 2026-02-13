#include "audio/audio_manager.hpp"
#include "event/event_bus.hpp"
#include "entity/ecs_common.hpp"
#include "audio/audio_common.hpp"
#include "entity/entity.hpp"

#include "audio/audio_emitter_wrapper.hpp"
#include "audio/audio_listener_wrapper.hpp"
#include "core/utils/string_hasher.hpp"

#include "core/utils/asset_service.hpp"
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

			m_audio_system.reset(sys);
			SubscribeEvents();

		}

		void AudioManager::LoadSound(string_view alias_name, string_view path, FMOD_MODE mode) {

			if (NameExists(alias_name)) {
				LUM_LOG_ERROR("Audio Clip with name {} already exist");
				return;
			}

			auto id = GenerateID<AudioHandle, detail::AUDIO_ID_NULL>::Get();
			auto hashed_new_id = HashStr(alias_name);

			AudioClip clip;
			FMOD_ASSERT(
				m_audio_system->createSound(
					lum::AssetService::LoadAudio(path).c_str(),
					mode,
					nullptr,
					&clip.sound
				)
			);

			m_sounds.emplace(clip, id);
			m_name_to_id.emplace(hashed_new_id, id);

			#ifdef LUM_ENABLE_LOGGER
				m_id_to_name.emplace(id, alias_name.data());
			#endif

			LUM_LOG_INFO("Audio {} created");

		}

		AudioListenerWrapper	AudioManager::CreateListener(EntityID entityID) {

			if (!m_listener) {

				m_listener = std::make_unique<AudioListenerWrapper>(entityID);

				m_listener->transform_component	= m_entity_manager.GetComponent<ecs::components::TransformComponent>(entityID);
				m_listener->listener_component	= m_entity_manager.GetComponent<ecs::components::AudioListenerComponent>(entityID);

				LUM_LOG_INFO("Audio Listener created");

			}

			return *m_listener;
		}
		AudioEmitterWrapper		AudioManager::CreateEmitter(Entity entity) {

			if (!entity.Has<AudioEmitterComponent>())
				entity.AddComponent<AudioEmitterComponent>();

			auto id = GenerateID<EmitterHandle, detail::EMITTER_ID_NULL>::Get();
			entity.GetComponent<AudioEmitterComponent>()->emitterID = id;
			detail::AudioEmitter emitter;
			emitter.transform = m_entity_manager.GetComponent<ecs::components::TransformComponent>(entity.GetID());
			AudioEmitterWrapper wrapper(*this, id);

			m_emitters.emplace(emitter, id);

			LUM_LOG_INFO("Audio Emitter created");

			return wrapper;
		}




		////////////////////////////////////
		/// Private API
		////////////////////////////////////

		void AudioManager::AddClipToEmitter(EmitterHandle emitterID, AudioHandle audioID) {

			ValidateEmitterAndAudioID(emitterID, audioID);

			auto& emitter = m_emitters[emitterID];

			if (emitter.clips.contains(audioID))
				return;

			detail::AudioInstance instance{};
			instance.id = audioID;

			emitter.clips.emplace(audioID, std::move(instance));

			LUM_LOG_DEBUG("Added clip {} to emitter {}");

		}
		void AudioManager::RemoveClipFromEmitter(EmitterHandle emitterID, AudioHandle audioID) {

			ValidateEmitterAndAudioID(emitterID, audioID);

			auto& emitter = m_emitters[emitterID];
			emitter.clips.erase(audioID);

			LUM_LOG_DEBUG("Removed clip {} from emitter {}");
		}
		void AudioManager::PlayEmitterClip(EmitterHandle emitterID, AudioHandle audioID) {

			ValidateEmitterAndAudioID(emitterID, audioID);

			auto& emitter = m_emitters[emitterID];

			if (!emitter.clips.contains(audioID))
				return;

			AudioChannel channel;
			channel.audio_clip = audioID;
			channel.emitter_id = emitterID;


			m_audio_system->playSound(m_sounds[audioID].sound, nullptr, false, &channel.channel);

			emitter.active_clips.push_back(std::move(channel));

			LUM_LOG_DEBUG("Played clip {} on emitter {}");

		}
		void AudioManager::StopEmitterClip(EmitterHandle emitterID, AudioHandle audioID) {

			ValidateEmitterAndAudioID(emitterID, audioID);

			auto& emitter = m_emitters[emitterID];

			if (!emitter.clips.contains(audioID)) {
				LUM_LOG_WARN("Audio {} does not exists");
				return;
			}

			for (auto& active_clip : emitter.active_clips) {
				if (active_clip.audio_clip == audioID) {
					active_clip.channel->stop();
					std::swap(emitter.active_clips.back(), active_clip);
					emitter.active_clips.pop_back();
					emitter.transform = nullptr;
				}
			}

			LUM_LOG_DEBUG("Stopped clip {} on emitter {}");

		}
		void AudioManager::SetEmitterClipVolume(EmitterHandle emitterID, AudioHandle audioID, float32 volume) {

			ValidateEmitterAndAudioID(emitterID, audioID);

			auto& emitter = m_emitters[emitterID];

			if (!emitter.clips.contains(audioID)) {
				LUM_LOG_WARN("Audio {} does not exists");
				return;
			}

			emitter.clips[audioID].volume = volume;

			LUM_LOG_DEBUG("Set volume ({}) at clip {} on emitter {}");

		}
		void AudioManager::SetEmitterClipPitch(EmitterHandle emitterID, AudioHandle audioID, float32 pitch) {

			ValidateEmitterAndAudioID(emitterID, audioID);

			auto& emitter = m_emitters[emitterID];

			if (!emitter.clips.contains(audioID)) {
				LUM_LOG_WARN("Audio {} does not exists");
				return;
			}

			emitter.clips[audioID].pitch = pitch;

			LUM_LOG_DEBUG("Set pitch ({}) at clip {} on emitter {}");

		}
		void AudioManager::SetEmitterClipPause(EmitterHandle emitterID, AudioHandle audioID, bool paused) {

			ValidateEmitterAndAudioID(emitterID, audioID);

			auto& emitter = m_emitters[emitterID];

			if (!emitter.clips.contains(audioID)) {
				LUM_LOG_WARN("Audio {} does not exists");
				return;
			}

			emitter.clips[audioID].paused = paused;

			LUM_LOG_DEBUG("Set paused ({}) at clip {} on emitter {}");

		}
		void AudioManager::SetEmitterClipLoop(EmitterHandle emitterID, AudioHandle audioID, bool loop) {

			ValidateEmitterAndAudioID(emitterID, audioID);

			auto& emitter = m_emitters[emitterID];

			if (!emitter.clips.contains(audioID))
				return;

			emitter.clips[audioID].loop = loop;

			LUM_LOG_DEBUG("Set loop ({}) at clip {} on emitter {}");

		}
		void AudioManager::DestroyEmitter(EmitterHandle emitterID) {

			ValidateEmitterID(emitterID);

			m_emitters.remove(emitterID);

			LUM_LOG_INFO("Destroyed emitter {}");

		}
		float32 AudioManager::GetEmitterClipVolume(EmitterHandle emitterID, AudioHandle audioID) {
			return m_emitters[emitterID].clips[audioID].volume;
		}
		float32 AudioManager::GetEmitterClipPitch(EmitterHandle emitterID, AudioHandle audioID) {
			return m_emitters[emitterID].clips[audioID].pitch;
		}
		bool AudioManager::GetEmitterClipLooped(EmitterHandle emitterID, AudioHandle audioID) {
			return m_emitters[emitterID].clips[audioID].loop;
		}
		bool AudioManager::GetEmitterClipPaused(EmitterHandle emitterID, AudioHandle audioID) {
			return m_emitters[emitterID].clips[audioID].paused;
		}
		void AudioManager::SubscribeEvents() {
			m_event_bus.SubscribePermanently<ev::ComponentAdded>(
					[this](const ev::ComponentAdded& ev)
					{
						if (ev.component_typeID == ecs::detail::ComponentTypeID::Get<ecs::components::AudioListenerComponent>() or
							ev.component_typeID == ecs::detail::ComponentTypeID::Get<ecs::components::AudioEmitterComponent>()) {

							m_entity_manager.RequireComponent<ecs::components::TransformComponent>(ev.entityID);

						}

					}
				);
		}

		AudioListenerWrapper* AudioManager::GetListener() {
			return m_listener.get();
		}

		std::optional<AudioHandle> AudioManager::GetIDByName(string_view name) {
			auto it = m_name_to_id.find(HashStr(name));
			if (it == m_name_to_id.end()) {
				LUM_LOG_WARN("Audio file named {} does not exists");
				return std::nullopt;
			}
			return it->second;
		}

		bool AudioManager::NameExists(string_view name) {
			return m_name_to_id.find(HashStr(name)) != m_name_to_id.end();
		}
	};
}