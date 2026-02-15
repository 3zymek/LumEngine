// ************************************
// LumEngine Copyright (C) 2026 3zymek
// All rights reserved.
// Core math API and backend abstraction.
// ************************************
#pragma once

#include "core/core_pch.hpp"

namespace lum {

	namespace math {

		using Vec2 = glm::vec2;
		using Vec3 = glm::vec3;
		using Vec4 = glm::vec4;

		using Mat3 = glm::mat3;
		using Mat4 = glm::mat4;

		using Quat = glm::quat;

		inline const float* value_ptr(const Mat4& m) {

			return glm::value_ptr(m);

		}
		inline const float* value_ptr(const Mat3& m) {

			return glm::value_ptr(m);

		}
		inline const float* value_ptr(const Vec2& v) {

			return glm::value_ptr(v);

		}
		inline const float* value_ptr(const Vec3& v) {

			return glm::value_ptr(v);

		}
		inline const float* value_ptr(const Vec4& v) {

			return glm::value_ptr(v);

		}
		inline const float* value_ptr(const Quat& q) {

			return glm::value_ptr(q);

		}

		inline Mat4 mat4_cast(const Quat& q) {

			return glm::mat4_cast(q);

		}
	}

}
