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
#include "Core/CorePch.hpp"

// Header impl file
#include "Rendering/VertexLayoutConfig.hpp"

#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/RhiTypes.hpp"

namespace lum {

	const VertexLayoutConfiguration& VertexLayoutConfiguration::GetStatic( ) noexcept {
		static VertexLayoutConfiguration val{};
		return val;
	}

	LUM_NODISCARD rhi::VertexInputPass VertexLayoutConfiguration::GetVertexInputPass( ) const {

		rhi::VertexInputPass info{};

		rhi::VertexBinding binding{};
		binding.m_Binding = 0;
		binding.m_Instanced = false;
		binding.m_Stride = sizeof( rhi::Vertex );
		info.m_Bindings.push_back( binding );

		info.m_Attributes.reserve( 3 );

		info.m_Attributes.push_back( { 0, 0, 0, rhi::ImageFormat::RGB32_FLOAT } );
		info.m_Attributes.push_back( { 1, 0, 12, rhi::ImageFormat::RGB32_FLOAT } );
		info.m_Attributes.push_back( { 2, 0, 24, rhi::ImageFormat::RG32_FLOAT } );

		return info;

	}

} // namespace lum