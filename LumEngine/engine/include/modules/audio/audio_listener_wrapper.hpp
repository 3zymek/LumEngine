#pragma once
#include "audio/audio_common.hpp"
#include "entity/ecs_common.hpp"
#include "entity/components/c_transform.hpp"
#include "audio/components/c_audio_listener.hpp"
namespace lum {
	namespace systems {
		struct AudioSystem;
	}
	namespace audio {
		class AudioListenerWrapper {
		public:

			AudioListenerWrapper(ecs::EntityID _id) : entityID(_id) {}

			inline ecs::EntityID GetEntityID() const noexcept { return entityID; }

		private:

			friend class AudioManager;
			friend class lum::systems::AudioSystem;

			ecs::EntityID							entityID{};
			CTransform*	mTransform = nullptr;
			CAudioListener* mListener = nullptr;

		};
	}
}