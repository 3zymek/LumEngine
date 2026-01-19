#pragma once
#include "rhi/rhi_pch.hpp"
#include "core/core_common.hpp"
namespace lum {

	enum class RenderBackend : LUMbyte { OpenGL, /*Vulkan*/ };

	struct WindowDescriptor {
		LUMcharptr title		= "LumEngine";
		LUMbool fullscreen		= false;
		LUMuint height			= 500;
		LUMuint width			= 500;
		LUMuint MSAA_samples	= 0;
		RenderBackend render	= RenderBackend::OpenGL;
	};

	class Window {
	public:

		virtual LUMuint	GetWidth		( ) = 0;
		virtual LUMuint	GetHeight		( ) = 0;
		virtual LUMvptr	GetNativeWindow	( ) = 0;
		virtual LUMbool	IsOpen			( ) = 0;
		virtual RenderBackend	GetBackend		( )	= 0;

	protected:

		virtual void Init( const WindowDescriptor& ) = 0;
		RenderBackend backend{};

	};

	class OpenGL_Window : public Window {
	public:

		OpenGL_Window( const WindowDescriptor& desc ) { Init(desc); }

		LUMuint					GetWidth		( ) override;
		LUMuint					GetHeight		( ) override;
		LUMvptr					GetNativeWindow	( ) override;
		LUMbool					IsOpen			( ) override;
		virtual RenderBackend	GetBackend		( ) override;

	private:

		LUMuint width = 0;
		LUMuint height = 0;
		
		GLFWwindow* m_window = nullptr;

		void Init( const WindowDescriptor& ) override;

	};

	Window* CreateWindow(const WindowDescriptor&);

}