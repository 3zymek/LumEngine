#pragma once
#include "rhi/core/rhi_device.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "render/render_pch.hpp"
#include "core/core_defines.hpp"
namespace lum::gl {

	class GL_Device : rhi::RHI_Device {
	public:

		void BeginFrame() override {

		}
		void EndFrame() override {

		}

		rhi::RHIBHandle CreateBuffer(const rhi::RHI_BufferDescriptor& desc) override {
			if (m_buffers.DenseSize() >= MAX_BUFFERS) {
				LOG_ERROR("Max buffers reached");
				return rhi::RHIBHandle{};
			}
			if (desc.size <= 0) {
				LOG_WARN("Invalid buffer size");
				return rhi::RHIBHandle{};
			}
			
			rhi::RHI_Buffer buffer;

			glCreateBuffers(1, &buffer.handle.gl_handle);
			glNamedBufferStorage(buffer.handle.gl_handle, desc.size, nullptr, TranslateBufferFlags(desc.flags));

			return m_buffers.CreateHandle(std::move(buffer));
		}
		void UpdateBuffer(rhi::RHI_BufferHandle handle) override {
			if (!m_buffers.Exists(handle)) return;



		}


	private:

		GLenum TranslateBufferFlags(rhi::RHIBFlag flags) {
			GLenum flag = 0;

			if (flags & rhi::RHIBFlag::Static && flags & rhi::RHIBFlag::Map_Write) {
				LOG_ERROR("Invalid buffer flags combination");
				return 0;
			}
			
			switch (flags) {
			case rhi::RHIBFlag::Dynamic:		flag |= GL_DYNAMIC_STORAGE_BIT;
			case rhi::RHIBFlag::Map_Persistent: flag |= GL_MAP_PERSISTENT_BIT;
			case rhi::RHIBFlag::Map_Write:		flag |= GL_MAP_WRITE_BIT;
			case rhi::RHIBFlag::Map_Read:		flag |= GL_MAP_READ_BIT;
			case rhi::RHIBFlag::Map_Coherent:	flag |= GL_MAP_COHERENT_BIT;
			}

			return flag;
		}

	};

}