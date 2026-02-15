#pragma once
#include "rhi/rhi_common.hpp"

namespace lum {

	struct Material {

		rhi::TextureHandle mAlbedoMap;
		rhi::TextureHandle mNormalMap;
		rhi::TextureHandle mRoughnessMap;
		rhi::TextureHandle mMetallicMap;
		rhi::TextureHandle mAmbientMap;

		glm::vec3 mAlbedo = glm::vec3(1.0f);
		float32 mRoughness = 0.5f;
		float32 mMetallic = 0.0f;
		float32 mAmbient = 1.0f;

	};

}