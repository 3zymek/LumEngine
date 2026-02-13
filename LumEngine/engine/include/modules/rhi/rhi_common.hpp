#pragma once
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
namespace lum {

	struct alignas(16) Vertex {
		math::Vec3 position;	// Vertex position in 3D space
		math::Vec3 normal;		// Vertex normal
		math::Vec2 uv;			// Texture coordinates (UV)
	};

	namespace rhi {

		// Returns number of mipmap levels for an image given width and height.
		// @param width Width of the image.
		// @param height Height of the image.
		// @return Number of mipmap levels (>= 1).
		inline uint32 mipmap_lvls(uint32 width, uint32 height) {
			return 1 + std::floor(std::log2(std::max(width, height)));
		}

		// Casts enum to its underlying integer type for array lookups or bitmasks.
		// @param e Enum value.
		// @return Underlying integer representation.
		template<typename E>
			requires std::is_enum_v<E>
		constexpr inline std::underlying_type_t<E> lookup_cast(E e) {
			return ToUnderlyingEnum(e);
		}

		// Returns a special "null" value for arithmetic types (usually max value).
		// Useful for invalid IDs or handles.
		template<typename T>
			requires std::is_arithmetic_v<T>
		inline constexpr T null_id() {
			return MaxVal<T>();
		}

		struct PipelineHandle		: cstd::BaseHandle {}; // Pipeline state handle
		struct FramebufferHandle	: cstd::BaseHandle {}; // Framebuffer handle
		struct SamplerHandle		: cstd::BaseHandle {}; // Texture sampler handle
		struct ShaderHandle			: cstd::BaseHandle {}; // Shader handle
		struct TextureHandle		: cstd::BaseHandle {}; // Texture handle
		struct BufferHandle			: cstd::BaseHandle {}; // Buffer handle
		struct VertexLayoutHandle	: cstd::BaseHandle {}; // Vertex layout handle

		using BufferID		= uint32; // Numeric ID for buffer
		using LayoutID		= uint32; // Numeric ID for vertex layout
		using TextureID		= uint32; // Numeric ID for texture
		using SamplerID		= uint32; // Numeric ID for sampler
		using FramebufferID = uint32; // Numeric ID for framebuffer
		using PipelineID	= uint32; // Numeric ID for pipeline
		using ShaderID		= uint8;  // Numeric ID for shader (compact)

		using EnumFlag = uint16;        // Bitmask storage type
		using DepthBiasVal = float32;   // Depth bias value type

		using ChannelRGBA = math::Vec4; // RGBA color channel type

		enum class ClearFlag : EnumFlag {
			Color = 1 << 0,     // Clear color buffer
			Depth = 1 << 1,     // Clear depth buffer
			Stencil = 1 << 2    // Clear stencil buffer
		};

		enum class DataFormat : EnumFlag {
			Float1, // Single float
			Vec2,   // 2-component vector (x,y)
			Vec3,   // 3-component vector (x,y,z)
			Vec4,   // 4-component vector (x,y,z,w)
			Mat3,   // 3x3 matrix
			Mat4    // 4x4 matrix
		};

		enum class BufferType : EnumFlag {
			Vertex,        // Vertex buffer (VBO)
			Element,       // Element / index buffer (EBO)
			Uniform,       // Uniform buffer (UBO)
			ShaderStorage, // Shader storage buffer (SSBO)
		};

		enum class BufferUsage : EnumFlag {
			Static,  // Data does not change during runtime (fast)
			Dynamic  // Data updated during runtime (slower)
		};

		enum class TopologyMode : EnumFlag {
			Point, // Draw vertices as points
			Line,  // Draw edges as lines
			Fill   // Fill polygon interiors
		};

		enum class Mapflag : EnumFlag {
			Persistent = 1 << 0,        // Pointer remains valid across frames
			Write = 1 << 1,             // CPU can write
			Read = 1 << 2,              // CPU can read
			Coherent = 1 << 3,          // Changes instantly visible to GPU
			Invalidate_Range = 1 << 4,  // GPU allocates new range, old still valid
			Invalidate_Buffer = 1 << 5, // GPU allocates new buffer, old discarded
			Unsynchronized = 1 << 6,    // Map without GPU sync guarantees
		};

		enum class Face : EnumFlag {
			Front,      // Front-facing polygon
			Back,       // Back-facing polygon
			FrontBack,  // Both faces
		};

		enum class WindingOrder : EnumFlag {
			CounterClockwise, // Triangles front-facing if CCW
			Clockwise         // Triangles front-facing if CW
		};

		enum class State : EnumFlag {
			None						= 1 << 0, // No state
			DepthTest					= 1 << 1, // Depth testing enabled
			DepthWrite					= 1 << 2, // Depth writes enabled
			StencilTest					= 1 << 3, // Stencil testing enabled
			Scissor						= 1 << 4, // Scissor test enabled
			Blend						= 1 << 5, // Blending enabled
			Cull						= 1 << 6, // Backface culling enabled
			DepthBias					= 1 << 7, // Depth bias enabled
			Multisample					= 1 << 8, // MSAA enabled
			MultisampleCoverage			= 1 << 9, // Sample coverage enabled
			MultisampleAlphaToOne		= 1 << 10,// Alpha-to-one enabled
			MultisampleAlphaToCoverage	= 1 << 11,// Alpha-to-coverage enabled
		};

		struct ColorMask {
			bool r : 1 = true; // Red channel write enabled
			bool g : 1 = true; // Green channel write enabled
			bool b : 1 = true; // Blue channel write enabled
			bool a : 1 = true; // Alpha channel write enabled
		};

		namespace detail {
			LUM_COMPILE_VARIABLE
				static uint8 skDataFormatLookup[] = { 1, 2, 3, 4, 9, 16 }; // Lookup for data format sizes
		}

	} // namespace rhi
} // namespace lum