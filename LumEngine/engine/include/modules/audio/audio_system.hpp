#pragma once
#include "audio/audio_manager.hpp"
#include "core/math/glm.hpp"
namespace lum {
	namespace systems {

		class AudioSystem {

			audio::AudioManager& manager;
			using Type = cmd::Type;


		public:

			AudioSystem(audio::AudioManager& m) : manager(m) {}

			inline void Update() noexcept {


				UpdateEmitters();

				UpdateListener();

				manager.m_audio_system->update();

			}

		private:

			void UpdateEmitters() noexcept;
			void UpdateEmitterActions() noexcept;
			void UpdateListener() noexcept;

			FMOD_VECTOR TransferCoordsToFMOD(const glm::vec3) noexcept;

		};
	}
}