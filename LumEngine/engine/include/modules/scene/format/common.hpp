//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Common types, tokens and parsing utilities shared between
//          the .lsc (scene) and .lmt (material) format parsers.
//
//=============================================================================//
#pragma once
#include "core/core_common.hpp"

namespace lum {

	/* @brief Scene and material file format parsing utilities.
	* Handles tokenization and parsing of .lsc (scene) and .lmt (material) files.
	*/
	namespace fmt {

		/* @brief Token types produced by the lexer. */
		enum class TokenType : byte {
			Identifier, // Named keyword or symbol
			LBracket,   // Opening bracket '{'
			RBracket,   // Closing bracket '}'
			Parameter,  // Parameter keyword
			Component,  // Component keyword
			Colon,      // Colon separator ':'
			Dollar,     // Dollar sign '$'
			String,     // String value
			Number,     // Numeric value
			EndOfLine   // End of line marker
		};

		/* @brief Supported file format types. */
		enum class Format : byte {
			Scene,    // .lsc scene file
			Material  // .lmt material file
		};

		/* @brief A single token produced by the lexer. */
		struct FToken {
			TokenType mType;  // Token type
			String mValue;     // Raw string value of the token
		};

		/* @brief Validates whether a file path matches the expected format extension.
		* @param str File path to validate.
		* @param format Expected format type.
		* @return True if extension matches the format (.lsc or .lmt).
		*/
		inline bool IsValidFormat( StringView str, Format format ) {

			usize dot = str.find_last_of( '.' );
			if (dot == StringView::npos)
				return false;

			StringView result = str.substr( dot + 1 );

			if (result == "lsc" && format == Format::Scene)
				return true;
			if (result == "lmt" && format == Format::Material)
				return true;

			return false;
		}

		/* @brief Internal parsing helpers — not intended for direct use. */
		namespace detail {

			inline bool InBlock( std::vector<FToken>& tokens, int32 i ) {
				return i < tokens.size( ) && tokens[ i ].mType != TokenType::RBracket;
			}

			inline bool IsString( std::vector<FToken>& tokens, int32& i, StringView str ) {
				return tokens[ i ].mValue == ToLower( str );
			}

			/* @brief Advances index and asserts the next token is an opening bracket. */
			inline void ExpectOpeningBracket( std::vector<FToken>& tokens, int32& i ) {
				++i;
				LUM_ASSERT( tokens[ i ].mType == TokenType::LBracket, "Opening bracket expected" );
				++i;
			}

			/* @brief Advances index and asserts the next token is a colon. */
			inline void ExpectColon( std::vector<FToken>& tokens, int32& i ) {
				++i;
				LUM_ASSERT( tokens[ i ].mType == TokenType::Colon, "Colon expected" );
				++i;
			}

			/* @brief Reads a string value after a colon separator. */
			inline String ReadStringParameter( std::vector<FToken>& tokens, int32& i ) {
				ExpectColon( tokens, i );
				String value = tokens[ i ].mValue;
				LUM_ASSERT( i + 1 >= tokens.size( ) || tokens[ i + 1 ].mType != TokenType::String, "String expected" );
				return value;
			}

			/* @brief Reads a boolean value (> 0 = true) after a colon separator. */
			inline bool ReadBoolParameter( std::vector<FToken>& tokens, int32& i ) {
				ExpectColon( tokens, i );
				bool value = std::stof( tokens[ i ].mValue ) > 0;
				LUM_ASSERT( i + 1 >= tokens.size( ) || tokens[ i + 1 ].mType != TokenType::Number, "Boolean expected" );
				return value;
			}

			/* @brief Reads a float value after a colon separator. */
			inline float32 ReadFloatParameter( std::vector<FToken>& tokens, int32& i ) {
				ExpectColon( tokens, i );
				float32 value = std::stof( tokens[ i ].mValue );
				LUM_ASSERT( i + 1 >= tokens.size( ) || tokens[ i + 1 ].mType != TokenType::Number, "Float expected" );
				return value;
			}

			/* @brief Reads three consecutive float values as a vec3 after a colon separator. */
			inline glm::vec3 ReadVec3Parameter( std::vector<FToken>& tokens, int32& i ) {
				ExpectColon( tokens, i );
				float32 x = std::stof( tokens[ i++ ].mValue );
				float32 y = std::stof( tokens[ i++ ].mValue );
				float32 z = std::stof( tokens[ i ].mValue );
				LUM_ASSERT( i + 1 >= tokens.size( ) || tokens[ i + 1 ].mType != TokenType::Number, "Vec3 expected" );
				return glm::vec3( x, y, z );
			}

			/* @brief Reads two consecutive float values as a vec2 after a colon separator. */
			inline glm::vec2 ReadVec2Parameter( std::vector<FToken>& tokens, int32& i ) {
				LUM_ASSERT( tokens[ i ].mType == TokenType::Colon, "Colon expected" );
				++i;
				float32 x = std::stof( tokens[ i++ ].mValue );
				float32 y = std::stof( tokens[ i ].mValue );
				LUM_ASSERT( tokens[ i + 1 ].mType != TokenType::Number, "Vec2 expected" );
				return glm::vec2( x, y );
			}

		} // namespace lum::fmt::detail

	} // namespace lum::fmt

} // namespace lum