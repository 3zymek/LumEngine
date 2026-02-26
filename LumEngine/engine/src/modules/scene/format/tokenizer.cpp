#include "scene/format/tokenizer.hpp"

namespace lum::fmt {

	void Tokenizer::Tokenize(StringView str) {

		mTokens.clear();

		usize pos = 0;
		String result;
		while (pos < str.size()) {

			const char& c = str[pos];

			if (c == '/' && pos + 1 < str.size() && str[pos + 1] == '/') {
				while (pos < str.size() && str[pos] != '\n') {
					pos++;
				}
			}

			else if (c == '"') {

				++pos;

				String value;

				while (pos < str.size() && str[pos] != '"') {
					value += str[pos++];
				}

				mTokens.push_back({ ETokenType::String, value });

			}

			else if (c == '@') {

				++pos;

				String value;

				while (pos < str.size() && (isalpha(str[pos]) || str[pos] == '_')) {
					value += str[pos++];
				}

				mTokens.push_back({ ETokenType::Component, value });

			}

			else if (isalpha(c)) {

				String value;

				while (pos < str.size() && (isalpha(str[pos]) || str[pos] == '_'))
					value += str[pos++];

				bool isParameter = false;
				usize npos = pos;
				while (npos < str.size() && str[npos] != '\n') {
					if (str[npos] == ':') {
						isParameter = true;
						pos = npos;
						break;
					}
					npos++;
				}

				if (isParameter)
					mTokens.push_back({ ETokenType::Parameter, value });
				else
					mTokens.push_back({ ETokenType::Identifier, value });

				if (str[npos] == ':')
					mTokens.push_back({ ETokenType::Colon, ":" });

			}

			else if (isdigit(c) || c == '-') {

				String value;
				while (pos < str.size() && (isdigit(str[pos]) || (str[pos] == '.' || str[pos] == '-'))) {
					value += str[pos++];
				}
				if (pos < str.size() && isalpha(str[pos])) {
					LUM_LOG_ERROR("Invalid number token: %s", value.c_str());
				}
				else {
					mTokens.push_back({ ETokenType::Number, value });
				}

			}

			else if (c == '{') {

				++pos;

				mTokens.push_back({ ETokenType::LBracket, "{" });

			}

			else if (c == '}') {

				++pos;

				mTokens.push_back({ ETokenType::RBracket, "}" });

			}

			pos++;

		}

	}

} // namespace lum::fmt