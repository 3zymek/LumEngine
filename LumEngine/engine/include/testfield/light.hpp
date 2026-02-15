#pragma once

#include "rhi/rhi_common.hpp"

namespace lum {

	struct PointLight {

		glm::vec3 mPosition = { 0.0f, 0.0f, 0.0f };
		float32 _padding1 = 0.f;
		glm::vec3 mColor = { 1.f, 1.f, 1.f };
		float32 mIntensity = 1.f;

	};

}