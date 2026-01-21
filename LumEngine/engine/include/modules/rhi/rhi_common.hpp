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
		using shaderid		= uint8;
		using mapflag		= uint16;

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
			Uniform // Buffer contains uniforms.
		};

		enum class BufferUsage : byte {
			Static, // Data cannot be updated during runtime (better performance).
			Dynamic // Data can be updated during runtime (slower performance).
		};

		namespace map_flags {

			LUM_CONST_VAR_QUALIFIER mapflag Persistent			= 1 << 0; // Pointer to the mapped data can be available all the time.
			LUM_CONST_VAR_QUALIFIER mapflag Write				= 1 << 1; // CPU can only write data through mapping.
			LUM_CONST_VAR_QUALIFIER mapflag Read				= 1 << 2; // CPU can only read data through mapping.
			LUM_CONST_VAR_QUALIFIER mapflag Coherent			= 1 << 3; // CPU changes instantly are available for GPU.
			LUM_CONST_VAR_QUALIFIER mapflag Invalidate_Range	= 1 << 4; // GPU creates new range in buffer, old data is still available.
			LUM_CONST_VAR_QUALIFIER mapflag Invalidate_Buffer	= 1 << 5; // GPU creates new buffer, destroys the old one.
			LUM_CONST_VAR_QUALIFIER mapflag Unsynchronized		= 1 << 6; // GPU maps buffer with no backend conditions.

		}

	}


}