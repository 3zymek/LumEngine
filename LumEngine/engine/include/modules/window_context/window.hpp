#pragma once
#include "rhi/rhi_pch.hpp"
#include "core/core_common.hpp"
namespace lum {

	enum class RenderBackend : byte { OpenGL, /*Vulkan*/ };

	struct WindowDescriptor {
		ccharptr title			= "LumEngine";
		bool fullscreen			= false;
		uint32 height			= 500;
		uint32 width			= 500;
		uint32 MSAA_samples		= 0;
		RenderBackend render	= RenderBackend::OpenGL;
	};

	class Window {
	public:

		virtual void	SetWidth		( uint32 ) = 0;
		virtual void	SetHeight		( uint32 ) = 0;
		virtual uint32	GetWidth		( ) = 0;
		virtual uint32	GetHeight		( ) = 0;
		virtual vptr	GetNativeWindow	( ) = 0;
		virtual bool	IsOpen			( ) = 0;
		virtual RenderBackend	GetBackend		( )	= 0;

	protected:

		virtual void Init( const WindowDescriptor& ) = 0;
		RenderBackend backend{};

	};

	class OpenGLWindow : public Window {
	public:

		OpenGLWindow( const WindowDescriptor& desc ) { Init(desc); }

		void					SetWidth		( uint32 ) override;
		void					SetHeight		( uint32 ) override;
		uint32					GetWidth		( ) override;
		uint32					GetHeight		( ) override;
		vptr					GetNativeWindow	( ) override;
		bool					IsOpen			( ) override;
		RenderBackend			GetBackend		( ) override;

	private:

		uint32 mWidth = 0;
		uint32 mHeight = 0;
		
		GLFWwindow* mWindow = nullptr;

		void Init( const WindowDescriptor& ) override;

	};

	Window* CreateWindow(const WindowDescriptor&);

}