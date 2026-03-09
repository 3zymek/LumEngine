//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: OpenGL samplers operations
//			Creation, binding
//
//=============================================================================//
#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	RSamplerHandle GLDevice::CreateSampler( const RSamplerDescriptor& desc ) {

		LUM_ASSERT(mSamplers.DenseSize() <= skMaxSamplers, "Max samplers reached");

		FSampler sampler;

		glCreateSamplers(1, &sampler.mHandle);
		glSamplerParameteri(sampler.mHandle, GL_TEXTURE_MAG_FILTER, (desc.mMagFilter == SamplerMagFilter::Nearest) ? GL_NEAREST : GL_LINEAR);
		glSamplerParameteri(sampler.mHandle, GL_TEXTURE_MIN_FILTER, skTextureMinFilterLookup[LookupCast(desc.mMinFilter)]);

		glSamplerParameteri(sampler.mHandle, GL_TEXTURE_WRAP_S, skSamplerWrapLookup[LookupCast(desc.mWrapS)]);
		glSamplerParameteri(sampler.mHandle, GL_TEXTURE_WRAP_T, skSamplerWrapLookup[LookupCast(desc.mWrapT)]);


		GLfloat maxAnisotropy = 1.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);

		GLfloat finalAnisotropy = std::clamp((float32)desc.mAnisotropy, 1.0f, (float32)maxAnisotropy);
		glSamplerParameterf ( sampler.mHandle, GL_TEXTURE_MAX_ANISOTROPY, finalAnisotropy );

		RSamplerHandle samplerHandle = mSamplers.Append(std::move(sampler));

		LUM_LOG_INFO("Created sampler %d", samplerHandle.mID);

		return samplerHandle;
	}

	void GLDevice::BindSampler( RSamplerHandle sampler, uint16 binding ) {
		
		LUM_ASSERT(binding < skMaxSamplerUnits, "Sampler binding out of range");
		LUM_HOTCHK_RETURN_VOID(IsValid(sampler), LUM_SEV_WARN, "Invalid sampler");

		if (mCurrentSamplers[binding] == sampler) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		mCurrentSamplers[binding] = sampler;

		glBindSampler(binding, mSamplers[sampler].mHandle);

		LUM_PROFILER_CACHE_MISS();

	}

	void GLDevice::DeleteSampler( RSamplerHandle sampler ) {

		LUM_HOTCHK_RETURN_VOID(IsValid(sampler), LUM_SEV_WARN, "Invalid sampler");

		glDeleteSamplers ( 1, &mSamplers[sampler].mHandle );
		mSamplers.Remove(sampler);

		LUM_LOG_INFO("Deleted sampler %d", sampler.mID);

	}

}