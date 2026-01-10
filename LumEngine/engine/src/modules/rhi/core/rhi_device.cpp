#include "rhi/core/rhi_device.hpp"
#include "window_context/window.hpp"
#include "rhi/backend/opengl/gl_device.hpp"
namespace lum::rhi {

	RHI_Device* CreateDevice(Window* window) {
		RenderBackend backend = window->GetBackend();
		switch (backend) {
		case RenderBackend::OpenGL: return new gl::GL_Device(window);
		}
		return nullptr;
	}

}