#pragma once
#include "audio_emitter.hpp"
#include "audio_manager.hpp"
#include "lum_events.hpp"
namespace audio {

	class AudioEmitterWrapper {
	public:

		AudioEmitterWrapper(detail::EmitterID _id) : id(_id) {}

		void AddClip(
			std::string_view _name, 
			float _volume = 1.f, 
			float _pitch = 1.f, 
			bool _loop = false
		) 
		{
			ev::EventBus::Global().Emit<ev::AddClipToEmitter>(
				ev::AddClipToEmitter { id, _name.data(), _volume, _pitch, _loop }
			);
		}

		void PlayClip(
			std::string_view _name, 
			float _volume = 1.f, 
			float _pitch = 1.f, 
			bool _loop = false
		) 
		{
			ev::EventBus::Global().Emit<ev::PlaySound>(
				ev::PlaySound { id, _name.data(), _volume, _pitch, _loop }
			);
		}

	private:

		detail::EmitterID id{};

	};

}