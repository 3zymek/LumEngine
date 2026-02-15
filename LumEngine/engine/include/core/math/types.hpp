//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Core math API abstraction
// 
//=============================================================================//
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

		inline Mat4 mat4_cast(const Quat& q) {

			return glm::mat4_cast(q);

		}
	}

}
