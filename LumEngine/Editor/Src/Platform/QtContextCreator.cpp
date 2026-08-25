//========= Copyright (C) 2025-present 3zymek, MIT License  ============//
//
// Purpose: Creator interface for Qt-backed rendering contexts.
//
//=============================================================================//

#include "Platform/QtContextCreator.hpp"
#include "Platform/QtOpenGLContext.hpp"

namespace lum::editor {

	std::unique_ptr<IRenderContext> QtContextCreator::Create( Api api ) {
		switch (api) {
			case Api::OpenGL:
				return std::move( std::make_unique<QtOpenGLContext>( ) );
			default:
				return nullptr;
		}
	}

}