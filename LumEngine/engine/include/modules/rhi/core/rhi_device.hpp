#pragma once
#include "core/core_pch.hpp"
#include "core/utils/handle_pool.hpp"
namespace lum::rhi {

	struct RHI_Buffer;
	struct RHI_BufferDescriptor;

	class RHI_Device {
	public:

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		// VBO EBO UBO
		virtual RHIBHandle CreateBuffer(const RHI_BufferDescriptor&) = 0;
		virtual void UpdateBuffer(RHI_BufferHandle) = 0;
		virtual void DeleteBuffer() = 0;
		virtual void MapBuffer() = 0;
		virtual void UnmapBuffer() = 0;

		// VAO
		virtual void CreateVertexLayout() = 0;
		virtual void DeleteVertexLayout() = 0;


		virtual void Draw() = 0;

	protected:

		static constexpr unsigned int MAX_BUFFERS = 10000;
		static constexpr unsigned int MAX_LAYOUTS = 10000;

		cstd::handle_pool<RHI_Buffer, RHIBHandle> m_buffers{ MAX_BUFFERS };

	};


}