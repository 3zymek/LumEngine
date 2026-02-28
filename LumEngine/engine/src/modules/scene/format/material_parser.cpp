//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Parser for .lmt material format files.
//
//=============================================================================//

#include "scene/format/material_parser.hpp"
#include "scene/format/tokenizer.hpp"
#include "entity/components/material.hpp"

namespace lum::fmt {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void MaterialParser::Parse( FMaterialDescriptor& material ) {

		auto& tokens = mTokenizer.GetTokens();

		for (int32 i = 0; i < tokens.size(); i++) {

			if (tokens[i].mType == ETokenType::Parameter) {
				
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
				else if (tokens[i].mValue == "base_color") {
					
					glm::vec3 value = detail::ReadVec3Parameter(tokens, i);

					material.mBaseColor = value;

				}
				else if (tokens[i].mValue == "roughness_value") {

					float32 value = detail::ReadFloatParameter(tokens, i);

					material.mRoughnessValue = value;
					
				}
				else if (tokens[i].mValue == "metallic_value") {
					
					float32 value = detail::ReadFloatParameter(tokens, i);

					material.mMetallicValue = value;

				}

			}
			
		}

	}

} // namespace lum::fmt