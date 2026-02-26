#pragma once

#include "core/core_common.hpp"

namespace lum {

	namespace fmt {
		
		enum class ETokenType : byte {
			
			Identifier,
			LBracket,
			RBracket,
			Parameter,
			Component,
			Colon,
			Dollar,
			String,
			Number,
			EndOfLine
			
		};

		enum class EFormat : byte {
			Scene,
			Material
		};
		
		struct FToken {

			ETokenType mType;
			String mValue;

		};

		inline bool IsValidFormat( StringView str, EFormat format ) {
		
			usize dot = str.find_last_of('.');

			if (dot == StringView::npos)
				return false;

			StringView result = str.substr(dot + 1);
			if (result == "lsc" && format == EFormat::Scene)
				return true;

			if (result == "lmt" && format == EFormat::Material)
				return true;

			return false;
		}

		namespace detail {

			inline void ExpectOpeningBracket(std::vector<FToken>& tokens, int32& i) {
				++i;
				LUM_ASSERT(tokens[i].mType == ETokenType::LBracket, "Opening bracket expected");
				++i;
			}

			inline void ExpectColon(std::vector<FToken>& tokens, int32& i) {
				++i;
				LUM_ASSERT(tokens[i].mType == ETokenType::Colon, "Colon expected");
				++i;
			}

			inline String ReadStringParameter(std::vector<FToken>& tokens, int32& i) {

				ExpectColon(tokens, i);

				String value = tokens[i].mValue;

				LUM_ASSERT(i + 1 >= tokens.size() || tokens[i + 1].mType != ETokenType::String, "String expected");

				return value;

			}

			inline bool ReadBoolParameter(std::vector<FToken>& tokens, int32& i) {

				ExpectColon(tokens, i);

				bool value = std::stof(tokens[i].mValue) > 0;

				LUM_ASSERT(i + 1 >= tokens.size() || tokens[i + 1].mType != ETokenType::Number, "Boolean expected");

				return value;

			}

			inline float32 ReadFloatParameter(std::vector<FToken>& tokens, int32& i) {

				ExpectColon(tokens, i);

				float32 value = std::stof(tokens[i].mValue);

				LUM_ASSERT(i + 1 >= tokens.size() || tokens[i + 1].mType != ETokenType::Number, "Float expected");

				return value;

			}

			inline glm::vec3 ReadVec3Parameter(std::vector<FToken>& tokens, int32& i) {

				ExpectColon(tokens, i);

				float32 x = std::stof(tokens[i++].mValue);
				float32 y = std::stof(tokens[i++].mValue);
				float32 z = std::stof(tokens[i].mValue);

				LUM_ASSERT(i + 1 >= tokens.size() || tokens[i + 1].mType != ETokenType::Number, "Vec3 expected");

				return glm::vec3(x, y, z);

			}
			inline glm::vec2 ReadVec2Parameter(std::vector<FToken>& tokens, int32& i) {

				LUM_ASSERT(tokens[i].mType == ETokenType::Colon, "Colon expected");
				++i;

				float32 x = std::stof(tokens[i++].mValue);
				float32 y = std::stof(tokens[i].mValue);

				LUM_ASSERT(tokens[i + 1].mType != ETokenType::Number, "Vec2 expected");

				return glm::vec2(x, y);

			}


		} // namespace lum::fmt::detail
		
	} // namespace lum::fmt

} // namespace lum