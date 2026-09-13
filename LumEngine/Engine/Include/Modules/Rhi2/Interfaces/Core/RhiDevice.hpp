#pragma once

#include "Rhi2/RhiCommon.hpp"
#include "Rhi2/RhiAdapter.hpp"

namespace lum {

	class ISurfaceProvider;

} // namespace lum

namespace lum::rhi {

	struct RenderDeviceCreateInfo {

		std::vector<const char*>	m_InstanceExtensions{};
		void*						m_NativeWindowHandle = nullptr;
		bool						m_EnableValidation = false;
		AdapterRequirements			m_AdapterRequirements{};
		SafePtr<ISurfaceProvider>	m_SurfaceProvider = nullptr;

	};

	class IRenderDevice {
	public:

		virtual void Initialize( const RenderDeviceCreateInfo& info ) noexcept;
		virtual void Finalize( ) noexcept;

		virtual ~IRenderDevice( ) = default;

	};

} // namespace lum::rhi