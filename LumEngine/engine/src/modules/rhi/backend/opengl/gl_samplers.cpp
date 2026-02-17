//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: OpenGL samplers operations
//			Creation, binding
//
//=============================================================================//
#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	RSamplerHandle GLDevice::CreateSampler ( const RSamplerDescriptor& desc ) {
		LUM_HOTCHK_RETURN_CUSTOM(
			mSamplers.dense_size() <= skMaxSamplers,
			LUM_SEV_WARN,
			RSamplerHandle{},
			"Max samplers reached"
		);

		Sampler sampler;

		glCreateSamplers	( 1, &sampler.mHandle );
		glSamplerParameteri ( sampler.mHandle, GL_TEXTURE_MAG_FILTER, (desc.mMagFilter == RSamplerMagFilter::Nearest) ? GL_NEAREST : GL_LINEAR);
		glSamplerParameteri ( sampler.mHandle, GL_TEXTURE_MIN_FILTER, skTextureMinFilterLookup[lookup_cast(desc.mMinFilter)]);

		glSamplerParameteri ( sampler.mHandle, GL_TEXTURE_WRAP_S, skSamplerWrapLookup[lookup_cast(desc.mWrapS)] );
		glSamplerParameteri ( sampler.mHandle, GL_TEXTURE_WRAP_T, skSamplerWrapLookup[lookup_cast(desc.mWrapT)] );


		GLfloat max_anisotropy = 1.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);

		GLfloat final_anisotropy = std::clamp((float32)desc.mAnisotropy, 1.0f, (float32)max_anisotropy);
		glSamplerParameterf ( sampler.mHandle, GL_TEXTURE_MAX_ANISOTROPY, final_anisotropy );

		RSamplerHandle samplerHandle = mSamplers.create_handle(std::move(sampler));

		LUM_LOG_INFO("Created sampler %d", samplerHandle.id);

		return samplerHandle;
	}

	void GLDevice::BindSampler ( const RSamplerHandle& sampler, uint16 binding ) {
		
		LUM_HOTCHK_RETURN_VOID(mSamplers.exist(sampler) && binding < MAX_SAMPLER_UNITS, LUM_SEV_WARN, "Sampler doesn't exist");

		if (mCurrentSamplers[binding] == sampler) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		mCurrentSamplers[binding] = sampler;

		glBindSampler ( binding, mSamplers[sampler].mHandle );

		LUM_PROFILER_CACHE_MISS();

	}

	void GLDevice::DeleteSampler ( RSamplerHandle sampler ) {
		LUM_HOTCHK_RETURN_VOID( mSamplers.exist(sampler), LUM_SEV_WARN, "Sampler doesn't exist" );

		glDeleteSamplers ( 1, &mSamplers[sampler].mHandle );

		mSamplers.delete_handle(sampler);

		LUM_LOG_INFO( "Deleted sampler %d", sampler.id );

	}

}