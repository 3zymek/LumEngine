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
		BindResourceSet,
		DrawIndexed,
		SetViewport,
		SetScissors,
		UpdateResourceSet,
		TransferImage
	};

	template<typename tType>
	concept cHasCmdType = requires(tType t) {
		t.s_Type;
	};

#	define LUM_RHI_CMD_TYPE( type ) \
		static inline constexpr CommandType s_Type = type
#	define LUM_RHI_CMD_PRERENDER( ) \
		static inline constexpr bool s_PreRender = true

	class CommandBuffer {
	public:

		template<typename tCommand>
			requires ( std::is_convertible_v<tCommand, Command> && cHasCmdType<tCommand> )
		void Push( const tCommand& data ) {
			append_command( tCommand::s_Type, data );
		}

		std::span<uint8> GetRenderByteStream( ) {
			return m_RenderBuffer;
		}

		std::span<uint8> GetPreRenderByteStream( ) {
			return m_PreRenderBuffer;
		}

		void Clear( ) noexcept {
			m_RenderBuffer.clear( );
			m_PreRenderBuffer.clear( );
		}

	protected:

		template<typename tCommand>
		void append_command( CommandType type, const tCommand& data ) {
			if constexpr (requires { tCommand::s_PreRender; }) {
				append_bytes( &type, sizeof( CommandType ), m_PreRenderBuffer );
				append_bytes( &data, sizeof( tCommand ), m_PreRenderBuffer );
			}
			else {
				append_bytes( &type, sizeof( CommandType ), m_RenderBuffer );
				append_bytes( &data, sizeof( tCommand ), m_RenderBuffer );
			}
		}

		void append_bytes( const void* data, usize size, std::vector<uint8>& buffer ) {
			usize oldSize = buffer.size( );
			buffer.resize( oldSize + size );
			std::memcpy( buffer.data( ) + oldSize, data, size );
		}

		std::vector<uint8> m_RenderBuffer{};
		std::vector<uint8> m_PreRenderBuffer{};

	};

} // namespace lum::rhi