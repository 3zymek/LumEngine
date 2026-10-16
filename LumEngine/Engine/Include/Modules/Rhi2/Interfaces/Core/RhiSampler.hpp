#pragma once

#include "Rhi2/RhiCommon.hpp"

namespace lum::rhi {

    using SamplerID2 = uint32;
    struct SamplerHandle2 : cstd::BaseHandle<SamplerID2> {};

    enum class SamplerFilter {
        Linear,
        Nearest
    };

    // Defines what will happen to texture when out of bounds
    enum class SamplerWrap2 {
        Repeat,				// Texture will repeat
        RepeatMirrored,		// Texture will repeat and every second repeat is mirrored
        ClampEdge,			// Last texture pixel will be streched
        ClampBorder			// Sets color border outside bounds
    };

    struct SamplerCreateInfo2 {

        // Filter that applies to texture, when texture is magnified ( scaled up )
        SamplerFilter m_MagFilter{};
        // Filter that applies to texture, when texture is minified ( scaled down )
        SamplerFilter m_MinFilter{};

        SamplerFilter m_MipmapFilter{};

        // How texture is acting out of bounds on s( x ) axis ( when s > 1 )
        SamplerWrap2 m_WrapU{};
        // How texture is acting out of bounds on t( y ) axis ( when t > 1 )
        SamplerWrap2 m_WrapV{};
        // How texture is acting out of bounds on r( z ) axis ( when r > 1, ONLY FOR 3D TEXTURES )
        SamplerWrap2 m_WrapW{};

        float32 m_MinLod = 0.0f;
        float32 m_MaxLod = 11.0f; // 11.0f For full mipmaps
        float32 m_MipLodBias = 0.0f;

        // Level of anisotropic filtering ( sharpens textures at steep viewing angles, prevents blur and stretch ) 0 for disabled
        float32 m_Anisotropy = 0.0f;
        float32 m_MaxAnisotropy = 1.0f;

    };

    struct Sampler2 {

    };

}