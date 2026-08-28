#include "Core/ViewportWidget.hpp"
#include "Rhi/Core/RhiDevice.hpp"
#include "Event/Events/WindowEvents.hpp"
#include "Event/EventBus.hpp"
#include "Engine.hpp"
#include <QPaintEvent>
#include <QPainter>
#include <QImage>

namespace lum::editor {

	void ViewportWidget::Initialize( const ViewportCreateInfo& info ) {
		m_Ctx = info;

		connect(
			&m_ResizeTimer,
			&QTimer::timeout,
			this,
			[ this ]( ) {

				if (m_PendingSize.width( ) <= 0 || m_PendingSize.height( ) <= 0) return;

				const qreal dpr = devicePixelRatioF( );

				EWindowResized resized{};
				resized.m_Width = SafeCast<uint32>( m_PendingSize.width( ) * dpr );
				resized.m_Height = SafeCast<uint32>( m_PendingSize.height( ) * dpr );
				m_Ctx.m_EventBus( ).Emit( resized );

			}
		);

	}

	void ViewportWidget::paintEvent( QPaintEvent* event ) {

		Q_UNUSED( event );

		m_Ctx.m_Engine().Render().m_Renderer.UpdateCamera( m_Camera.Update( m_Ctx.m_Engine( ).GetDeltaTime( ), m_ControlsUnlocked ) );

		QPainter painter( this );

		auto texture = m_Ctx.m_RenderDevice( ).Get( m_TextureId );

		if (!texture) {
			painter.fillRect( rect( ), Qt::red );
			return;
		}

		const qreal dpr = devicePixelRatio( );
		const uint32 width = texture.Value( ).m_Rect.m_Width;
		const uint32 height = texture.Value( ).m_Rect.m_Height;

		if (width <= 0 || height <= 0) return;

		if (m_Ctx.m_RenderContext) {
			m_Ctx.m_RenderContext( ).MakeCurrent( );
		}

		std::vector<uint8> pixels( width * height * 4 );
		m_Ctx.m_RenderDevice( ).GetTextureImage( m_TextureId, pixels.data( ) );

		QImage frameImage( pixels.data( ), width, height, QImage::Format_RGBX8888 );
		QImage flippedImage = frameImage.mirrored( false, true );

		frameImage.setDevicePixelRatio( dpr );
		painter.drawImage( rect( ), flippedImage );

	}

	void ViewportWidget::resizeEvent( QResizeEvent* event ) {

		QWidget::resizeEvent( event );
		m_PendingSize = event->size( );
		m_ResizeTimer.start( m_ResizeFreshrate );

	}

	void ViewportWidget::mousePressEvent( QMouseEvent* event ) {

		if (event->button( ) == Qt::MouseButton::RightButton)
			m_ControlsUnlocked = true;

		QWidget::mousePressEvent( event );

	}

	void ViewportWidget::mouseReleaseEvent( QMouseEvent* event ) {

		if (event->button( ) == Qt::MouseButton::RightButton)
			m_ControlsUnlocked = false;

		QWidget::mouseReleaseEvent( event );

	}

	void ViewportWidget::mouseMoveEvent( QMouseEvent* event ) {
		QWidget::mouseMoveEvent( event );
	}

	void ViewportWidget::keyPressEvent( QKeyEvent* event ) {

		if (!m_ControlsUnlocked) return;

		switch (event->key( )) {
			case Qt::Key_W:
			{
				m_Camera.Move( EditorCamera::Direction::Forward );
				break;
			}
			case Qt::Key_S:
			{
				m_Camera.Move( EditorCamera::Direction::Backward );
				break;
			}
			case Qt::Key_D:
			{
				m_Camera.Move( EditorCamera::Direction::Right );
				break;
			}
			case Qt::Key_A:
			{
				m_Camera.Move( EditorCamera::Direction::Left );
				break;
			}
			default: break;
		}

	}

} // namespace lum::editor