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
#include "Core/Utils/Result.hpp"
#include "Platform/FileSystem/Path.hpp"

namespace lum::rhi {

	struct CompiledShader {
		std::vector<uint8> m_Bytecode{};
	};

	struct CompileContext {
		friend class ShaderCompiler;
	private:
		Slang::ComPtr<slang::ISession> m_Session{};
	};

	enum class ShaderTarget : uint8 {
		None = 0U,
		SPIRV,
		DXIL
	};

	struct CompileContextCreateInfo {

		ShaderTarget m_TargetFormat = ShaderTarget::None;

	};

	struct CompileShaderInfo {

		CompileContext m_Context{};
		Path m_SourceFile{};

	};

	class ShaderCompiler {
	public:

		void Initialize();
		auto CreateCompileContext(const CompileContextCreateInfo& info) -> Result<CompileContext>;
		auto CompileShader(const CompileShaderInfo& info) -> std::vector<CompiledShader>;

	private:

		Slang::ComPtr<slang::IGlobalSession> m_Session{};

		Result<String> read_shader_file(const Path& path);

	};

} // namespace lum::rhi
