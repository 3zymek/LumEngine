#pragma once

#include "core/core_common.hpp"
#include "core/utils/handle_pool.hpp"
#include "core/limits.hpp"
#include "material.hpp"

namespace lum {

	struct Renderer;

	struct MaterialHandle : cstd::BaseHandle {};

	class MaterialManager {
	public:

		MaterialManager() {}
		
		LUM_FORCEINLINE
		void Init ( Renderer* renderer ) {
			mRenderer = renderer;
		}

		MaterialHandle	CreateMaterial	( const MaterialDescriptor& desc );
		Material&		Get				( const MaterialHandle& handle );
		
	private:

		cstd::handle_pool<Material, MaterialHandle> mMaterials { limits::gMaxMaterials };

		Renderer*		mRenderer = nullptr;
		MaterialHandle	mDefaultMaterial;

		rhi::TextureHandle	validate_texture ( rhi::TextureHandle handle );
		void				create_materials ( );

	};


}