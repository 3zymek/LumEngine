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
			float1,
			vec2,
			vec3,
			vec4,
			mat3,
			mat4
		};

		enum class BufferType : byte {
			vertex, // Buffer contains vertices.
			element,// Buffer contains indices (elements).
			uniform// Buffer contains uniforms.
		};

		enum class BufferUsage : byte {
			static_usage, // Data cannot be updated during runtime (better performance).
			dynamic_usage // Data can be updated during runtime (slower performance).
		};

		enum class PolygonMode : byte {
			point,
			line,
			fill
		};

		enum class Mapflag : byte {
			persistent			= 1 << 0, // Pointer to the mapped data can be available all the time.
			write				= 1 << 1, // CPU can only write data through mapping.
			read				= 1 << 2, // CPU can only read data through mapping.
			coherent			= 1 << 3, // CPU changes instantly are available for GPU.
			invalidate_Range	= 1 << 4, // GPU creates new range in buffer, old data is still available.
			invalidate_Buffer	= 1 << 5, // GPU creates new buffer, destroys the old one.
			unsynchronized		= 1 << 6, // GPU maps buffer with no backend conditions.
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
			none			= 1 << 0,
			depth_test		= 1 << 1,
			stencil_test	= 1 << 2,
			scissor			= 1 << 3,
			blend			= 1 << 4,
			cull_face		= 1 << 5,
		};

		constexpr State operator|(State a, State b) noexcept {
			return static_cast<State>(static_cast<byte>(a) | static_cast<byte>(b));
		}
		constexpr State operator|=(State& a, State b) noexcept {
			a = a | b;
			return a;
		}

		constexpr bool operator&(State a, State b) noexcept {
			return (static_cast<byte>(a) & static_cast<byte>(b)) != 0;
		}
		constexpr bool operator&(byte a, State b) noexcept {
			return (a & static_cast<byte>(b)) != 0;
		}
		constexpr bool operator&(State a, byte b) noexcept {
			return (static_cast<byte>(a) & b) != 0;
		}

		constexpr State operator~(State a) noexcept {
			return static_cast<State>(~static_cast<byte>(a));
		}

		constexpr State& operator^=(State& a, State b) {
			a = static_cast<State>(static_cast<byte>(a) ^ static_cast<byte>(b));
			return a;
		}

		namespace detail {

			LUM_CONST_VAR_QUALIFIER
			static uint8 skDataFormatLookup[] = { 1, 2, 3, 4, 9, 16 };

		}

	}


}