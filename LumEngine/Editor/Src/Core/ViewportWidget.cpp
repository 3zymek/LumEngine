#include "Core/ViewportWidget.hpp"
#include "Rhi/Core/RhiDevice.hpp"
#include "Event/Events/WindowEvents.hpp"
#include "Event/EventBus.hpp"
#include "Engine.hpp"
#include <QPaintEvent>
#include <QPainter>
#include <QImage>

namespace lum::editor {

	/*
	ViewportWidget::ViewportWidget( QWidget* parent ) : QWidget( parent ) {

		m_Window = new ViewportWindow( );
		m_Container = QWidget::createWindowContainer( m_Window, this );

		auto* layout = new QVBoxLayout( );
		layout->setContentsMargins( 0, 0, 0, 0 );
		layout->addWidget( m_Container );
		setLayout( layout ); //

		m_Container->setFocusPolicy( Qt::StrongFocus ); //

	}

	void ViewportWidget::Initialize( const ViewportCreateInfo& info ) {
		
		m_Ctx = info;

		void* nativeHandle = GetNativeHandle( );

		if (m_Ctx.m_RenderDevice) {
			qreal pixelRatio = devicePixelRatioF( );
			const uint32 width = SafeCast<uint32>( this->width( ) * pixelRatio );
			const uint32 height = SafeCast<uint32>( this->height( ) * pixelRatio );
			//m_Ctx.m_RenderDevice().createSwa
		}

	}*/

} // namespace lum::editor