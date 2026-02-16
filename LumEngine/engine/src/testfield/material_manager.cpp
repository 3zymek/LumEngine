#include "testfield/material_manager.hpp"
#include "testfield/renderer.hpp"
namespace lum {

	MaterialHandle MaterialManager::CreateMaterial(const MaterialDescriptor& desc) {

		Material material;

		material.mAlbedoMap = validate_texture(desc.mAlbedoMap);
		material.mNormalMap = validate_texture(desc.mNormalMap);
		material.mRoughnessMap = validate_texture(desc.mRoughnessMap);
		material.mMetallicMap = validate_texture(desc.mMetallicMap);
		material.mAmbientMap = validate_texture(desc.mAmbientMap);

		material.mBaseColor = desc.mBaseColor;
		material.mRoughness = desc.mRoughness;
		material.mMetallic = desc.mMetallic;
		material.mAmbient = desc.mAmbient;

		return mMaterials.create_handle(std::move(material));
	}

	Material& MaterialManager::Get(const MaterialHandle& handle) {
		if (!mMaterials.exist(handle))
			return mMaterials[mDefaultMaterial];
		return mMaterials[handle];
	}

	rhi::TextureHandle MaterialManager::validate_texture(rhi::TextureHandle handle) {
		if (!mRenderer->mRenderDevice->IsValid(handle)) {
			return mRenderer->mDefaultTexture;
		}
		return handle;
	}

	void MaterialManager::create_materials() {
		{
			MaterialDescriptor desc;

			desc.mBaseColor = { 1.0f, 1.0f, 1.0f };
			desc.mRoughness = 1.0f;
			desc.mMetallic = 1.0f;
			desc.mAmbient = 1.0f;

			mDefaultMaterial = CreateMaterial(desc);
		}
	}

}