#pragma once
#include "audio_emitter.hpp"
#include "lum_events.hpp"
namespace audio {

	// Wrapper to secure audio emitter and implement user-friendly API
	class AudioEmitterWrapper {
	public:

		AudioEmitterWrapper(detail::EmitterID _id) : id(_id) {}

		ev::EventEmitter event_emitter;

		void AddClip(
			std::string_view _name, 
			float _volume = 1.f, 
			float _pitch = 1.f, 
			bool _loop = false
		) 
		{
			ev::EventBus::Engine().Emit<ev::AddClipToEmitter>(
				ev::AddClipToEmitter(id, _name.data(), _volume, _pitch, _loop)
			);
		}

		void PlayClip(
			std::string_view _name, 
			float _volume = 1.f, 
			float _pitch = 1.f, 
			bool _loop = false
		) 
		{
			ev::EventBus::Engine().Emit<ev::PlaySound>(
				ev::PlaySound { id, _name.data(), _volume, _pitch, _loop }
			);
		}

		void RemoveClip(std::string_view _name) {
			ev::EventBus::Engine().Emit<ev::RemoveClipFromEmitter>(
				ev::RemoveClipFromEmitter{ id, _name.data() }
			);
		}

		std::unordered_map<std::string, detail::AudioID>& GetAllClips() {

		}

	private:

		void SubscribeEvents() {
			
			event_emitter.SubscribePermamently<ev::GetAllEmitterClips>(
				[](const ev::GetAllEmitterClips& ev) {

				}
			);

		}

		detail::EmitterID id{};

	};

}