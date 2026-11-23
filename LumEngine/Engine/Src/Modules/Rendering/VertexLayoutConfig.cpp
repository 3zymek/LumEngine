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

    LUM_NODISCARD rhi::VertexInputPass VertexLayoutConfiguration::GetDefaultConfiguration( ) const {

        rhi::VertexInputPass info{};

        // Vertex binding
        rhi::VertexBinding binding{};
        binding.m_Binding = 0;
        binding.m_Instanced = false;
        binding.m_Stride = sizeof( rhi::Vertex );

        info.m_Bindings.push_back( binding );

        // Position
        info.m_Attributes.push_back( {
            0,
            0,
            offsetof( rhi::Vertex, m_Position ),
            rhi::ImageFormat::RGB32_FLOAT
        } );

        // Normal
        info.m_Attributes.push_back( {
            1,
            0,
            offsetof( rhi::Vertex, m_Normal ),
            rhi::ImageFormat::RGB32_FLOAT
        } );

        // UV
        info.m_Attributes.push_back( {
            2,
            0,
            offsetof( rhi::Vertex, m_UV ),
            rhi::ImageFormat::RGB32_FLOAT
        } );

        // Tangent
        info.m_Attributes.push_back( {
            3,
            0,
            offsetof( rhi::Vertex, m_Tangent ),
            rhi::ImageFormat::RGB16_FLOAT
        } );

        // Tangent
        info.m_Attributes.push_back( {
            4,
            0,
            offsetof( rhi::Vertex, m_Bitangent ),
            rhi::ImageFormat::RGB16_FLOAT
        } );

        return info;
    }

} // namespace lum