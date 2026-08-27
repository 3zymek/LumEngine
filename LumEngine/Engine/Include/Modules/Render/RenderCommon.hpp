//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Shared render data types — lights, instances, camera
//          and internal pass/uniform structures.
//
//=============================================================================//
#pragma once

#include "Rhi/Core/RhiDevice.hpp"

#include "Core/Utils/HandlePool.hpp"

#include "Entity/Components/Material.hpp"

namespace lum {

	namespace ev { class EventBus; }

	/// Forward declare ///
	class TextureManager;
	class MaterialManager;
	class ShaderManager;
	class MeshManager;
	class SceneManager;
	struct CStaticMesh;
	struct CTransform;
	///////////////////////

	struct StaticMeshHandle : cstd::BaseHandle<uint32> {};
	struct DynamicMeshHandle : cstd::BaseHandle<uint32> {};

}

namespace lum::render {

#	define LUM_UBO_ALIGNMENT alignas(16)

	/* @brief Aggregates all external subsystem pointers required to initialize the renderer. */
	struct RendererContext {

		SafePtr<rhi::IRenderDevice>	m_RenderDev = nullptr;
		SafePtr<TextureManager>		m_TextureMgr = nullptr;
		SafePtr<MaterialManager>	m_MaterialMgr = nullptr;
		SafePtr<MeshManager>		m_MeshMgr = nullptr;
		SafePtr<ShaderManager>		m_ShaderMgr = nullptr;
		SafePtr<ev::EventBus>		m_EventBus = nullptr;

		void Validate( ) const {
			LUM_ASSERT( m_RenderDev, "RendererContext: IRenderDevice is nullptr!" );
			LUM_ASSERT( m_TextureMgr, "RendererContext: TextureManager is nullptr!" );
			LUM_ASSERT( m_MaterialMgr, "RendererContext: MaterialManager is nullptr!" );
			LUM_ASSERT( m_MeshMgr, "RendererContext: MeshManager is nullptr!" );
			LUM_ASSERT( m_ShaderMgr, "RendererContext: ShaderManager is nullptr!" );
			LUM_ASSERT( m_EventBus, "RendererContext: EventBus is nullptr!" );
		}

	};

	/* @brief Directional light source — infinite distance, uniform direction.
	*  Aligned to 16 bytes for std140 uniform buffer compatibility.
	*/
	struct DirectionalLight {

		/* @brief Normalized direction vector the light is pointing towards. */
		Vector3 m_Direction = Vector3( 0.0f, 1.0f, 0.0f );

		/* @brief Intensity multiplier applied to the light color. */
		float32 m_Intensity = 10.f;

		/* @brief Linear RGB color of the light. */
		Vector3 m_Color = { 1.0f, 1.0f, 1.0f };

		/* Matrix used to calculate directrional light shadows. */
		Matrix4 m_LightSpaceMatrix = Matrix4( 1.0f );

	};


	/* @brief Point light source — emits light in all directions from a single position. */
	struct PointLight {

		Vector3 m_Position = Vector3( 0.0f );
		float32 m_Intensity = 10.f;
		Vector3 m_Color = Vector3( 1.0f );
		float32 m_Radius = 10.f;

	};

	struct SpotLight {

		Vector3 m_Position = Vector3( 0.0f );
		float32 m_Intensity = 10.0f;

		Vector3 m_Direction = Vector3( 0.0f, 1.0f, 0.0f );
		float32 m_Radius = 35.0f;;

		Vector3 m_Color = Vector3( 1.0f );
		float32 m_InnerCone = 10.0f;

		float32 m_OuterCone = 50.0f;

		float32 m_Pad[ 3 ]{};

	};


	/* @brief Represents a single renderable object submitted to the renderer per frame. */
	struct RenderInstance {

		SafePtr<CTransform>			m_Transform = nullptr;
		SafePtr<CStaticMesh>		m_StaticMesh = nullptr;
		SafePtr<CMaterialInstance>	m_Material = nullptr;

	};


	/* @brief Per-frame camera data provided by the caller to the renderer. */
	struct RenderCamera {

		/* @brief World-space position of the camera. */
		Vector3 m_Position = Vector3( 0.0f );

		/* @brief View matrix transforming world space to camera space. */
		Matrix4 m_View = Matrix4( 1.0f );

		/* @brief Projection matrix transforming camera space to clip space. */
		Matrix4 m_Projection = Matrix4( 1.0f );

	};

	namespace detail {

		struct ScreenQuad {

			rhi::VertexLayoutHandle m_Vao{};

			rhi::BufferHandle m_Vbo{};
			rhi::BufferHandle m_Ebo{};

			rhi::FramebufferHandle m_SceneFbo{};
			rhi::FramebufferHandle m_PostprocessFbo{};

			rhi::TextureHandle m_SceneTex{};
			rhi::TextureHandle m_PostprocessTex{};

		};

		struct PostprocessExecuteContext {



		};

		/* @brief Internal state for the environment (skybox) render pass. */
		struct Cubemap {

			/* @brief Shader program used for skybox rendering. */
			rhi::ShaderHandle m_Shader{};

			/* @brief Pipeline state for the environment pass. */
			rhi::PipelineHandle m_Pipeline{};

			/* @brief Cubemap or equirectangular environment texture. */
			rhi::TextureHandle m_Texture{};

			/* @brief Vertex buffer holding skybox geometry. */
			rhi::BufferHandle m_Vbo{};

			/* @brief Index buffer for skybox geometry. */
			rhi::BufferHandle m_Ebo{};

			/* @brief Vertex layout describing skybox vertex attributes. */
			rhi::VertexLayoutHandle m_Vao{};

			/* @brief Number of indices in the skybox index buffer. */
			uint32 m_NumIndices = 0;

		};

		struct LUM_UBO_ALIGNMENT LightSpaceMatricesGPU {

			Matrix4 m_DirectionalLightSpaceMatrix = Matrix4( 1.0f );

		};


		/* @brief GPU-side camera uniform buffer layout. Aligned to std140. */
		struct LUM_UBO_ALIGNMENT CameraGPU {
			Matrix4 m_View = Matrix4( 1.0f );
			Matrix4 m_Projection = Matrix4( 1.0f );
			Matrix4 m_InvViewProj = Matrix4( 1.0f );
			Vector4 m_Position = Vector4( 1.0f );
		};


		/* @brief GPU-side model uniform buffer layout. Aligned to std140. */
		struct LUM_UBO_ALIGNMENT ModelGPU {
			Matrix4 m_Model = Matrix4( 1.0f );
		};


		/* @brief GPU-side material uniform buffer layout. Aligned to std140. */
		struct LUM_UBO_ALIGNMENT MaterialGPU {

			Vector4 m_BaseColor = Vector4( 1.0f );
			float32 m_Roughness = 0.5f;
			float32 m_Metallic = 0.0f;

			float32 m_Pad[ 2 ]{};

		};

		struct LUM_UBO_ALIGNMENT DirectionalLightGPU {

			Vector4 m_Direction = Vector4( 0.0f, -1.0f, 0.0f, 0.0f );
			Vector4 m_Color = Vector4( 1.00, 0.97, 0.90, 0.0f );
			float32 m_Intensity = 5.0f;

			float32 m_Pad[ 3 ]{};

		};


	} // namespace lum::render::detail

} // namespace lum::render