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

#include <vector>

#include "Core/CoreTypes.hpp"
#include "Core/Utils/HandlePool.hpp"
#include "Rhi/Interfaces/Core/RhiShader.hpp"

namespace lum::rhi {

	using ResourceSetID = uint32;
	using ResourceLayoutID = uint32;

	struct ResourceSetHandle : BaseHandle<ResourceSetID> {};
	struct ResourceLayoutHandle : BaseHandle<ResourceLayoutID> {};

	enum class ResourceType : uint8 {
		Sampler = 0U,
		CombinedImageSampler,
		SampledImage,
		StorageImage,
		UniformBuffer,
		StorageBuffer
	};

	struct ResourceSetBinding {

		uint32 m_Binding{};
		uint32 m_ResourceCount{};
		ShaderStageFlags m_ShaderStages{};
		ResourceType m_Type{};

	};

	struct ResourceLayoutCreateInfo {

		std::vector<ResourceSetBinding> m_Bindings{};
		ResourceLayoutHandle m_Layout{};

	};

	struct ResourceSetCreateInfo {

		ResourceLayoutHandle m_Layout{};

	};


} // namespace lum::rhi