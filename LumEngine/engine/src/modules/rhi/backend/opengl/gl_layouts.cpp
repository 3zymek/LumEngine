//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: OpenGL vertex layout (VAO) creation and deletion.
//
//=============================================================================//

#include "modules/rhi/backend/gl_device.hpp"

namespace lum::rhi::gl {

	VertexLayoutHandle GLDevice::CreateVertexLayout( const VertexLayoutCreateInfo& desc, BufferHandle vbo ) {

		LUM_ASSERT( IsValid( vbo ), "Invalid buffer" );
		LUM_ASSERT( desc.mAttributes.size( ) > 0, "Vertex layout has no attributes" );

		VertexLayout layout;
		Buffer& buffer = mBuffers[ vbo ];

		glCreateVertexArrays( 1, &layout.mHandle );
		glVertexArrayVertexBuffer(
			layout.mHandle,
			desc.mBinding,
			buffer.mHandle,
			desc.mOffset,
			desc.mStride
		);

		for (int32 i = 0; i < desc.mAttributes.size( ); i++) {

			glVertexArrayAttribFormat(
				layout.mHandle,
				desc.mAttributes[ i ].mShaderLocation,
				detail::gDataFormatLookup[ LookupCast( desc.mAttributes[ i ].mFormat ) ],
				GL_FLOAT,
				GL_FALSE,
				desc.mAttributes[ i ].mRelativeOffset
			);

			glVertexArrayAttribBinding(
				layout.mHandle,
				desc.mAttributes[ i ].mShaderLocation,
				desc.mBinding
			);

			glEnableVertexArrayAttrib( layout.mHandle, desc.mAttributes[ i ].mShaderLocation );

		}

		return mLayouts.Append( std::move( layout ) );

	}
	void GLDevice::Delete( VertexLayoutHandle& layout ) {

		LUM_RETURN_IF( !IsValid( layout ), LUM_SEV_WARN, "Invalid layout" );
		glDeleteVertexArrays( 1, &mLayouts[ layout ].mHandle );
		mLayouts.Remove( layout );

	}
}