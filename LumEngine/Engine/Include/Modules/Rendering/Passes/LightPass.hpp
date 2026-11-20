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
#include "Rhi/Interfaces/Core/RhiResourceSet.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"

namespace lum {

    class DeferredBuffer;

    struct DirectionalLight {

        Vector3 m_Direction{};
        Vector3 m_Color{};
        float32 m_Intensity{};

    };

    struct SpotLight {

    };

    struct PointLight {
        Vector3 m_Position{};
        float32 m_Intensity{};
        Vector3 m_Color{};
        float32 m_Radius{};
    };

    class LightPass {
    public:

        void Initialize(RendererContext& ctx, DeferredBuffer& buffer);
        void DrawScene();

        void SendPointLight(const PointLight& light);

    private:

        SafePtr<RendererContext> m_Ctx = nullptr;
        SafePtr<DeferredBuffer> m_DeferredBuffer = nullptr;

        rhi::PipelineHandle m_LightsPipeline{};
        rhi::ResourceLayoutHandle m_ResourceLayout{};
        rhi::ResourceSetHandle m_ResourceSet{};
        rhi::BufferHandle m_LightsUbo{};

    };

} // namespace lum
