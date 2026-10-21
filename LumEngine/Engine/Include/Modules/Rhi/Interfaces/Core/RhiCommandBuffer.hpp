/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#pragma once

#include <span>
#include <type_traits>
#include <vector>

#include "Core/Utils/Assert.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"
#include "Rhi/RhiTypes.hpp"

namespace lum::rhi {

	class IRenderDevice;


	struct Command {};
	enum class CommandType : uint8 {
		BindPipeline = 0U,
		BindTexture,
		BindVertexBuffers,
		BindElementBuffer,
		DrawIndexed
	};

	template<typename tType>
	concept cHasCmdType = requires(tType t) {
		t.s_Type;
	};

#	define LUM_RHI_CMD_TYPE( type ) \
		static inline constexpr CommandType s_Type = type

	class CommandBuffer {
	public:

		template<typename tCommandData>
			requires std::is_convertible_v<tCommandData, Command>
		void Push( const tCommandData& data ) {
			LUM_SASSERT( cHasCmdType<tCommandData> );
			push_command( tCommandData::s_Type, data );
		}

		std::span<uint8> GetByteStream( ) {
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