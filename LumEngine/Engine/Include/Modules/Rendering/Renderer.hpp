/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#pragma once
#include "Core/CoreDefines.hpp"
#include "Rendering/RenderingTypes.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"

namespace lum {

	class LUM_API Renderer {
	public:

		Renderer( ) = default;

		void Initialize( ) noexcept;
		const rhi::ImageHandle GetFrameTexture( ) const noexcept;
		void SubmitInstance( const RenderInstance& instance ) noexcept;

		void BeginFrame( );
		void EndFrame( );

	private:



	};

} // namespace lum