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

		struct BufferHandle;
		struct VertexLayoutHandle;
		struct ShaderHandle;
		
		using bufferid		= uint32;
		using layoutid		= uint32;
		using textureid		= uint32;
		using samplerid		= uint32;
		using framebufferid = uint32;
		using pipelineid	= uint32;
		using shaderid		= uint8;

		enum class DataFormat : byte {
			Float,
			Float2,
			Float3,
			Float4,
			Mat3,
			Mat4,
			Quat
		};

		enum class BufferType : byte {
			Vertex, // Buffer contains vertices.
			Element,// Buffer contains indices (elements).
			Uniform// Buffer contains uniforms.
		};

		enum class BufferUsage : byte {
			Static, // Data cannot be updated during runtime (better performance).
			Dynamic // Data can be updated during runtime (slower performance).
		};

		enum class PolygonMode : byte {
			point,
			line,
			fill
		};

		enum class Mapflag : byte {
			Persistent, // Pointer to the mapped data can be available all the time.
			Write, // CPU can only write data through mapping.
			Read, // CPU can only read data through mapping.
			Coherent, // CPU changes instantly are available for GPU.
			Invalidate_Range, // GPU creates new range in buffer, old data is still available.
			Invalidate_Buffer, // GPU creates new buffer, destroys the old one.
			Unsynchronized, // GPU maps buffer with no backend conditions.
		};

		constexpr Mapflag operator|(Mapflag a, Mapflag b) noexcept {
			return static_cast<Mapflag>(static_cast<byte>(a) | static_cast<byte>(b));
		}
		constexpr bool operator&(Mapflag a, Mapflag b) noexcept {
			return static_cast<byte>(a) & static_cast<byte>(b);
		}
		constexpr Mapflag operator~(Mapflag a) noexcept {
			return static_cast<Mapflag>(~static_cast<byte>(a));
		}


		enum class Face : byte {
			front,
			back,
			front_back,
		};

		enum class State : byte {
			none = 0,
			depth_test = 0xAA,
			stencil_test = 0xAB,
			scissor = 0xAC,
			blend = 0xAD,
			cull_face = 0xAE,
			// polygon
		};

		constexpr State operator|(State a, State b) noexcept {
			return static_cast<State>(static_cast<byte>(a) | static_cast<byte>(b));
		}
		constexpr bool operator&(State a, State b) noexcept {
			return static_cast<byte>(a) & static_cast<byte>(b);
		}
		constexpr State operator~(State a) noexcept {
			return static_cast<State>(~static_cast<byte>(a));
		}

		namespace detail {

			LUM_CONST_VAR_QUALIFIER
			static uint8 skDataFormatLookup[] = { 1, 2, 3, 4, 9, 16 };

		}

	}


}