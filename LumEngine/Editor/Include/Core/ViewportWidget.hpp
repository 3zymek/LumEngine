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

	class ViewportWidget : public QWidget {

		Q_OBJECT

	public:

		explicit ViewportWidget( QWidget* parent = nullptr ) : QWidget( parent ) { }
		~ViewportWidget( ) override = default;

		void Initialize( const ViewportCreateInfo& info );
		LUM_FORCEINLINE void SetTexture( rhi::TextureHandle tex ) {
			m_TextureId = tex;
			update( );
		}

	protected:
		
		void paintEvent( QPaintEvent* event ) override;
		void resizeEvent( QResizeEvent* event ) override;
		void mousePressEvent( QMouseEvent* event ) override;
		void mouseReleaseEvent( QMouseEvent* event ) override;
		void mouseMoveEvent( QMouseEvent* event ) override;
		void keyPressEvent( QKeyEvent* event ) override;

	private:

		EditorCamera m_Camera{};

		QTimer m_ResizeTimer{};
		QSize m_PendingSize{};
		const uint32 m_ResizeFreshrate = 50; // in ms
		bool m_ControlsUnlocked = false;

		ViewportCreateInfo m_Ctx{};
		rhi::TextureHandle m_TextureId{};

	};


}