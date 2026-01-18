#pragma once
#include "core/core_pch.hpp"

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
		inline constexpr unsigned int mipmap_lvls(unsigned int width, unsigned int height) {
			return 1 + std::floor(std::log2(std::max(width, height)));
		}

		struct BufferHandle;
		struct VertexLayoutHandle;
		struct ShaderHandle;

		using bufferid_t		= uint32_t;
		using layoutid_t		= uint32_t;
		using textureid_t		= uint32_t;
		using samplerid_t		= uint32_t;
		using shaderid_t		= uint8_t;
		using mapflag_t			= uint16_t;
		using layoutformat_t	= uint8_t;

		namespace format {

			LUM_CONST_VAR_QUALIFIER layoutformat_t Float = 1;
			LUM_CONST_VAR_QUALIFIER layoutformat_t Float2 = 2;
			LUM_CONST_VAR_QUALIFIER layoutformat_t Float3 = 3;
			LUM_CONST_VAR_QUALIFIER layoutformat_t Float4 = 4;

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

			LUM_CONST_VAR_QUALIFIER mapflag_t Persistent		= 1 << 0; // Pointer to the mapped data can be available all the time.
			LUM_CONST_VAR_QUALIFIER mapflag_t Write				= 1 << 1; // CPU can only write data through mapping.
			LUM_CONST_VAR_QUALIFIER mapflag_t Read				= 1 << 2; // CPU can only read data through mapping.
			LUM_CONST_VAR_QUALIFIER mapflag_t Coherent			= 1 << 3; // CPU changes instantly are available for GPU.
			LUM_CONST_VAR_QUALIFIER mapflag_t Invalidate_Range	= 1 << 4; // GPU creates new range in buffer, old data is still available.
			LUM_CONST_VAR_QUALIFIER mapflag_t Invalidate_Buffer	= 1 << 5; // GPU creates new buffer, destroys the old one.
			LUM_CONST_VAR_QUALIFIER mapflag_t Unsynchronized	= 1 << 6; // GPU maps buffer with no backend conditions.

		}

	}


}