#pragma once
#include "entitymodule/systems/sys_define.hpp"
#include "details/audio_manager.hpp"
#include "core/math/vec3f.hpp"

namespace audio {

	class AudioSystem : public BaseSystem<AudioSystem> {

		AudioManager& manager;
		using Type = cmd::Type;


	public:

		AudioSystem( AudioManager& m ) : manager( m ) {}

		inline void Update() noexcept {


			UpdateEmitters( );

			UpdateListener( );

			manager.m_audio_system->update();

		}

	private:

		void UpdateEmitters			( ) noexcept;
		void UpdateEmitterActions	( ) noexcept;
		void UpdateListener			( ) noexcept;

		FMOD_VECTOR TransferCoordsToFMOD( const Vec3f& ) noexcept;

	};

}
