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

#include <vector>

#include "Core/CoreTypes.hpp"
#include "Core/Limits.hpp"
#include "Core/Math/Mat4.hpp"
#include "Core/Utils/FixedVector.hpp"
#include "Core/Utils/SafePtr.hpp"
#include "Rendering/MeshBuilder.hpp"
#include "Rendering/RenderingTypes.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"

namespace lum {

	struct CMaterialInstance;
	class DeferredBuffer;

	namespace rhi {
		class CommandBuffer;
	} // namespace rhi

	struct alignas(16) MaterialGPU {

		Vector4 m_BaseColor = Vector4( 1.0f );
		float32 m_Roughness = 0.5f;
		float32 m_Metalness = 0.0f;

		float32 _m_Pad[ 2 ]{};

	};

	struct CameraGPU {
		Matrix4 m_View{};
		Matrix4 m_Projection{};
		Matrix4 m_InvertedProjection{};
		Vector4 m_Position{};
	};

	class GeometryPass {
	public:

		GeometryPass( ) = default;

		void Initialize( RendererContext& ctx, DeferredBuffer& deferredBuffer );
		void SubmitInstance( const RenderInstance& instance );
		void DrawScene( );

	private:

		SafePtr<RendererContext> m_Ctx = nullptr;
		MeshBuilder m_MeshBuilder{};

		constexpr inline static usize s_TempSize = 64;

		FixedVector<RenderInstance, limits::k_MaxDrawCallsPerFrame> m_PendingInstances{};
		FixedVector<RenderInstance, s_TempSize> m_TemporaryInstances{};

		rhi::ResourceLayoutHandle m_ResourceLayout{};
		rhi::ResourceSetHandle m_ResourceSet{};
		rhi::BufferHandle m_MaterialUbo{};
		rhi::BufferHandle m_VertexBuffer{};
		rhi::PipelineHandle m_GeometryPipeline{};
		rhi::ImageHandle m_AlbedoAttachment = rhi::ImageHandle::Null( );

		void draw_instance( const RenderInstance& instance, rhi::CommandBuffer& cmdBuffer ) noexcept;
		void draw_mesh( const RenderInstance& instance, rhi::CommandBuffer& cmdBuffer ) noexcept;
		void upload_model_matrix( const RenderInstance& instance ) noexcept;
		void upload_material( const CMaterialInstance& instance ) noexcept;

	};

} // namespace lum