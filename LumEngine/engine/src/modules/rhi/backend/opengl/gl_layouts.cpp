#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	RVertexLayoutHandle GLDevice::CreateVertexLayout ( const RVertexLayoutDescriptor& desc, const RBufferHandle& vbo ) {

		LUM_HOTCHK_RETURN_CUSTOM(
			mBuffers.exist(vbo),
			LUM_SEV_WARN,
			RVertexLayoutHandle{},
			"Buffer doesn't exist"
		);
		LUM_HOTCHK_RETURN_CUSTOM(
			desc.mAttributes.size() > 0,
			LUM_SEV_WARN,
			RVertexLayoutHandle{},
			"VertexLayout has no attributes",
		);

		RVertexLayout layout;
		RBuffer& buffer = mBuffers[vbo];

		glCreateVertexArrays(1, &layout.mHandle);
		glVertexArrayVertexBuffer(
			layout.mHandle,
			desc.mBinding,
			buffer.mHandle.gl,
			desc.mOffset,
			desc.mStride
		);

		for (int i = 0; i < desc.mAttributes.size(); i++) {

			glVertexArrayAttribFormat(
				layout.mHandle,
				desc.mAttributes[i].mShaderLocation,
				detail::gDataFormatLookup[lookup_cast(desc.mAttributes[i].mFormat)],
				GL_FLOAT,
				GL_FALSE,
				desc.mAttributes[i].mRelativeOffset
			);

			glVertexArrayAttribBinding(
				layout.mHandle,
				desc.mAttributes[i].mShaderLocation,
				desc.mBinding
			);

			glEnableVertexArrayAttrib(layout.mHandle, desc.mAttributes[i].mShaderLocation);

		}

		auto layoutHandle = mLayouts.create_handle(std::move(layout));

		LUM_LOG_INFO("Created vertex layout %d", layoutHandle.id);

		return layoutHandle;

	}
	void GLDevice::DeleteVertexLayout ( RVertexLayoutHandle& layout ) {
		LUM_HOTCHK_RETURN_VOID(!mLayouts.exist(layout), LUM_SEV_WARN, "Layout doesn't exist");

		glDeleteVertexArrays(1, &mLayouts[layout].mHandle);
		mLayouts.delete_handle(layout);

	}
}