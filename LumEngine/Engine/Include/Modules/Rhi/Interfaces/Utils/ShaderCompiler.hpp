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
#include <slang/slang.h>
#include <slang/slang-com-ptr.h>

#include "Core/CoreTypes.hpp"

namespace lum::rhi {

	struct CompiledShader {
		std::vector<uint8> m_Bytecode{};
	};

	enum class ShaderTarget : uint8 {
		SPIRV = 0U,
		DXIL
	};

	struct CompileContextCreateInfo {

		ShaderTarget m_TargetFormat{};

	};

	class ShaderCompiler {
	public:

		void Initialize( );

	private:

		Slang::ComPtr<slang::IGlobalSession> m_Session{};

	};

} // namespace lum::rhi
