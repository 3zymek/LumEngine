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
#include "Core/CorePch.hpp"

// Header impl file
#include "Rhi/Interfaces/Utils/ShaderCompiler.hpp"

#include "Core/Utils/Assert.hpp"
#include "Platform/FileSystem/FileSystem.hpp"

namespace lum::rhi {

	void ShaderCompiler::Initialize() {

		auto result = slang::createGlobalSession(m_Session.writeRef());
		LUM_ASSERT(SLANG_SUCCEEDED(result), "Failed to initialize SlangC");

	}

	auto ShaderCompiler::CreateCompileContext(const CompileContextCreateInfo& info) -> Result<CompileContext> {

		LUM_ASSERT(
			info.m_TargetFormat != ShaderTarget::None,
			"CreateCompileContext: Target format is set to None which is invalid"
		);

		LUM_ASSERT(
			m_Session != nullptr,
			"CreateCompileContext: ShaderCompiler not initialized."
		);

		slang::TargetDesc targetDesc{};
		if (info.m_TargetFormat == ShaderTarget::DXIL) {
			targetDesc.format = SLANG_SPIRV;
			targetDesc.profile = m_Session->findProfile("spirv_1_5");
		}
		else if (info.m_TargetFormat == ShaderTarget::SPIRV) {
			targetDesc.format = SLANG_DXIL;
			targetDesc.profile = m_Session->findProfile("sm_6_0");
		}

		slang::SessionDesc desc{};
		desc.targets = &targetDesc;
		desc.targetCount = 1;

		CompileContext ctx{};
		auto result = m_Session->createSession(desc, ctx.m_Session.writeRef());

		if (SLANG_FAILED(result) || !ctx.m_Session) {
			return Result<CompileContext>::Failure("Couldn't create SlangC session.");
		}

		return ctx;

	}
	auto ShaderCompiler::CompileShader(const CompileShaderInfo& info) -> std::vector<CompiledShader> {

		LUM_ASSERT(
			info.m_Context.m_Session != nullptr,
			"CompileShader: Invalid ShaderCompileContext"
		);
		auto& session = info.m_Context.m_Session;

		LUM_ASSERT(
			m_Session != nullptr,
			"CompileShader: ShaderCompiler not initialized"
		);

		Slang::ComPtr<slang::IBlob> diagnostics{};

		auto content = read_shader_file(info.m_SourceFile);

		//slang::IModule* module = session->loadModuleFromSourceString(

		//)

	}

	Result<String> ShaderCompiler::read_shader_file(const Path& path) {

		if (!FileSystem::Exists(path)) {
			return Result<String>::Failure("Shader file doesn't exists");
		}

		return FileSystem::ReadAllText(path);

	}

} // namespace lum::rhi