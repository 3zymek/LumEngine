#pragma once
#include "core/core_pch.hpp"

namespace lum {

	struct Vertex {
		glm::vec3 position;
		glm::vec3 color;
	};

	namespace rhi {

		struct BufferHandle;
		struct VertexLayoutHandle;
		struct ShaderHandle;

		using MapFlag				= uint32_t;
		using VertexLayoutFormat	= uint8_t;

		namespace format {

			inline constexpr VertexLayoutFormat Float = 1;
			inline constexpr VertexLayoutFormat Float2 = 2;
			inline constexpr VertexLayoutFormat Float3 = 3;
			inline constexpr VertexLayoutFormat Float4 = 4;

		}

		enum class BufferType : uint8_t {
			Vertex, // Buffer contains vertices.
			Element,// Buffer contains indices (elements).
			Uniform // Buffer contains uniforms.
		};

		enum class BufferUsage : uint8_t {
			Static, // Data cannot be updated during runtime (better performance).
			Dynamic // Data can be updated during runtime (slower performance).
		};

		namespace map_flags {

			inline constexpr MapFlag Persistent			= 1 << 0; // Pointer to the mapped data can be available all the time.
			inline constexpr MapFlag Write				= 1 << 1; // CPU can only write data through mapping.
			inline constexpr MapFlag Read				= 1 << 2; // CPU can only read data through mapping.
			inline constexpr MapFlag Coherent			= 1 << 3; // CPU changes instantly are available for GPU.
			inline constexpr MapFlag Invalidate_Range	= 1 << 4; // GPU creates new range in buffer, old data is still available.
			inline constexpr MapFlag Invalidate_Buffer	= 1 << 5; // GPU creates new buffer, destroys the old one.
			inline constexpr MapFlag Unsynchronized		= 1 << 6; // GPU maps buffer with no backend conditions.

		}

	}


}