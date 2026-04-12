//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Tokenizer for .lsc and .lmt format files.
//
//=============================================================================//

#include "scene/format/tokenizer.hpp"

namespace lum::fmt {

	void Tokenizer::Tokenize( StringView str ) {

		mTokens.clear( );

		usize pos = 0;

		while (pos < str.size( )) {

			const char& c = str[ pos ];

			// Comment
			if (c == '/' && pos + 1 < str.size( ) && str[ pos + 1 ] == '/') {
				while (pos < str.size( ) && str[ pos ] != '\n') {
					pos++;
				}
			}

			// String
			else if (c == '"') {

				++pos;

				String value;

				while (pos < str.size( ) && str[ pos ] != '"') {
					value += str[ pos++ ];
				}

				mTokens.push_back( { TokenType::String, value } );

			}

			// Component
			else if (c == '@') {

				++pos;

				String value;

				while (pos < str.size( ) && (isalpha( str[ pos ] ) || str[ pos ] == '_')) {
					value += str[ pos++ ];
				}

				mTokens.push_back( { TokenType::Component, value } );

			}

			// Identifier / Parameter
			else if (isalpha( c )) {

				String value;

				while (pos < str.size( ) && (isalpha( str[ pos ] ) || str[ pos ] == '_'))
					value += str[ pos++ ];

				bool isParameter = false;
				usize npos = pos;
				while (npos < str.size( ) && str[ npos ] != '\n') {
					if (str[ npos ] == ':') {
						isParameter = true;
						pos = npos;
						break;
					}
					npos++;
				}

				if (isParameter)
					mTokens.push_back( { TokenType::Parameter, value } );
				else
					mTokens.push_back( { TokenType::Identifier, value } );

				if (str[ npos ] == ':')
					mTokens.push_back( { TokenType::Colon, ":" } );

			}

			// Number
			else if (isdigit( c ) || c == '-') {

				String value;
				while (pos < str.size( ) && (isdigit( str[ pos ] ) || (str[ pos ] == '.' || str[ pos ] == '-'))) {
					value += str[ pos++ ];
				}
				if (pos < str.size( ) && isalpha( str[ pos ] )) {
					LUM_LOG_ERROR( "Invalid number token: %s", value.c_str( ) );
				}
				else {
					mTokens.push_back( { TokenType::Number, value } );
				}

			}

			// Left Bracket
			else if (c == '{') {

				++pos;

				mTokens.push_back( { TokenType::LBracket, "{" } );

			}

			// Right Bracket
			else if (c == '}') {

				++pos;

				mTokens.push_back( { TokenType::RBracket, "}" } );

			}

			pos++;

		}

	}

} // namespace lum::fmt