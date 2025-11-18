#pragma once
#include "audiomodule/details/audio_define.hpp"
#include "entitymodule/details/ecs_define.hpp"
#include "components/c_transform.hpp"
#include "core/math/vec3f.hpp"
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