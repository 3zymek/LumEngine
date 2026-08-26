//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Parser for .lmt material format files.
//
//=============================================================================//

#include "Scene/Format/MaterialDeserializer.hpp"
#include "Scene/Format/Tokenizer.hpp"

namespace lum::fmt {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void MaterialDeserializer::Deserialize( MaterialDescriptor& material, Tokenizer& tokenizer ) {

		auto& tokens = tokenizer.GetTokens();

		for (int32 i = 0; i < tokens.size(); i++) {

			if (tokens[i].m_Type == TokenType::Parameter) {
				
				if (tokens[i].m_Value == "albedo") {
					
					String path = detail::ReadStringParameter(tokens, i);
					
					material.m_AlbedoTex = path;
					
				}
				else if (tokens[i].m_Value == "normal") {
					
					String path = detail::ReadStringParameter(tokens, i);

					material.m_NormalTex = path;

				}
				else if (tokens[i].m_Value == "roughness") {
		
					String path = detail::ReadStringParameter(tokens, i);

					material.m_RoughnessTex = path;

				}
				else if (tokens[i].m_Value == "metallic") {
					
					String path = detail::ReadStringParameter(tokens, i);

					material.m_MetallicTex = path;

					
				}

			}
			
		}

	}

} // namespace lum::fmt