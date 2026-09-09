#pragma once

#include "Rhi2/RhiCommon.hpp"

namespace lum::rhi {

	class IRenderDevice {
	public:

		virtual void Initialize( ) noexcept;
		virtual void Finalize( ) noexcept;

		virtual ~IRenderDevice( ) = default;

	private:


	};

} // namespace lum::rhi