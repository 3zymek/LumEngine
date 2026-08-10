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

			if (tokens[i].mType == TokenType::Parameter) {
				
				if (tokens[i].mValue == "albedo") {
					
					String path = detail::ReadStringParameter(tokens, i);
					
					material.mAlbedoTex = path;
					
				}
				else if (tokens[i].mValue == "normal") {
					
					String path = detail::ReadStringParameter(tokens, i);

					material.mNormalTex = path;

				}
				else if (tokens[i].mValue == "roughness") {
		
					String path = detail::ReadStringParameter(tokens, i);

					material.mRoughnessTex = path;

				}
				else if (tokens[i].mValue == "metallic") {
					
					String path = detail::ReadStringParameter(tokens, i);

					material.mMetallicTex = path;

					
				}

			}
			
		}

	}

} // namespace lum::fmt