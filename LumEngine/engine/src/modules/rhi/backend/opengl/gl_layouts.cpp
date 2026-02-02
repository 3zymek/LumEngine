#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {
///////////////////////////////////////////////////
/// Layouts
///////////////////////////////////////////////////

	VertexLayoutHandle GLDevice::CreateVertexLayout(const VertexLayoutDescriptor& desc, const BufferHandle& vbo) {

		LUM_HOTCHK_RETURN_CUSTOM(mBuffers.exist(vbo), "Buffer doesn't exists", VertexLayoutHandle{});
		LUM_HOTCHK_RETURN_CUSTOM(desc.attributes.size() > 0, "VertexLayout has no attributes", VertexLayoutHandle{});

		VertexLayout layout;
		Buffer& buffer = mBuffers[vbo];

		glCreateVertexArrays(1, &layout.vao);
		glVertexArrayVertexBuffer(
			layout.vao,
			desc.binding,
			buffer.handle.glHandle,
			desc.offset,
			desc.stride
		);

		for (int i = 0; i < desc.attributes.size(); i++) {

			glVertexArrayAttribFormat(
				layout.vao,
				desc.attributes[i].shaderLocation,
				detail::skDataFormatLookup[lookup_cast(desc.attributes[i].format)],
				GL_FLOAT,
				GL_FALSE,
				desc.attributes[i].relativeOffset
			);

			glVertexArrayAttribBinding(
				layout.vao,
				desc.attributes[i].shaderLocation,
				desc.binding
			);

			glEnableVertexArrayAttrib(layout.vao, desc.attributes[i].shaderLocation);

		}

		LUM_LOG_INFO("Created vertex layout {}");
		return mLayouts.create_handle(std::move(layout));

	}
	void GLDevice::DeleteVertexLayout(VertexLayoutHandle& layout) {
		LUM_HOTCHK_RETURN_VOID(!mLayouts.exist(layout), "Layout doesn't exist");

		glDeleteVertexArrays(1, &mLayouts[layout].vao);
		mLayouts.delete_handle(layout);

	}
}