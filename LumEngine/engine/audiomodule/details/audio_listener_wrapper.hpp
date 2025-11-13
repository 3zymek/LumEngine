#pragma once
#include "audiomodule/details/audio_define.hpp"
#include "entitymodule/details/ecs_define.hpp"
#include "utils/math/vec3f.hpp"
namespace audio {
	class AudioListenerWrapper {
	public:

		AudioListenerWrapper(ecs::EntityID _id) : entityID(_id) {}

		void SetPosition(float _x, float _y, float _z) noexcept {
			position.x() = _x;
			position.y() = _y;
			position.z() = _z;
		}

		const Vec3f& GetPosition() const noexcept {
			return position;
		}

		ecs::EntityID GetEntityID() const noexcept {
			return entityID;
		}

	private:
		ecs::EntityID entityID{};
		Vec3f position;
	};
}