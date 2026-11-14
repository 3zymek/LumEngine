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

#include "Core/CoreTypes.hpp"
#include "Core/Utils/SafePtr.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rendering/RenderingTypes.hpp"

namespace lum {

	enum class DeferredAttachment {
		Albedo,
		Normal,
		Depth
	};

	class DeferredBuffer {
	public:

		void Initialize( RendererContext& ctx, uint32 width, uint32 height );

		rhi::ImageHandle GetAttachment( DeferredAttachment attachment ) const noexcept;

	private:

		SafePtr<RendererContext> m_Ctx = nullptr;
		rhi::ImageHandle m_AlbedoAttachment{};

		void create_assets( uint32 width, uint32 height );


	};


} // namespace lum