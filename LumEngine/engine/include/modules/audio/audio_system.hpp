#pragma once
#include "audio/audio_manager.hpp"
#include "core/core_pch.hpp"
namespace lum {
	namespace systems {

		class AudioSystem {

			audio::AudioManager& manager;
			using Type = cmd::Type;


		public:

			AudioSystem(audio::AudioManager& m) : manager(m) {}

			inline void Update() noexcept {

				manager.m_audio_system->update();

				UpdateEmitters();

				UpdateListener();

			}

		private:

			void UpdateEmitters() noexcept;
			void UpdateEmitterActions() noexcept;
			void UpdateListener() noexcept;

			FMOD_VECTOR TransferEmitterCoordsToFMOD ( const glm::vec3 v ) noexcept;
			FMOD_VECTOR TransferListenerCoordsToFMOD( const glm::vec3 ) noexcept;

		};
	}
}