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
		Lit,
		Depth
	};

	class DeferredBuffer {
	public:

		void Initialize(rhi::IRenderDevice& device);
		void RefreshResources();
		rhi::ImageHandle GetAttachment(DeferredAttachment attachment) const noexcept;

	private:

		SafePtr<rhi::IRenderDevice> m_RenderDevice = nullptr;
		rhi::ImageHandle m_AlbedoAttachment{};
		rhi::ImageHandle m_DepthAttachment{};
		rhi::ImageHandle m_LitAttachment{};

		void create_assets();

	};


} // namespace lum
