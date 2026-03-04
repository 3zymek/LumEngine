#pragma once
#include "rhi/rhi_pch.hpp"
#include "core/core_common.hpp"
namespace lum {

	namespace ev { class EventBus; }

	struct WindowDescriptor {

		String mTitle			= "LumEngine";
		uint32 mHeight			= 500;
		uint32 mWidth			= 500;
		ev::EventBus* mEventBus = nullptr;

	};

	class Window {
	public:

		void Initialize( const WindowDescriptor& desc );

		void SetWidth( uint32 );
		void SetHeight( uint32 );
		uint32 GetWidth( ) const noexcept;
		uint32 GetHeight( ) const noexcept;
		vptr GetNativeWindow( ) const noexcept;
		void Update( ) noexcept;
		bool IsOpen( ) const noexcept;

	protected:

		GLFWwindow* mWindow = nullptr;
		ev::EventBus* mEventBus = nullptr;

		uint32 mWidth = 0;
		uint32 mHeight = 0;

		void init( const WindowDescriptor& );

	};

	Window* CreateWindow(const WindowDescriptor&);

}