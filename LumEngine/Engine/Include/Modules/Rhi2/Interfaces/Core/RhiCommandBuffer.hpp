#pragma once

#include "Rhi2/RhiCommon.hpp"
#include "Rhi2/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi2/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi2/Interfaces/Core/RhiImage.hpp"
#include "Rhi2/Interfaces/Core/RhiSampler.hpp"

namespace lum::rhi {

	class IRenderDevice2;

	enum class CommandType : uint8 {
		BindPipeline = 0U,
		BindTexture,
		BindVertexBuffer,
		Draw
	};


	class CommandBuffer {
	public:

		void BindPipeline( PipelineHandle2 handle ) {
			struct Data { PipelineHandle2 m_Handle{}; };
			push_command( CommandType::BindPipeline, Data{ handle } );
		}

		std::span<const uint8> GetByteStream( ) const {
			return m_Buffer;
		}

	protected:

		template<typename tType>
		void push_command( CommandType type, const tType& data ) {
			append_bytes( &type, sizeof( CommandType ) );
			append_bytes( &data, sizeof( tType ) );
		}

		void append_bytes( const void* data, usize size ) {
			usize oldSize = m_Buffer.size( );
			m_Buffer.resize( oldSize + size );
			std::memcpy( m_Buffer.data( ) + oldSize, data, size );
		}

		std::vector<uint8> m_Buffer{};

	};

} // namespace lum::rhi