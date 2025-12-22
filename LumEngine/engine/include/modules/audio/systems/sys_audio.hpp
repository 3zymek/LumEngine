#pragma once
#include "include/modules/entity/systems/sys_define.hpp"
#include "include/modules/audio/audio_manager.hpp"
#include "include/core/math/glm.hpp"
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

		FMOD_VECTOR TransferCoordsToFMOD( const glm::vec3 ) noexcept;

	};

}
