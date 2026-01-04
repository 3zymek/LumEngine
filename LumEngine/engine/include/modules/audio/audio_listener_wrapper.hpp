#pragma once
#include "audio/audio_common.hpp"
#include "entity/ecs_common.hpp"
#include "entity/components/c_transform.hpp"
namespace lum {
	namespace audio {
		class AudioListenerWrapper {
		public:

			AudioListenerWrapper(ecs::EntityID _id) : entityID(_id) {}

			inline ecs::EntityID GetEntityID() const noexcept { return entityID; }

		private:

			void SetRawTransform(ecs::components::TransformComponent* c) { transform = c; };

			friend class AudioManager;
			friend class AudioSystem;

			ecs::EntityID		entityID{};
			ecs::components::TransformComponent* transform = nullptr;

		};
	}
}