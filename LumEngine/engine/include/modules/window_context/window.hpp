#pragma once
#include "render/render_pch.hpp"

namespace lum {

	enum class RenderBackend : uint8_t { OpenGL, /*Vulkan*/ };

	struct WindowDescriptor {
		const char* title = "LumEngine";
		bool fullscreen = false;
		unsigned int height = 500;
		unsigned int width = 500;
		unsigned int MSAA_samples = 0;
		RenderBackend render = RenderBackend::OpenGL;
	};

	class Window {
	public:

		virtual unsigned int GetWidth		( ) = 0;
		virtual unsigned int GetHeight		( ) = 0;
		virtual void*		GetNativeWindow	( ) = 0;
		virtual bool		IsOpen			( ) = 0;

	private:

		virtual void Init(const WindowDescriptor&) = 0;

	};

	class OpenGL_Window : public Window {
	public:

		OpenGL_Window(const WindowDescriptor& desc) { Init(desc); }

		unsigned int	GetWidth		( ) override;
		unsigned int	GetHeight		( ) override;
		void*			GetNativeWindow	( ) override;
		bool			IsOpen			( ) override;

	private:

		unsigned int width = 0;
		unsigned int height = 0;
		
		GLFWwindow* m_window = nullptr;

		void Init(const WindowDescriptor&) override;

	};

	Window* CreateWindow(const WindowDescriptor&);

}