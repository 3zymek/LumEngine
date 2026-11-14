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
#include "Core/CoreDefines.hpp"
#include "Core/CoreTypes.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"

namespace lum::rhi {

    /* @brief Describes a buffer resource update within a resource set.
    * Specifies the buffer and the range of data associated with a descriptor binding.
    */
    struct LUM_API UpdateBufferInfo {

        /* @brief Handle of the buffer to bind. */
        BufferHandle m_BufferHandle{};

        /* @brief Byte offset into the buffer. */
        uint64 m_Offset = 0;

        /* @brief Size of the buffer range in bytes. */
        uint64 m_Range = 0;

        /* @brief Array element index within the descriptor binding. */
        uint32 m_ArrayIndex = 0;

        /* @brief Descriptor binding index. */
        uint32 m_Binding = 0;

    };

    /* @brief Describes an image resource update within a resource set.
    * Specifies the image and sampler associated with a descriptor binding.
    */
    struct LUM_API UpdateImageInfo {

        /* @brief Handle of the sampler to associate with the image. */
        SamplerHandle m_SamplerHandle{};

        /* @brief Handle of the image to bind. */
        ImageHandle m_ImageHandle{};

        /* @brief Array element index within the descriptor binding. */
        uint32 m_ArrayIndex = 0;

        /* @brief Descriptor binding index. */
        uint32 m_Binding = 0;

    };


    /* @brief Specifies how an attachment's existing contents are handled
    * at the beginning of a rendering operation.
    */
    enum class LUM_API LoadOp : uint8 {
        /* @brief Preserve the existing attachment contents. */
        Load = 0U,

        /* @brief Clear the attachment using the specified clear value. */
        Clear,

        /* @brief Discard the existing attachment contents. */
        DontCare,
    };

    /* @brief Specifies whether attachment contents are preserved
    * after a rendering operation finishes.
    */
    enum class LUM_API StoreOp : uint8 {
        /* @brief Preserve the rendered attachment contents. */
        Store = 0U,

        /* @brief Discard the attachment contents after rendering. */
        DontCare
    };

    /* @brief Describes an image attachment used during a rendering operation.
    * Contains the image resource, load/store operations and optional clear values.
    */
    struct LUM_API RenderingAttachment {

        /* @brief Handle of the image used as the rendering attachment. */
        ImageHandle m_Image{};

        /* @brief Operation performed on the attachment before rendering begins. */
        LoadOp m_LoadOp = LoadOp::DontCare;

        /* @brief Operation performed on the attachment after rendering finishes. */
        StoreOp m_StoreOp = StoreOp::DontCare;

        /* @brief Union containing the value used to clear the attachment.
        * The active member depends on the attachment type.
        */
        union ClearValue {

            /* @brief RGBA color used when clearing a color attachment. */
            struct {
                Vector4 m_RGBA{};
            } m_Color;

            /* @brief Depth and stencil values used when clearing a depth/stencil attachment. */
            struct {
                float32 m_Depth{};
                uint32 m_Stencil{};
            } m_DepthStencil;

            /* @brief Creates a clear value for a color attachment. */
            static constexpr ClearValue Color( Vector4 color ) noexcept {
                return ClearValue{
                    .m_Color = { color }
                };
            }

            /* @brief Creates a clear value for a depth/stencil attachment. */
            static constexpr ClearValue DepthStencil( float32 depth, uint32 stencil ) {
                return ClearValue{
                    .m_DepthStencil = { depth, stencil }
                };
            }

        } m_ClearValue{};

    };

} // namespace lum::rhi