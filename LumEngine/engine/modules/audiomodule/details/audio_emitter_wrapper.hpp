#pragma once
#include "audiomodule/details/audio_manager.hpp"
#include "details/audio_emitter.hpp"
namespace audio {

	// Wrapper to secure audio emitter and implement user-friendly API
	class AudioEmitterWrapper {

		using string_view	= std::string_view;
		using Type			= cmd::Type;
		using AudioCmd		= cmd::AudioCmd;
		using AudioManager	= audio::AudioManager;

		AudioManager& manager;

	public:

		AudioEmitterWrapper( AudioManager& m, EmitterHandle emitterID ) : emitterID( emitterID ), manager( m ) {}

		~AudioEmitterWrapper() { manager.DestroyEmitter(emitterID); }

		AudioEmitterWrapper& Add		( string_view );
		AudioEmitterWrapper& Play		( string_view );
		AudioEmitterWrapper& Stop		( string_view );
		AudioEmitterWrapper& SetVolume	( string_view, float );
		AudioEmitterWrapper& SetPitch	( string_view, float );
		AudioEmitterWrapper& SetPaused	( string_view, bool );
		AudioEmitterWrapper& SetLoop	( string_view, bool );
		inline EmitterHandle GetID() const	{ return emitterID; }

		void Remove( string_view _name );

	private:

		EmitterHandle emitterID{};

	};

}