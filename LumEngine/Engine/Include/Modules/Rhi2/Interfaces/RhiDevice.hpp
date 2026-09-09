#pragma once


namespace lum::rhi {

	class IRenderDevice {
	public:

		virtual void Initialize( );
		virtual void Finalize( );

		virtual ~IRenderDevice( ) = default;

	private:


	};

} // namespace lum::rhi