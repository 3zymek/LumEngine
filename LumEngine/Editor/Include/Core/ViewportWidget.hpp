#pragma once

#include "EditorPch.hpp"
#include "Rhi/RhiCommon.hpp"
#include "Core/EditorCamera.hpp"

namespace lum {

	namespace rhi { class IRenderDevice; }
	namespace ev { class EventBus; }
	class IRenderContext;
	class Engine;

}

namespace lum::editor {
	
	struct ViewportCreateInfo {
		SafePtr<rhi::IRenderDevice> m_RenderDevice = nullptr;
		SafePtr<IRenderContext> m_RenderContext = nullptr;
		SafePtr<ev::EventBus> m_EventBus = nullptr;
		SafePtr<Engine> m_Engine = nullptr;
	};

	/*
	class ViewportWindow : public QWindow {
		
		Q_OBJECT

	public:
		
		ViewportWindow( ) {
			setSurfaceType( QSurface::RasterSurface );
		}
		~ViewportWindow( ) override = default;

	protected:

		bool event( QEvent* event ) override {
			if (event->type( ) == QEvent::UpdateRequest) {
				return true;
			}
			return QWindow::event( event );
		}

	};

	class ViewportWidget : public QWidget {

		Q_OBJECT

	public:

		explicit ViewportWidget( QWidget* parent = nullptr );
		~ViewportWidget( ) override = default;

		void Initialize( const ViewportCreateInfo& info );

		LUM_NODISCARD void* GetNativeHandle( ) const;

	protected:

		void resizeEvent( QResizeEvent* event ) override;
		void mousePressEvent( QMouseEvent* event ) override;
		void mouseReleaseEvent( QMouseEvent* event ) override;
		void mouseMoveEvent( QMouseEvent* event ) override;
		void keyPressEvent( QKeyEvent* event ) override;

	private:

		ViewportWindow* m_Window = nullptr;
		QWidget* m_Container = nullptr; //

		EditorCamera m_Camera{};

		QTimer m_ResizeTimer{};
		QSize m_PendingSize{};
		const uint32 m_ResizeFreshrate = 1; // in ms
		bool m_ControlsUnlocked = false;

		ViewportCreateInfo m_Ctx{};
		rhi::TextureHandle m_FrameTextureId{};

	};*/

} // namespace lum::editor