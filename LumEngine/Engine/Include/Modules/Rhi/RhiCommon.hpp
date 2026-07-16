//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Render Hardware Interface helpers and types
// 
//=============================================================================//
#pragma once
#include "Core/CoreCommon.hpp"
#include "Core/Utils/HandlePool.hpp"

#include "Rhi/RhiPch.hpp"

namespace lum {

	/* @brief Interleaved vertex structure passed to the GPU per draw call.
	* Aligned to 16 bytes for optimal GPU memory access.
	*/
	struct alignas(16) Vertex {

		/* @brief Vertex position in 3D space. */
		Vector3 mPosition;

		/* @brief Vertex normal vector. */
		Vector3 mNormal;

		/* @brief Texture coordinates. */
		Vector2 mUv;

		/* @brief Tangent vector used for TBN matrix construction. */
		Vector3 mTangent;

		/* @brief Bitangent vector used for TBN matrix construction. */
		Vector3 mBitangent;

	};


	/* @brief Render Hardware Interface namespace.
	*  Provides a thin abstraction over the underlying graphics API (OpenGL).
	*  Contains handles, descriptors, enums and utilities for GPU resource management.
	*/
	namespace rhi {

		/* @brief Returns the number of mipmap levels for a given texture resolution.
		* @param width  Width of the texture in pixels.
		* @param height Height of the texture in pixels.
		* @return Number of mip levels, always >= 1.
		*/
		inline uint32 MipmapLvls( uint32 width, uint32 height ) {
			return 1 + std::floor( std::log2( std::max( width, height ) ) );
		}

		/* @brief Casts an enum to its underlying integer type.
		* Used for array lookups and bitmask operations.
		* @param e Enum value to cast.
		* @return Underlying integer representation of the enum.
		*/
		template<typename E>
			requires std::is_enum_v<E>
		constexpr inline std::underlying_type_t<E> LookupCast( E e ) {
			return ToUnderlyingEnum( e );
		}

		/* @brief Returns a sentinel null value for arithmetic ID types.
		* Typically resolves to the maximum representable value of T.
		* @tparam T Arithmetic type to get the null sentinel for.
		* @return Max value of T, used as an invalid/unset ID.
		*/
		template<typename T>
			requires std::is_arithmetic_v<T>
		inline constexpr T NullID( ) {
			return MaxValue<T>( );
		}

		using BufferID = uint32; /* @brief Numeric identifier for a GPU buffer object. */
		using LayoutID = uint32; /* @brief Numeric identifier for a vertex layout (VAO). */
		using TextureID = uint32; /* @brief Numeric identifier for a GPU texture object. */
		using SamplerID = uint32; /* @brief Numeric identifier for a texture sampler. */
		using FramebufferID = uint32; /* @brief Numeric identifier for a framebuffer object. */
		using PipelineID = uint32; /* @brief Numeric identifier for a pipeline state object. */
		using ShaderID = uint8;  /* @brief Numeric identifier for a shader program (compact). */

		/* @brief Typed handle wrapping a pipeline state object ID. */
		struct PipelineHandle : public cstd::BaseHandle<PipelineID> { };

		/* @brief Typed handle wrapping a framebuffer object ID. */
		struct FramebufferHandle : public cstd::BaseHandle<FramebufferID> { using BaseHandle::BaseHandle; };

		/* @brief Typed handle wrapping a texture sampler ID. */
		struct SamplerHandle : public cstd::BaseHandle<SamplerID> { };

		/* @brief Typed handle wrapping a shader program ID. */
		struct ShaderHandle : public cstd::BaseHandle<ShaderID> { };

		/* @brief Typed handle wrapping a GPU texture ID. */
		struct TextureHandle : public cstd::BaseHandle<TextureID> { };

		/* @brief Typed handle wrapping a GPU buffer object ID. */
		struct BufferHandle : public cstd::BaseHandle<BufferID> { };

		/* @brief Typed handle wrapping a vertex layout (VAO) ID. */
		struct VertexLayoutHandle : public cstd::BaseHandle<LayoutID> { };

		inline constexpr FramebufferHandle kDefaultFramebuffer { 0, MaxValue<FramebufferID>( ) };

		/* @brief Bitmask storage type used for RHI flag enums. */
		using REnumFlag = uint16;

		enum class RenderBackend : byte {
			OpenGL,
			/*Vulkan*/
		};

		/* @brief Bitmask flags controlling framebuffer attachments. */
		enum class BufferBit : bitfield {
			Color = 1 << 0, /* @brief Color buffer. */
			Depth = 1 << 1, /* @brief Depth buffer. */
			Stencil = 1 << 2  /* @brief Stencil buffer. */
		};

		/* @brief Describes a single texture layer attachment operation for a framebuffer.
		* Used to bind a specific layer of a Texture2DArray or Cubemap to an FBO attachment point.
		*
		* @param mLayer        Layer index to attach (array layer or cubemap face).
		* @param mMip          Mip level to attach. 0 = base level.
		* @param mSlot         Color attachment index. Only used when mAttachment == Color.
		* @param mAttachment   Framebuffer attachment point (Color, Depth, Stencil, or Depth|Stencil).
		*/
		struct TextureLayerAttachment {

			uint32 mLayer = 0;
			uint32 mMip = 0;
			uint32 mSlot = 0;
			Flags<BufferBit> mAttachment = BufferBit::Color;

		};

		/* @brief Vertex attribute data format passed to the GPU.
		* Describes the component count and type of a single vertex attribute.
		*/
		enum class DataFormat : byte {
			Float1, /* @brief Single float scalar. */
			Vec2,   /* @brief 2-component float vector (x, y). */
			Vec3,   /* @brief 3-component float vector (x, y, z). */
			Vec4,   /* @brief 4-component float vector (x, y, z, w). */
			Mat3,   /* @brief 3x3 float matrix (9 floats). */
			Mat4    /* @brief 4x4 float matrix (16 floats). */
		};

		/* @brief Type of a GPU buffer object. */
		enum class BufferType : byte {
			None,
			Vertex,        /* @brief Vertex buffer object (VBO). */
			Element,       /* @brief Index buffer object (EBO). */
			Uniform,       /* @brief Uniform buffer object (UBO). */
			ShaderStorage, /* @brief Shader storage buffer object (SSBO). */
		};

		/* @brief Intended usage pattern of a GPU buffer.
		* Hints the driver on how to optimize memory allocation.
		*/
		enum class BufferUsage : byte {
			Static,  /* @brief Data is set once and never changed (fast reads). */
			Dynamic  /* @brief Data is updated frequently during runtime. */
		};

		/* @brief Polygon rasterization mode. */
		enum class TopologyMode : byte {
			Point, /* @brief Render vertices as individual points. */
			Line,  /* @brief Render polygon edges as lines (wireframe). */
			Fill   /* @brief Fill polygon interiors (solid rendering). */
		};

		/* @brief Bitmask flags controlling CPU-side buffer mapping behavior. */
		enum class MapFlag : bitfield {
			None				= 0,
			Persistent			= 1 << 0, /* @brief Mapping persists across multiple frames. */
			Write				= 1 << 1, /* @brief CPU may write to the mapped range. */
			Read				= 1 << 2, /* @brief CPU may read from the mapped range. */
			Coherent			= 1 << 3, /* @brief Writes are immediately visible to the GPU. */
			Invalidate_Range	= 1 << 4, /* @brief GPU allocates a new range; old range remains valid. */
			Invalidate_Buffer	= 1 << 5, /* @brief GPU allocates a new buffer; old buffer is discarded. */
			Unsynchronized		= 1 << 6, /* @brief Map without GPU synchronization guarantees. */
		};

		/* @brief Specifies which polygon face(s) an operation applies to. */
		enum class Face : byte {
			Front,     /* @brief Apply to front-facing polygons only. */
			Back,      /* @brief Apply to back-facing polygons only. */
			FrontBack, /* @brief Apply to both front and back-facing polygons. */
		};

		/* @brief Determines which polygon winding order is considered front-facing. */
		enum class WindingOrder : byte {
			CounterClockwise, /* @brief Triangles wound counter-clockwise are front-facing. */
			Clockwise         /* @brief Triangles wound clockwise are front-facing. */
		};

		/* @brief Per-channel write mask controlling which color channels are written to the framebuffer. */
		struct ColorMask {
			bool mR : 1 = true; /* @brief Enable writes to the red channel. */
			bool mG : 1 = true; /* @brief Enable writes to the green channel. */
			bool mB : 1 = true; /* @brief Enable writes to the blue channel. */
			bool mA : 1 = true; /* @brief Enable writes to the alpha channel. */
		};

		/* @brief Internal implementation details for the RHI.
		*  Not intended for direct use outside of the RHI subsystem.
		*/
		namespace detail {

#		if LUM_ENABLE_DEBUG_RENDER == 1
			inline void APIENTRY GLDebugCallback(
				GLenum src,
				GLenum type,
				GLuint id,
				GLenum severity,
				GLsizei length,
				const char* msg,
				const void* usrParam
			) {
				if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
					LUM_LOG_DEBUG( msg );
				else if (severity == GL_DEBUG_SEVERITY_LOW)
					LUM_LOG_INFO( msg );
				else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
					LUM_LOG_WARN( msg );
				else if (severity == GL_DEBUG_SEVERITY_HIGH)
					LUM_LOG_ERROR( msg );

			}
#		endif

			/* @brief Lookup table mapping EDataFormat enum values to their component counts.
			* Indexed by the underlying value of EDataFormat.
			*/
			inline constexpr
				uint8 kDataFormatLookup[ ] = { 1, 2, 3, 4, 9, 16 };

		} // namespace lum::rhi::detail

	} // namespace lum::rhi

	LUM_ENABLE_ENUM_BITFLAG_OPERATIONS( rhi::MapFlag );
	LUM_ENABLE_ENUM_BITFLAG_OPERATIONS( rhi::BufferBit );

} // namespace lum