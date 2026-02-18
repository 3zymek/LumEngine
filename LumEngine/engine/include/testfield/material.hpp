#pragma once
#include "rhi/rhi_common.hpp"

namespace lum {

	struct MaterialDescriptor {

		rhi::RTextureHandle mAlbedoMap{};
		rhi::RTextureHandle mNormalMap{};
		rhi::RTextureHandle mRoughnessMap{};
		rhi::RTextureHandle mMetallicMap{};
		rhi::RTextureHandle mAmbientMap{};

		glm::vec3 mBaseColor = glm::vec3(1.0f);
		float32 mRoughness = 0.5f;
		float32 mMetallic = 0.0f;
		float32 mAmbient = 1.0f;

	};

	struct FMaterialBase {

		rhi::RTextureHandle mAlbedoMap;
		rhi::RTextureHandle mNormalMap;
		rhi::RTextureHandle mRoughnessMap;
		rhi::RTextureHandle mMetallicMap;
		rhi::RTextureHandle mAmbientMap;

		glm::vec3 mBaseColor = glm::vec3(1.0f);
		float32 mRoughness = 0.5f;
		float32 mMetallic = 0.0f;
		float32 mAmbient = 1.0f;

	};
}