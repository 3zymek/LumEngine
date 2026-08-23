//========= Copyright (C) 2025-present 3zymek, MIT License  ============//
//
// Purpose: Creator interface for Qt-backed rendering contexts.
//
//=============================================================================//
#pragma once

#include "Platform/RenderContext.hpp"

namespace lum::editor {

	/* @brief Factory class for creating Qt-based rendering contexts. */
	class QtContextCreator {
	public:

		enum class Api {
			OpenGL,
			/* Vulkan */
		};

		/*
		* @brief Creates a rendering context for the specified graphics API.
		* @param api Target graphics API.
		* @return Unique pointer to the created RenderContext instance.
		*/
		static std::unique_ptr<RenderContext> Create( Api api );

	};

} // namespace lum::editor