#pragma once
#include "rhi/rhi_pch.hpp"
#include "core/core_common.hpp"
namespace lum {

	enum class RenderBackend : bitfield { OpenGL, /*Vulkan*/ };

	struct WindowDescriptor {
		ccharptr title			= "LumEngine";
		bool bFullscreen			= false;
		uint32 height			= 500;
		uint32 width			= 500;
		uint32 msaaSamples		= 0;
		RenderBackend backend	= RenderBackend::OpenGL;
	};

	class Window {
	public:

		virtual void	SetWidth		( uint32 ) noexcept = 0;
		virtual void	SetHeight		( uint32 ) noexcept = 0;
		virtual uint32	GetWidth		( ) const noexcept = 0;
		virtual uint32	GetHeight		( ) const noexcept = 0;
		virtual vptr	GetNativeWindow	( ) const noexcept = 0;
		virtual bool	IsOpen				( ) const noexcept = 0;
		virtual RenderBackend	GetBackend	( )	const noexcept = 0;

	protected:

		virtual void Init( const WindowDescriptor& ) = 0;
		RenderBackend backend{};

	};

	class OpenGLWindow : public Window {
	public:

		OpenGLWindow( const WindowDescriptor& desc ) { Init(desc); }

		void					SetWidth		( uint32	) noexcept override ;
		void					SetHeight		( uint32	) noexcept override;
		uint32					GetWidth		(			) const noexcept override;
		uint32					GetHeight		(			) const noexcept override;
		vptr					GetNativeWindow(			) const noexcept override;
		bool					IsOpen			(			) const noexcept override;
		RenderBackend			GetBackend		(			) const noexcept override;

	private:

		uint32 mWidth = 0;
		uint32 mHeight = 0;
		
		GLFWwindow* mWindow = nullptr;

		void Init( const WindowDescriptor& ) override;

	};

	Window* CreateWindow(const WindowDescriptor&);

}