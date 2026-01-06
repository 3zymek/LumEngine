#pragma once
#include "core/core_defines.hpp"
#include "core/core_pch.hpp"
#include "render/render_pch.hpp"
namespace lum {
	namespace render {

		using Index = uint32_t;
		struct Vertex {
			glm::vec3 position;
			glm::vec3 color;
			glm::vec3 normal;
			glm::vec2 uv;
		};

		// Mesh
		inline constexpr unsigned int MAX_MESHES_AMOUNT = 100000;

		namespace detail {

			using Generation = uint32_t;
			using MeshID = uint64_t;
			using VertexCount = size_t;
			using VertexArray = std::vector<Vertex>;
			using IndexArray = std::vector<Index>;

			struct StaticMesh;
			struct DynamicMesh;

			template<typename T>
			concept MeshT = std::same_as<T, StaticMesh> || std::same_as<T, DynamicMesh>;

		}

		// Shader

		namespace detail {

			using ShaderID = uint32_t;

		}


		// Texture

	}

	// Texture
	using TextureHandle = uint64_t;



	// Material
	using MaterialHandle = uint64_t;




	namespace render {

		enum class RenderMode : Bit {
			Solid,
			Wireframe,
			Points,
		};

		enum class RenderFeature : Bit {
			None			= 0x00000000,
			AntiAliasing	= 0x00000010,
			Bloom			= 0x00000100,
			Shadows			= 0x00001000,
			SSAO			= 0x00010000,
			Reflection		= 0x00100000,
		};

		struct RenderConfig {

			// Window

			unsigned int width = 500, height = 500;
			bool fullscreen = false;
			std::string title = "";

			// Render

			RenderMode render_mode{};
			unsigned int MSAA_samples = 1;
			bool vSync = false;

		};

		struct MaterialInitParams {

		};


		inline RenderFeature operator|(RenderFeature a, RenderFeature b) {
			return static_cast<RenderFeature>(
				static_cast<Bit>(a) | static_cast<Bit>(b)
				);
		}
		inline RenderFeature operator&(RenderFeature a, RenderFeature b) {
			return static_cast<RenderFeature>(
				static_cast<Bit>(a) & static_cast<Bit>(b)
				);
		}

		namespace detail {

			inline void APIENTRY GLDebugCallback(
				GLenum src,
				GLuint id,
				GLenum severity,
				GLsizei lenght,
				const GLchar* message,
				const void* userParam
			)
			{
				if (severity == GL_DEBUG_SEVERITY_HIGH) {
					LOG_ERROR(message);
					__debugbreak();
				}

				if (severity != GL_DEBUG_SEVERITY_NOTIFICATION);


			}
		}
	}
}