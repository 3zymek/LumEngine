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
				resized.mWidth = static_cast<uint32>(mPendingSize.width( ) * dpr);
				resized.mHeight = static_cast<uint32>(mPendingSize.height( ) * dpr);
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
		const uint32 width = texture.Value( ).mRect.mWidth;
		const uint32 height = texture.Value( ).mRect.mHeight;

		if (width <= 0 || height <= 0) return;

		if (mInfo.mRenderContext) {
			mInfo.mRenderContext( ).MakeCurrent( );
		}

		std::vector<uint8> pixels( width * height * 4 );
		mInfo.mRenderDevice( ).GetTextureImage( mTextureId, pixels.data( ) );

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
		QWidget::mousePressEvent( event );
	}

	void ViewportWidget::mouseReleaseEvent( QMouseEvent* event ) {
		QWidget::mouseReleaseEvent( event );
	}

	void ViewportWidget::mouseMoveEvent( QMouseEvent* event ) {
		QWidget::mouseMoveEvent( event );
	}

} // namespace lum::editor