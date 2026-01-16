#pragma once
#include "core/core_pch.hpp"

namespace lum {

	struct alignas(16) Vertex {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 uv;
	};

	namespace rhi {
		#define RHI_FUNC_QUALIFIER __forceinline
		#define RHI_VAR_QUALIFIER inline constexpr

		/// Returns amount of mipmap levels count for image.
		/// @param width Width of an image.
		/// @param height Height of an image.
		/// @return Mipmap levels count.
		RHI_FUNC_QUALIFIER
		unsigned int mipmap_lvls(unsigned int width, unsigned int height) {
			return 1 + std::floor(std::log2(std::max(width, height)));
		}

		struct BufferHandle;
		struct VertexLayoutHandle;
		struct ShaderHandle;

		using MapFlag				= uint32_t;
		using VertexLayoutFormat	= uint8_t;

		namespace format {

			RHI_VAR_QUALIFIER VertexLayoutFormat Float = 1;
			RHI_VAR_QUALIFIER VertexLayoutFormat Float2 = 2;
			RHI_VAR_QUALIFIER VertexLayoutFormat Float3 = 3;
			RHI_VAR_QUALIFIER VertexLayoutFormat Float4 = 4;

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

			RHI_VAR_QUALIFIER MapFlag Persistent			= 1 << 0; // Pointer to the mapped data can be available all the time.
			RHI_VAR_QUALIFIER MapFlag Write				= 1 << 1; // CPU can only write data through mapping.
			RHI_VAR_QUALIFIER MapFlag Read				= 1 << 2; // CPU can only read data through mapping.
			RHI_VAR_QUALIFIER MapFlag Coherent			= 1 << 3; // CPU changes instantly are available for GPU.
			RHI_VAR_QUALIFIER MapFlag Invalidate_Range	= 1 << 4; // GPU creates new range in buffer, old data is still available.
			RHI_VAR_QUALIFIER MapFlag Invalidate_Buffer	= 1 << 5; // GPU creates new buffer, destroys the old one.
			RHI_VAR_QUALIFIER MapFlag Unsynchronized		= 1 << 6; // GPU maps buffer with no backend conditions.

		}

	}


}