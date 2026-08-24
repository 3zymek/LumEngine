#pragma once

#include "EditorPch.hpp"
#include "Rhi/RhiCommon.hpp"

namespace lum {

	namespace rhi { class RenderDevice; }
	namespace ev { class EventBus; }
	class RenderContext;

}

namespace lum::editor {
	
	struct ViewportCreateInfo {
		SafePtr<rhi::RenderDevice> mRenderDevice = nullptr;
		SafePtr<RenderContext> mRenderContext = nullptr;
		SafePtr<ev::EventBus> mEventBus = nullptr;
	};

	class ViewportWidget : public QWidget {

		Q_OBJECT

	public:

		explicit ViewportWidget( QWidget* parent = nullptr ) : QWidget( parent ) { }
		~ViewportWidget( ) override = default;

		void Initialize( const ViewportCreateInfo& info );
		LUM_FORCEINLINE void SetTexture( rhi::TextureHandle tex ) {
			mTextureId = tex;
			update( );
		}

	protected:

		void paintEvent( QPaintEvent* event ) override;
		void resizeEvent( QResizeEvent* event ) override;
		void mousePressEvent( QMouseEvent* event ) override;
		void mouseReleaseEvent( QMouseEvent* event ) override;
		void mouseMoveEvent( QMouseEvent* event ) override;

	private:

		QTimer mResizeTimer{};
		QSize mPendingSize{};
		const uint32 mResizeFreshrate = 50; // in ms

		ViewportCreateInfo mInfo{};
		rhi::TextureHandle mTextureId{};

	};


}