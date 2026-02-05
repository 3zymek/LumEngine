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

		virtual void	set_width		( uint32 ) noexcept = 0;
		virtual void	set_height		( uint32 ) noexcept = 0;
		virtual uint32	get_width		( ) const noexcept = 0;
		virtual uint32	get_height		( ) const noexcept = 0;
		virtual vptr	get_native_window	( ) const noexcept = 0;
		virtual bool	is_open				( ) const noexcept = 0;
		virtual RenderBackend	get_backend	( )	const noexcept = 0;

	protected:

		virtual void init( const WindowDescriptor& ) = 0;
		RenderBackend backend{};

	};

	class OpenGLWindow : public Window {
	public:

		OpenGLWindow( const WindowDescriptor& desc ) { init(desc); }

		void					set_width		( uint32	) noexcept override ;
		void					set_height		( uint32	) noexcept override;
		uint32					get_width		(			) const noexcept override;
		uint32					get_height		(			) const noexcept override;
		vptr					get_native_window(			) const noexcept override;
		bool					is_open			(			) const noexcept override;
		RenderBackend			get_backend		(			) const noexcept override;

	private:

		uint32 mWidth = 0;
		uint32 mHeight = 0;
		
		GLFWwindow* mWindow = nullptr;

		void init( const WindowDescriptor& ) override;

	};

	Window* CreateWindow(const WindowDescriptor&);

}