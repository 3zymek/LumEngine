#pragma once
#include "entitymodule/systems/sys_define.hpp"
#include "audiomodule/details/audio_manager.hpp"
#include "utils/math/vec3f.hpp"
namespace audio {

	class AudioSystem : public BaseSystem<AudioSystem> {
	public:

		AudioManager& manager;

		AudioSystem(AudioManager& m = AudioManager::Global()) : manager(m) {}

		void UpdateImplementation() {
			manager.GetSystem()->set3DListenerAttributes(
				1, &TransferCoordsToFMOD(manager.GetListener()->GetPosition()), nullptr, nullptr, nullptr);

			for (auto& emitter : manager.GetAllEmitters()) {
				emitter.position.x() += 10000;
				for (auto& clip : emitter.active_clips) {
					std::cout << "zmieniam pozycje o: " << emitter.position.x();
					clip.channel->set3DAttributes(&TransferCoordsToFMOD(emitter.position), nullptr);
				}
			}
		}


	private:

		FMOD_VECTOR& TransferCoordsToFMOD(const Vec3f& v) noexcept {
			FMOD_VECTOR fmodv;
			fmodv.x = v.x();
			fmodv.y = v.y();
			fmodv.z = v.z();
			return fmodv;
		}

	};

}
