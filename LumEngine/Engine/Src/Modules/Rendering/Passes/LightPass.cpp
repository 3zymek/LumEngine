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

// Header impl file
#include "Rendering/Passes/LightPass.hpp"

#include "Core/Utils/ResourceLoader.hpp"
#include "Rhi/Interfaces/Core/RhiDevice.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rendering/DeferredBuffer.hpp"
#include "Rendering/VertexLayoutConfig.hpp"

namespace lum {

    void LightPass::Initialize(RendererContext& ctx, DeferredBuffer& buffer) {

        ctx().Assert();
        m_Ctx = ctx;

        m_DeferredBuffer = buffer;

        { // Resource layout
            rhi::ResourceSetBinding cameraBinding{};
            cameraBinding.m_Binding = 0;
            cameraBinding.m_ResourceCount = 1;
            cameraBinding.m_ShaderStages = rhi::ShaderStageFlag::Fragment;
            cameraBinding.m_Type = rhi::ResourceType::UniformBuffer;
            rhi::ResourceLayoutCreateInfo info{};
            info.m_Bindings.push_back(cameraBinding);

        }
        { // Lights pipeline
            rhi::PipelineCreateInfo info{};
            info.m_Layout = m_ResourceLayout;

            rhi::ShaderInfo vertexInfo{};
            vertexInfo.m_EntryPoint = "main";
            vertexInfo.m_Path = ResourceLoader::ResolveResourcePath(
                ResourceRoot::Internal, "Shaders/LightPass.vert.spv"
            );
            vertexInfo.m_Stage = rhi::ShaderStageFlag::Vertex;
            info.m_ShaderInfos.push_back(vertexInfo);

            rhi::ShaderInfo fragmentInfo{};
            fragmentInfo.m_EntryPoint = "main";
            fragmentInfo.m_Path = ResourceLoader::ResolveResourcePath(
                ResourceRoot::Internal, "Shaders/LightPass.vert.spv"
            );
            fragmentInfo.m_Stage = rhi::ShaderStageFlag::Vertex;
            info.m_ShaderInfos.push_back(fragmentInfo);

            info.m_VertexInputPass = VertexLayoutConfiguration::GetStatic().GetDefaultConfiguration();

            m_LightsPipeline = m_Ctx().m_RenderDevice().CreatePipeline(info);

        }

    }

    void LightPass::DrawScene() {

    }

}