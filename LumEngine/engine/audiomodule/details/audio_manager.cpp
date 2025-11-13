#include "details/audio_manager.hpp"
#include "details/engine_events/ev_audio_events.hpp"
#include "details/ev_bus.hpp"
#include "details/audio_define.hpp"
#include "details/audio_emitter_wrapper.hpp"
#include "utils/path_service.hpp"
#include "systems/sys_audio.hpp"
#include "entitymodule/details/ecs_define.hpp"
#include "audiomodule/details/audio_listener_wrapper.hpp"
#include "audiomodule/components/c_audio_listener.hpp"
#include "audiomodule/components/c_audio_emitter.hpp"
namespace audio {


	void AudioManager::Init(
		unsigned int _maxchannels,
		FMOD_INITFLAGS _flags,
		void* _extradrivers
	)
	{
		FMOD::System_Create(&m_audio_system);
		m_audio_system->init(_maxchannels, _flags, _extradrivers);
		SubscribeEvents();
	}


	void AudioManager::LoadSound(
		std::string_view _name,
		std::string_view _path,
		FMOD_MODE _mode
	)
	{
		if (m_name_to_id.contains(_name.data())) return;

		auto id = detail::GenerateAudioID::Get();

		detail::AudioClip clip;
		m_audio_system->createSound(
			PathService::Resolve(_path.data()).c_str(), 
			_mode, nullptr, 
			&clip.sound
		);

		m_sounds.append(clip, id);
		m_name_to_id.emplace(_name.data(), id);
		m_id_to_name.emplace(id, _name.data());
	}


	AudioEmitterWrapper AudioManager::CreateEmitter(AudioEmitterComponent* _comp) 
	{
		auto id = detail::GenerateEmitterID::Get();
		_comp->emitterID = id;
		detail::AudioEmitter emitter;
		AudioEmitterWrapper wrapper(id);

		m_emitters.append(emitter, id);

		return wrapper;
	}


	void AudioManager::AddClipToEmitter(
		detail::EmitterID _eid,
		detail::AudioID _aid,
		float _volume,
		float _pitch,
		bool _loop
	)
	{
		if (!m_emitters.contains(_eid)) 
			return;
		
		auto& emitter = m_emitters[_eid];

		if (emitter.clips.contains(_aid))
			return;

		detail::AudioInstance instance;
		instance.id		= _aid;
		instance.volume	= _volume;
		instance.pitch	= _pitch;
		instance.loop	= _loop;

		emitter.clips.emplace(_aid, std::move(instance));

	}


	void AudioManager::PlayClip(
		detail::EmitterID _eid,
		detail::AudioID _aid,
		float _volume,
		float _pitch,
		bool _loop
	) 
	{
		if (!m_emitters.contains(_eid) or !m_sounds.contains(_aid))
			return;

		auto& emitter = m_emitters[_eid];

		if (!emitter.clips.contains(_aid))
			return;
		

		detail::AudioChannel channel;
		channel.audio_clip = _aid;
		channel.emitter_id = _eid;
		
		m_audio_system->playSound(m_sounds[_aid].sound, nullptr, _loop, &channel.channel);

		channel.channel->setVolume(_volume);
		channel.channel->setPitch(_pitch);

		emitter.active_clips.push_back(std::move(channel));
	}


	void AudioManager::RemoveClipFromEmitter(
		detail::EmitterID _eid, 
		detail::AudioID _aid
	) 
	{
		if (!m_emitters.contains(_eid) or !m_sounds.contains(_aid)) 
			return;

		auto& emitter = m_emitters[_eid];
		emitter.clips.erase(_aid);
	}


	cstd::sparse_set<detail::AudioEmitter>& AudioManager::GetAllEmitters() {
		return m_emitters;
	}


	void AudioManager::DestroyEmitter(detail::EmitterID _eid) {
		if (m_emitters.contains(_eid))
			m_emitters.remove(_eid);
	}

	AudioListenerWrapper* AudioManager::GetListener() {
		if (listener) return listener;
	}

	FMOD::System* AudioManager::GetSystem() {
		if (m_audio_system) return m_audio_system;
	}

	void AudioManager::CreateListener(ecs::EntityID _enid) {
		if (listener) return;
		AudioListenerWrapper wrapper(_enid);
		listener = &wrapper;
	}

	void AudioManager::SubscribeEvents() {

		ev::EventBus::Engine().SubscribePermamently<ev::AddClipToEmitter>(
			[this](const ev::AddClipToEmitter& ev) 
			{
				AddClipToEmitter(ev.emitterID, m_name_to_id[ev.name], ev.volume, ev.pitch, ev.loop);
			});

		ev::EventBus::Engine().SubscribePermamently<ev::PlaySound>(
			[this](const ev::PlaySound& ev)
			{
				PlayClip(ev.emitterID, m_name_to_id[ev.name], ev.volume, ev.pitch, ev.loop);
			}
		);

		ev::EventBus::Engine().SubscribePermamently<ev::RemoveClipFromEmitter>(
			[this](const ev::RemoveClipFromEmitter& ev)
			{
				RemoveClipFromEmitter(ev.emitterID, m_name_to_id[ev.name]);
			}
		);

		ev::EventBus::Engine().SubscribePermamently<ev::DestroyEmitter>(
			[this](const ev::DestroyEmitter& ev)
			{
				DestroyEmitter(ev.emitterID);
			}
		);

		ev::EventBus::Engine().SubscribePermamently<ev::ComponentAdded>(
			[this](const ev::ComponentAdded& ev)
			{
				if (ev.component_typeID == ecs::detail::ComponentTypeIndex::get<AudioListenerComponent>())
					CreateListener(ev.entityID);
			}
		);

	}


}