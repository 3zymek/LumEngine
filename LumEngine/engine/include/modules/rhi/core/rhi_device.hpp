#pragma once
#include "core/core_pch.hpp"
#include "core/core_defines.hpp"
#include "core/utils/handle_pool.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/rhi_common.hpp"
namespace lum { class Window; }
namespace lum::rhi {

	class RHI_Device {
	public:

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		// VBO EBO UBO
		virtual RHIbhandle CreateBuffer(const RHI_BufferDescriptor&) = 0;
		virtual void UpdateBuffer(RHIbhandle, const void*, size_t, size_t, rhi::RHIbflag) = 0;
		virtual void DeleteBuffer(RHIbhandle&) = 0;
		virtual void* MapBuffer(RHIbhandle, RHIbflag) = 0;
		virtual void UnmapBuffer(RHIbhandle) = 0;

		// VAO
		virtual void CreateVertexLayout() = 0;
		virtual void DeleteVertexLayout() = 0;


		virtual void Draw() = 0;

	protected:

		static constexpr unsigned int MAX_BUFFERS = 10000;
		static constexpr unsigned int MAX_LAYOUTS = 10000;

		cstd::handle_pool<RHI_Buffer, RHIbhandle> m_buffers{ MAX_BUFFERS };

	};

	RHI_Device* CreateDevice(Window*);


}