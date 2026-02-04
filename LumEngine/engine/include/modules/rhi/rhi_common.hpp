#pragma once
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
namespace lum {

	struct alignas(16) Vertex {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 uv;
	};

	namespace rhi {

		/// Returns amount of mipmap levels count for image.
		/// @param width Width of an image.
		/// @param height Height of an image.
		/// @return Mipmap levels count.
		inline uint32 mipmap_lvls( uint32 width, uint32 height ) {
			return 1 + std::floor(std::log2(std::max(width, height)));
		}

		/// @return Value of enum to lookup array.
		/// @param e Enum.
		template<typename E>
			requires std::is_enum_v<E>
		constexpr inline std::underlying_type_t<E> lookup_cast(E e) {
			return to_underlying(e);
		}

		template<typename T>
			requires std::is_arithmetic_v<T>
		inline constexpr T null_id() {
			return max_val<T>();
		}

		struct BufferHandle;
		struct VertexLayoutHandle;
		struct ShaderHandle;
		
		using BufferID			= uint32;
		using LayoutID			= uint32;
		using TextureID			= uint32;
		using SamplerID			= uint32;
		using FramebufferID		= uint32;
		using PipelineID		= uint32;
		using ShaderID			= uint8;
		using EnumFlag			= uint16;
		using DepthBiasVal		= float32;

		enum class DataFormat : EnumFlag {
			Float1,
			Vec2,
			Vec3,
			Vec4,
			Mat3,
			Mat4
		};

		enum class BufferType : EnumFlag {
			Vertex, // Buffer contains vertices.
			Element,// Buffer contains indices (elements).
			Uniform	// Buffer contains uniforms.
		};

		enum class BufferUsage : EnumFlag {
			Static, // Data cannot be updated during runtime (better performance).
			Dynamic // Data can be updated during runtime (slower performance).
		};

		// Specifies how polygons will be rasterized.
		enum class TopologyMode : EnumFlag {
			Point,	// Polygon vertices that are marked as the start of a boundary edge are drawn as points.
			Line,	// Boundary edges of the polygon are drawn as line segments.
			Fill	// The interior of the polygon is filled.
		};

		enum class Mapflag : EnumFlag {
			Persistent			= 1 << 0, // Pointer to the mapped data can be available all the time.
			Write				= 1 << 1, // CPU can only write data through mapping.
			Read				= 1 << 2, // CPU can only read data through mapping.
			Coherent			= 1 << 3, // CPU changes instantly are available for GPU.
			Invalidate_Range	= 1 << 4, // GPU creates new range in buffer, old data is still available.
			Invalidate_Buffer	= 1 << 5, // GPU creates new buffer, destroys the old one.
			Unsynchronized		= 1 << 6, // GPU maps buffer with no backend conditions.
		};

		constexpr Mapflag operator|(Mapflag a, Mapflag b) noexcept {
			return static_cast<Mapflag>(static_cast<EnumFlag>(a) | static_cast<EnumFlag>(b));
		}
		constexpr bool operator&(Mapflag a, Mapflag b) noexcept {
			return static_cast<EnumFlag>(a) & static_cast<EnumFlag>(b);
		}
		constexpr Mapflag operator~(Mapflag a) noexcept {
			return static_cast<Mapflag>(~static_cast<EnumFlag>(a));
		}

		enum class Face : EnumFlag {
			Front,
			Back,
			FrontBack,
		};

		enum class WindingOrder : EnumFlag {
			CounterClockwise, // Front-facing triangles are defined in counter-clockwise order ( CCW)
			Clockwise         // Front-facing triangles are defined in clockwise order ( CW )
		};

		enum class State : EnumFlag {
			None			= 1 << 0,
			DepthTest		= 1 << 1,
			DepthWrite		= 1 << 2,
			StencilTest		= 1 << 3,
			Scissor			= 1 << 4,
			Blend			= 1 << 5,
			Cull			= 1 << 6,
			DepthBias		= 1 << 7
		};

		namespace detail {

			LUM_COMPILE_VARIABLE
			static uint8 skDataFormatLookup[] = { 1, 2, 3, 4, 9, 16 };

		}

	}


}