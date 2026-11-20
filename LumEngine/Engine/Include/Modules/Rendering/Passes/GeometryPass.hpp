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
#include <array>
#include <unordered_map>

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

		Vector4 m_BaseColor = Vector4(1.0f);
		float32 m_Roughness = 0.5f;
		float32 m_Metalness = 0.0f;

		float32 _m_Pad[2]{};

	};

	struct CameraGPU {
		Matrix4 m_View{};
		Matrix4 m_Projection{};
		Matrix4 m_InvertedProjection{};
		Vector4 m_Position{};
	};

	struct alignas(16) DrawPushConstants {
		Matrix4 m_Model{};
		uint32 m_MaterialIndex = 0;
		uint32 _m_Pad[3]{};
	};

	class GeometryPass {
	public:

		GeometryPass() = default;

		void Initialize(RendererContext& ctx, DeferredBuffer& deferredBuffer);
		void SubmitInstance(const RenderInstance& instance);
		void SubmitCameraData(Vector3 pos, Vector3 front, Vector3 up, Vector3 right);
		void DrawScene();

	private:

		CameraGPU m_PendingCamera{};
		std::array<MaterialGPU, limits::k_MaxDrawCallsPerFrame + 1> m_MaterialData{};
		std::array<DrawPushConstants, limits::k_MaxDrawCallsPerFrame> m_DrawPushConstants{};
		std::unordered_map<const CMaterialInstance*, uint32> m_MaterialIndices{};
		SafePtr<DeferredBuffer> m_DeferredBuffer = nullptr;
		SafePtr<RendererContext> m_Ctx = nullptr;
		MeshBuilder m_MeshBuilder{};

		constexpr inline static usize s_TempSize = 64;

		FixedVector<RenderInstance, limits::k_MaxDrawCallsPerFrame> m_PendingInstances{};
		FixedVector<RenderInstance, s_TempSize> m_TemporaryInstances{};

		rhi::ResourceLayoutHandle m_ResourceLayout{};
		rhi::ResourceSetHandle m_ResourceSet{};
		rhi::BufferHandle m_CameraUbo{};
		rhi::BufferHandle m_MaterialUbo{};
		rhi::PipelineHandle m_GeometryPipeline{};

		void draw_instance(const RenderInstance& instance, uint32 drawIndex, rhi::CommandBuffer& cmdBuffer) noexcept;
		void draw_mesh(const RenderInstance& instance, rhi::CommandBuffer& cmdBuffer) noexcept;

	};

} // namespace lum
