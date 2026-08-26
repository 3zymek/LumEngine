//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Tokenizer for .lsc and .lmt format files.
//
//=============================================================================//

#include "Scene/Format/Tokenizer.hpp"

namespace lum::fmt {

	void Tokenizer::Tokenize( StringView content, const Path& filePath ) {

		m_Tokens.clear( );

		usize pos = 0;
		uint32 line = 1;

		while (pos < content.size( )) {

			const char c = content[ pos ];

			//=====================================================
			// Whitespace
			//=====================================================

			if (c == '\n') {
				++line;
				++pos;
				continue;
			}

			if (isspace( static_cast<unsigned char>(c) )) {
				++pos;
				continue;
			}

			//=====================================================
			// Comment
			//=====================================================

			if (c == '/' &&
				pos + 1 < content.size( ) &&
				content[ pos + 1 ] == '/') {

				pos += 2;

				while (pos < content.size( ) && content[ pos ] != '\n')
					++pos;

				continue;
			}

			//=====================================================
			// String
			//=====================================================

			if (c == '"') {

				++pos;

				String value;
				const uint32 tokenLine = line;

				while (pos < content.size( ) && content[ pos ] != '"') {

					if (content[ pos ] == '\n') {
						LUM_LOG_ERROR(
							"Unterminated string at line %llu in file %s",
							tokenLine,
							filePath.ToString( ).c_str( )
						);
						return;
					}

					value += content[ pos++ ];
				}

				if (pos >= content.size( )) {
					LUM_LOG_ERROR(
						"Unterminated string at line %llu in file %s",
						tokenLine,
						filePath.ToString( ).c_str( )
					);
					return;
				}

				++pos;

				m_Tokens.push_back(
					{ tokenLine, TokenType::String, value, filePath }
				);

				continue;
			}

			//=====================================================
			// Component
			//=====================================================

			if (c == '@') {

				++pos;

				String value;

				while (pos < content.size( ) &&
					(isalpha( static_cast<unsigned char>( content[ pos ] ) ) ||
					  content[ pos ] == '_')) {

					value += content[ pos++ ];
				}

				if (value.empty( )) {
					LUM_LOG_ERROR(
						"Component name expected at line %llu in file %s",
						line,
						filePath.ToString( ).c_str( )
					);
					return;
				}

				m_Tokens.push_back(
					{ line, TokenType::Component, value, filePath }
				);

				continue;
			}

			//=====================================================
			// Identifier / Parameter
			//=====================================================

			if (isalpha( static_cast<unsigned char>(c) )) {

				String value;

				while (pos < content.size( ) &&
					(isalpha( static_cast<unsigned char>( content[ pos ] ) ) ||
					  content[ pos ] == '_')) {

					value += content[ pos++ ];
				}

				if (pos < content.size( ) && content[ pos ] == ':') {

					m_Tokens.push_back(
						{ line, TokenType::Parameter, value, filePath }
					);

					m_Tokens.push_back(
						{ line, TokenType::Colon, ":", filePath }
					);

					++pos;

				}
				else {

					m_Tokens.push_back(
						{ line, TokenType::Identifier, value, filePath }
					);
				}

				continue;
			}

			//=====================================================
			// Number
			//=====================================================

			if (isdigit( static_cast<unsigned char>(c) ) || c == '-') {

				const usize start = pos;

				if (content[ pos ] == '-')
					++pos;

				bool hasDigits = false;
				bool hasDecimalPoint = false;

				while (pos < content.size( )) {

					const char numberChar = content[ pos ];

					if (isdigit( static_cast<unsigned char>( numberChar ) )) {
						hasDigits = true;
						++pos;
						continue;
					}

					if (numberChar == '.' && !hasDecimalPoint) {
						hasDecimalPoint = true;
						++pos;
						continue;
					}

					break;
				}

				if (!hasDigits) {

					LUM_LOG_ERROR(
						"Invalid number at line %llu in file %s",
						line,
						filePath.ToString( ).c_str( )
					);

					return;
				}

				if (pos < content.size( ) &&
					(isalpha( static_cast<unsigned char>( content[ pos ] ) ) ||
					  content[ pos ] == '.')) {

					LUM_LOG_ERROR(
						"Invalid number token '%s' at line %llu in file %s",
						String( content.substr( start, pos - start ) ).c_str( ),
						line,
						filePath.ToString( ).c_str( )
					);

					return;
				}

				m_Tokens.push_back(
					{
						line,
						TokenType::Number,
						String( content.substr( start, pos - start ) ),
						filePath
					}
				);

				continue;
			}

			//=====================================================
			// Single-character tokens
			//=====================================================

			switch (c) {

				case '{':
				m_Tokens.push_back(
					{ line, TokenType::LBracket, "{", filePath }
				);
				break;

				case '}':
				m_Tokens.push_back(
					{ line, TokenType::RBracket, "}", filePath }
				);
				break;

				case '[':
				m_Tokens.push_back(
					{ line, TokenType::LSquareBracket, "[", filePath }
				);
				break;

				case ']':
				m_Tokens.push_back(
					{ line, TokenType::RSquareBracket, "]", filePath }
				);
				break;

				case ':':
				m_Tokens.push_back(
					{ line, TokenType::Colon, ":", filePath }
				);
				break;

				case ',':
				// Commas are ignored.
				++pos;
				continue;

				default:

				LUM_LOG_ERROR(
					"Unknown character '%c' at line %llu in file %s",
					c,
					line,
					filePath.ToString( ).c_str( )
				);

				return;
			}

			++pos;
		}
	}

} // namespace lum::fmt