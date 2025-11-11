#include "details/audio_manager.hpp"
#include "details/engine_events/ev_audio_events.hpp"
#include "details/ev_bus.hpp"
#include "details/audio_define.hpp"
#include "details/audio_emitter_wrapper.hpp"
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
		m_audio_system->createSound(_path.data(), _mode, nullptr, &clip.sound);

		m_sounds.append(clip, id);
		m_name_to_id.emplace(_name.data(), id);
		m_id_to_name.emplace(id, _name.data());
	}

	AudioEmitterWrapper AudioManager::CreateEmitter() 
	{
		auto id = detail::GenerateEmitterID::Get();
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

		emitter.clips.emplace(_aid, instance);

	}

	void AudioManager::PlayClip(
		detail::EmitterID _eid,
		detail::AudioID _aid,
		float _volume,
		float _pitch,
		bool _loop
	) 
	{
		detail::AudioChannel channel;
		channel.audio_clip = _aid;
		channel.emitter_id = _eid;
		m_audio_system->playSound(m_sounds[_aid].sound, nullptr, false, &channel.channel);
	}

	void AudioManager::SubscribeEvents() {
		ev::EventBus::Global().SubscribePermamently<ev::AddClipToEmitter>(
			[this](const ev::AddClipToEmitter& ev) 
			{
				AddClipToEmitter(ev.emitterID, m_name_to_id[ev.name], ev.volume, ev.pitch, ev.loop);
			});

		ev::EventBus::Global().SubscribePermamently<ev::PlaySound>(
			[this](const ev::PlaySound& ev)
			{
				PlayClip(ev.emitterID, m_name_to_id[ev.name], ev.volume, ev.pitch, ev.loop);
			}
		);

	}


}