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

#include "Core/CoreTypes.hpp"
#include "Core/Utils/SafePtr.hpp"
#include "Rendering/RenderingTypes.hpp"
#include "Core/Utils/SafePtr.hpp"
#include "Rhi/Interfaces/Core/RhiResourceSet.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"

namespace lum {

    class DeferredBuffer;
    namespace rhi { class CommandBuffer; }

    struct LUM_API DirectionalLight {

        Vector4 m_Direction = Vector4( 0.0f, 0.0f, -1.0f, 0.0f );
        Vector4 m_Color = Vector4( 1.0f );
        float32 m_Intensity = 1.0f;

    };

    /*
    struct SpotLight {

    };

    struct PointLight {
        Vector3 m_Position{};
        float32 m_Intensity{};
        Vector3 m_Color{};
        float32 m_Radius{};
    };
    */

    class LightPass {
    public:

        void Initialize(RendererContext& ctx, DeferredBuffer& buffer);
        void RefreshResources();
        void SubmitCameraData( const CameraData& data );
        void DrawScene();

        void SetDirectionalLight( const DirectionalLight& light ) noexcept;

    private:

        SafePtr<RendererContext> m_Ctx = nullptr;
        SafePtr<DeferredBuffer> m_DeferredBuffer = nullptr;

        DirectionalLight m_DirLight{};
        struct CameraGPU {
            Matrix4 m_View{};
            Matrix4 m_Projection{};
            Matrix4 m_InvertedProjection{};
            Vector4 m_Position{};
        } m_Camera{};
        rhi::BufferHandle m_DirLightUbo{};
        rhi::BufferHandle m_CameraUbo{};
        rhi::SamplerHandle m_FrameSampler{};

        rhi::PipelineHandle m_LightsPipeline{};
        rhi::ResourceLayoutHandle m_ResourceLayout{};
        rhi::ResourceSetHandle m_ResourceSet{};
        void update_resource_set( rhi::CommandBuffer& cmdBuffer ) noexcept;

    };

} // namespace lum
