#pragma once
#include "core_defines.hpp"
namespace render {

	enum class RenderMode : BitFlags {
		Solid,
		Wireframe,
		Points,
	};

	enum class RenderFeature : BitFlags {
		None			= 0x00000000,
		AntiAliasing	= 0x00000010,
		Bloom			= 0x00000100,
		Shadows			= 0x00001000,
		SSAO			= 0x00010000,
		Reflection		= 0x00100000,
	};

	struct RenderInitParams {

		// Window

		unsigned int width = 500, height = 500;
		bool fullscreen = false;
		std::string title = "";

		// Render

		RenderMode render_mode;
		unsigned int MSAA_samples = 1;
		bool vSync = false;

	};


	inline RenderFeature operator|(RenderFeature a, RenderFeature b ) {
		return static_cast<RenderFeature>(
			static_cast<BitFlags>(a) | static_cast<BitFlags>(b)
			);
	}
	inline RenderFeature operator&(RenderFeature a, RenderFeature b ) {
		return static_cast<RenderFeature>(
			static_cast<BitFlags>(a) & static_cast<BitFlags>(b)
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
			
			if(severity != GL_DEBUG_SEVERITY_NOTIFICATION)
				RENDER_LOG(message);

		}



	}

}

