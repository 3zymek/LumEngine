#pragma once
#include "rhi/core/rhi_device.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/rhi_pch.hpp"
#include "core/core_defines.hpp"
namespace lum::gl {

	class GL_Device : public rhi::RHI_Device {
	public:

		GL_Device(Window* win) : window(win) {}

		void BeginFrame() override {

			glClearColor(0.2, 0.2, 0.2, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);
			glfwPollEvents();

		}
		void EndFrame() override {

			glfwSwapBuffers(static_cast<GLFWwindow*>(window->GetNativeWindow()));

		}


		rhi::RHIbhandle CreateBuffer(const rhi::RHI_BufferDescriptor& desc) override {
			if (m_buffers.DenseSize() >= MAX_BUFFERS) {
				LOG_ERROR("Max buffers reached");
				return rhi::RHIbhandle{};
			}
			if (desc.size <= 0) {
				LOG_WARN("Invalid buffer size");
				return rhi::RHIbhandle{};
			}
			
			rhi::RHI_Buffer buffer;
			buffer.size = desc.size;
			buffer.flags = desc.flags;


			glCreateBuffers(1, &buffer.handle.gl_handle);
			glNamedBufferStorage(
				buffer.handle.gl_handle, 
				desc.size, 
				nullptr, 
				TranslateBufferDataFlags(desc.flags)
			);

			return m_buffers.CreateHandle(std::move(buffer));
		}
		void UpdateBuffer(rhi::RHI_BufferHandle handle, const void* data, size_t offset, size_t size, rhi::RHIbflag flags) override {
			HOTPATH_ASSERT_VOID(!m_buffers.Exists(handle), "Buffer does not exists");
			rhi::RHI_Buffer& buffer = m_buffers[handle];
			HOTPATH_ASSERT_VOID(offset + size > buffer.size, "Invalid offset or size");
			if (size == 0) size = buffer.size;
			void* ptr = 
				glMapNamedBufferRange(
					buffer.handle.gl_handle, 
					offset, 
					size, 
					TranslateMappingFlags(flags)
				);
			HOTPATH_ASSERT_VOID(!ptr, "Error during mapping");
			std::memcpy(ptr, data, buffer.size);
			glUnmapNamedBuffer(buffer.handle.gl_handle);

		}
		void DeleteBuffer(rhi::RHIbhandle& handle) override {

		}
		void* MapBuffer(rhi::RHIbhandle, rhi::RHIbflag) override {
			return nullptr;
		}
		void UnmapBuffer(rhi::RHIbhandle handle) override {

		}

		void CreateVertexLayout() override {


		}
		void DeleteVertexLayout() override {
		
		
		}

		void Draw() override {
		}


	protected:

		Window* window = nullptr;

		GLenum TranslateBufferDataFlags(rhi::RHIbflag flags) {
			GLenum flag = 0;

			if (flags & rhi::flags::Static && flags & rhi::flags::Map_Write) {
				LOG_ERROR("Invalid buffer data flags combination");
				return 0;
			}
			
			if (flags & rhi::flags::Dynamic)				flag |= GL_DYNAMIC_STORAGE_BIT;
			if (flags & rhi::flags::Map_Persistent)			flag |= GL_MAP_PERSISTENT_BIT;
			if (flags & rhi::flags::Map_Write)				flag |= GL_MAP_WRITE_BIT;
			if (flags & rhi::flags::Map_Read)				flag |= GL_MAP_READ_BIT;
			if (flags & rhi::flags::Map_Coherent)			flag |= GL_MAP_COHERENT_BIT;
			if (flags & rhi::flags::Map_Invalidate_Buffer)	flag |= GL_MAP_INVALIDATE_BUFFER_BIT;
			if (flags & rhi::flags::Map_Invalidate_Range)	flag |= GL_MAP_INVALIDATE_RANGE_BIT;
			if (flags & rhi::flags::Map_Unsynchronized)		flag |= GL_MAP_UNSYNCHRONIZED_BIT;

			return flag;
		}
		GLenum TranslateMappingFlags(rhi::RHIbflag flags){
			GLenum flag = 0;

			if (flags & rhi::flags::Static && flags & rhi::flags::Dynamic) {
				LOG_ERROR("Invalid buffer mapping flags combination");
				return 0;
			}

			if (flags & rhi::flags::Map_Persistent)			flag |= GL_MAP_PERSISTENT_BIT;
			if (flags & rhi::flags::Map_Write)				flag |= GL_MAP_WRITE_BIT;
			if (flags & rhi::flags::Map_Read)				flag |= GL_MAP_READ_BIT;
			if (flags & rhi::flags::Map_Coherent)			flag |= GL_MAP_COHERENT_BIT;
			if (flags & rhi::flags::Map_Invalidate_Buffer)	flag |= GL_MAP_INVALIDATE_BUFFER_BIT;
			if (flags & rhi::flags::Map_Invalidate_Range)	flag |= GL_MAP_INVALIDATE_RANGE_BIT;
			if (flags & rhi::flags::Map_Unsynchronized)		flag |= GL_MAP_UNSYNCHRONIZED_BIT;

			return flag;
		}

	};

}