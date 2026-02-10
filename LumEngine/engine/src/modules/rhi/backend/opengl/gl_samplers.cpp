#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	SamplerHandle GLDevice::CreateSampler ( const SamplerDescriptor& desc ) {
		LUM_HOTCHK_RETURN_CUSTOM(
			mSamplers.dense_size() <= skMaxSamplers,
			LUM_SEV_WARN,
			SamplerHandle{},
			"Max samplers reached"
		);

		Sampler sampler;

		glCreateSamplers	( 1, &sampler.handle );
		glSamplerParameteri ( sampler.handle, GL_TEXTURE_MAG_FILTER, (desc.magFilter == SamplerMagFilter::Nearest) ? GL_NEAREST : GL_LINEAR);
		glSamplerParameteri ( sampler.handle, GL_TEXTURE_MIN_FILTER, skTextureMinFilterLookup[lookup_cast(desc.minFilter)]);

		glSamplerParameteri ( sampler.handle, GL_TEXTURE_WRAP_S, skSamplerWrapLookup[lookup_cast(desc.wrapS)] );
		glSamplerParameteri ( sampler.handle, GL_TEXTURE_WRAP_T, skSamplerWrapLookup[lookup_cast(desc.wrapT)] );


		GLfloat max_anisotropy = 1.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);

		GLfloat final_anisotropy = std::clamp((float32)desc.anisotropy, 1.0f, (float32)max_anisotropy);
		glSamplerParameterf ( sampler.handle, GL_TEXTURE_MAX_ANISOTROPY, final_anisotropy );

		SamplerHandle samplerHandle = mSamplers.create_handle(std::move(sampler));

		LUM_LOG_INFO("Created sampler %d", samplerHandle.id);

		return samplerHandle;
	}

	void GLDevice::BindSampler ( const SamplerHandle& sampler, uint16 binding ) {
		
		LUM_HOTCHK_RETURN_VOID(mSamplers.exist(sampler) && binding < MAX_SAMPLER_UNITS, LUM_SEV_WARN, "Sampler doesn't exist");

		if (mCurrentSamplers[binding] == sampler) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		mCurrentSamplers[binding] = sampler;

		glBindSampler ( binding, mSamplers[sampler].handle );

		LUM_PROFILER_CACHE_MISS();

	}

	void GLDevice::DeleteSampler ( SamplerHandle sampler ) {
		LUM_HOTCHK_RETURN_VOID( mSamplers.exist(sampler), LUM_SEV_WARN, "Sampler doesn't exist" );

		glDeleteSamplers ( 1, &mSamplers[sampler].handle );

		mSamplers.delete_handle(sampler);

		LUM_LOG_INFO( "Deleted sampler %d", sampler.id );

	}

}