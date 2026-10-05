#pragma once

#include "Rhi2/RhiCommon.hpp"
#include "Rhi2/RhiAdapter.hpp"
#include "Rhi2/Interfaces/Core/RhiBuffer.hpp"

namespace lum {

	class ISurfaceProvider;

	namespace ev {
		class EventBus;
	} // namespace ev

} // namespace lum

namespace lum::rhi {

	struct RenderDeviceCreateInfo {

		std::vector<const char*>	m_InstanceExtensions{};
		void*						m_NativeWindowHandle = nullptr;
		bool						m_EnableValidation = false;
		AdapterRequirements			m_AdapterRequirements{};
		SafePtr<ISurfaceProvider>	m_SurfaceProvider = nullptr; // REQUIRED
		SafePtr<ev::EventBus>		m_EventBus = nullptr; // REQUIRED

	};

	class IRenderDevice {
	public:

		virtual void Initialize( const RenderDeviceCreateInfo& info ) noexcept = 0;
		virtual void Finalize( ) noexcept = 0;

		virtual void UpdateFrame( ) noexcept = 0;

		virtual BufferHandle2 CreateBuffer( const BufferCreateInfo2& info ) noexcept = 0;
		virtual void DestroyBuffer( BufferHandle2& handle ) noexcept = 0;

	virtual ~IRenderDevice( ) = default;

	};

} // namespace lum::rhi