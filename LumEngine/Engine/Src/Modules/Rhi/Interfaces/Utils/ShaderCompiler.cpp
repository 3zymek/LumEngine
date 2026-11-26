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

namespace lum::rhi {

	void ShaderCompiler::Initialize( ) {

		auto result = slang::createGlobalSession( m_Session.writeRef( ) );
		LUM_ASSERT( SLANG_SUCCEEDED( result ), "Failed to initialize SlangC" );

	}

} // namespace lum::rhi