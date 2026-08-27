#include "Core/ViewportWidget.hpp"
#include "Rhi/Core/RhiDevice.hpp"
#include "Event/Events/WindowEvents.hpp"
#include "Event/EventBus.hpp"
#include <QPaintEvent>
#include <QPainter>
#include <QImage>

namespace lum::editor {

	void ViewportWidget::Initialize( const ViewportCreateInfo& info ) {
		mInfo = info;

		connect(
			&mResizeTimer,
			&QTimer::timeout,
			this,
			[ this ]( ) {

				if (mPendingSize.width( ) <= 0 || mPendingSize.height( ) <= 0) return;

				const qreal dpr = devicePixelRatioF( );

				EWindowResized resized{};
				resized.m_Width = SafeCast<uint32>(mPendingSize.width( ) * dpr);
				resized.m_Height = SafeCast<uint32>(mPendingSize.height( ) * dpr);
				mInfo.mEventBus( ).Emit( resized );

			}
		);

	}

	void ViewportWidget::paintEvent( QPaintEvent* event ) {
		Q_UNUSED( event );

		QPainter painter( this );

		auto texture = mInfo.mRenderDevice( ).Get( mTextureId );

		if (!texture) {
			painter.fillRect( rect( ), Qt::red );
			return;
		}

		const qreal dpr = devicePixelRatio( );
		const uint32 width = texture.Value( ).m_Rect.m_Width;
		const uint32 height = texture.Value( ).m_Rect.m_Height;

		if (width <= 0 || height <= 0) return;

		if (mInfo.mRenderContext) {
			mInfo.mRenderContext( ).MakeCurrent( );
		}

		std::vector<uint8> pixels( width * height * 4 );
		mInfo.mRenderDevice( ).GetTextureImage( mTextureId, pixels.data( ) );

		if (pixels.empty( )) {
			LUM_LOG_ERROR( "Pixels buffer is empty!" );
		}
		else {
			uint8 r = pixels[ 0 ];
			uint8 g = pixels[ 1 ];
			uint8 b = pixels[ 2 ];
			uint8 a = pixels[ 3 ];

			size_t midIdx = (width * height / 2) * 4;
			uint8 midR = pixels[ midIdx ];
			uint8 midG = pixels[ midIdx + 1 ];
			uint8 midB = pixels[ midIdx + 2 ];
			uint8 midA = pixels[ midIdx + 3 ];

			LUM_LOG_INFO( "Texture Debug [{}x{}]: First Pixel RGBA({}, {}, {}, {}), Mid Pixel RGBA({}, {}, {}, {})",
				width, height, r, g, b, a, midR, midG, midB, midA );
		}

		QImage frameImage( pixels.data( ), width, height, QImage::Format_RGBX8888 );
		QImage flippedImage = frameImage.mirrored( false, true );

		frameImage.setDevicePixelRatio( dpr );
		painter.drawImage( rect( ), flippedImage );

	}

	void ViewportWidget::resizeEvent( QResizeEvent* event ) {

		QWidget::resizeEvent( event );
		mPendingSize = event->size( );
		mResizeTimer.start( mResizeFreshrate );

	}

	void ViewportWidget::mousePressEvent( QMouseEvent* event ) {

		if (event->button( ) == Qt::MouseButton::RightButton)
			mControlsUnlocked = true;

		QWidget::mousePressEvent( event );

	}

	void ViewportWidget::mouseReleaseEvent( QMouseEvent* event ) {

		if (event->button( ) == Qt::MouseButton::RightButton)
			mControlsUnlocked = false;

		QWidget::mouseReleaseEvent( event );

	}

	void ViewportWidget::mouseMoveEvent( QMouseEvent* event ) {
		QWidget::mouseMoveEvent( event );
	}

} // namespace lum::editor