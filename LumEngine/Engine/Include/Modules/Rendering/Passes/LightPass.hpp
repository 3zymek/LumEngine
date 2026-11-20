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
#include "Rhi/Interfaces/Core/RhiResourceSet.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"

namespace lum {

    class DeferredBuffer;

    struct SpotLight {

    };

    struct PointLight {

    };

    class LightPass {
    public:

        void Initialize(const DeferredBuffer& buffer);
        void DrawScene();

        void SendPointLight(const PointLight& light);

    private:

        SafePtr<DeferredBuffer> m_DeferredBuffer = nullptr;

        rhi::ResourceLayoutHandle m_ResourceLayout{};
        rhi::ResourceSetHandle m_ResourceSet{};
        rhi::BufferHandle m_LightsUbo{};

    };

} // namespace lum
