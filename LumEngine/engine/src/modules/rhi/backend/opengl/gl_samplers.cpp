#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {
	///////////////////////////////////////////////////
/// Samplers
///////////////////////////////////////////////////

	SamplerHandle GLDevice::CreateSampler(const SamplerDescriptor& desc) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mSamplers.dense_size() >= skMaxSamplers,
			"Max samplers reached",
			SamplerHandle{}
		);

		Sampler sampler;

		glCreateSamplers(1, &sampler.handle);
		glSamplerParameteri(sampler.handle, GL_TEXTURE_MAG_FILTER, (desc.magFilter == SamplerMagFilter::Nearest) ? GL_NEAREST : GL_LINEAR);
		glSamplerParameteri(sampler.handle, GL_TEXTURE_MIN_FILTER, skTextureMinFilterLookup[lookup_cast(desc.minFilter)]);

		glSamplerParameteri(sampler.handle, GL_TEXTURE_WRAP_S, skSamplerWrapLookup[lookup_cast(desc.wrapS)]);
		glSamplerParameteri(sampler.handle, GL_TEXTURE_WRAP_T, skSamplerWrapLookup[lookup_cast(desc.wrapT)]);

		GLfloat max_anisotropy = 1.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);
		GLfloat final_anisotropy = std::clamp((float32)desc.anisotropy, 1.0f, (float32)max_anisotropy);
		glSamplerParameterf(sampler.handle, GL_TEXTURE_MAX_ANISOTROPY, final_anisotropy);

		return mSamplers.create_handle(std::move(sampler));
	}
	// TO IMPLEMENT:
	void GLDevice::SetSamplerBinding(const SamplerHandle& sampler, uint16 binding) {
		LUM_HOTPATH_ASSERT_VOID(!mSamplers.exists(sampler), std::format("Sampler {} doesn't exists", sampler.id));

		mSamplers[sampler].binding = binding;

	}
	void GLDevice::BindSampler(const SamplerHandle& sampler, uint16 binding) {
		LUM_HOTPATH_ASSERT_VOID(!mSamplers.exists(sampler), std::format("Sampler {} doesn't exists", sampler.id));
		LUM_HOTPATH_ASSERT_VOID(
			mSamplers[sampler].binding == LUM_NULL_BINDING &&
			binding == LUM_NULL_BINDING,
			std::format("Binding has not been given to sampler {}", sampler.id)
		);

		uint16_t bind = (binding == LUM_NULL_BINDING) ? mSamplers[sampler].binding : binding;

		glBindSampler(bind, mSamplers[sampler].handle);


	}
	void GLDevice::DeleteSampler(SamplerHandle sampler) {
		LUM_HOTPATH_ASSERT_VOID(!mSamplers.exists(sampler), std::format("Sampler {} doesn't exists", sampler.id));

		glDeleteSamplers(1, &mSamplers[sampler].handle);

		mSamplers.delete_handle(sampler);
	}

}