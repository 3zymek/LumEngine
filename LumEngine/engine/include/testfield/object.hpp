#pragma once
#include "rhi/rhi_common.hpp"
#include "render_resources.hpp"
#include "material.hpp"
#include "core/utils/asset_service.hpp"

namespace lum {

	struct Transform {
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
	};

	struct Object {

		detail::RenderResources mRenderResources;
		Material mMaterial;
		ModelData mModel;
		TextureData mTextureData;
		
		Transform mTransform;

	};

}