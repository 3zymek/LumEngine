#pragma once
#include "include/modules/audio/audio_common.hpp"
#include "include/modules/entity/ecs_common.hpp"
#include "include/modules/entity/components/c_transform.hpp"
namespace audio {
	class AudioListenerWrapper {
	public:

		AudioListenerWrapper( ecs::EntityID _id ) : entityID(_id) {}

		inline ecs::EntityID GetEntityID( ) const noexcept { return entityID; }

	private:

		void SetRawTransform( TransformComponent* c ) { transform = c; };

		friend class AudioManager;
		friend class AudioSystem;

		ecs::EntityID		entityID{};
		TransformComponent* transform = nullptr;

	};
}