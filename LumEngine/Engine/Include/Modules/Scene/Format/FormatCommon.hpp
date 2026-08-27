//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Common types, tokens and parsing utilities shared between
//          the .lsc (scene) and .lmt (material) format parsers.
//
//=============================================================================//
#pragma once

#include "Core/CoreCommon.hpp"
#include "Entity/EcsCommon.hpp"
#include "Platform/FileSystem/FileSystem.hpp"
#include "Core/Utils/StringBuilder.hpp"
#include "Scene/Format/DeserializeException.hpp"

namespace lum {

	// Forward Declare
	class TextureManager;
	class MaterialManager;
	class MeshManager;
	class ShaderManager;
	class AudioManager;
	struct SceneInstance;
	namespace render { class Renderer; }
	namespace ev { class EventBus; }
	////////////////////////


	/* @brief Aggregates all resource manager pointers required by the SceneManager
	*  to load and initialize scene assets.
	*/
	struct SceneManagerContext {

		/* @brief Pointer to the active texture manager. */
		SafePtr<TextureManager> m_TextureMgr = nullptr;

		/* @brief Pointer to the active material manager. */
		SafePtr<MaterialManager> m_MaterialMgr = nullptr;

		/* @brief Pointer to the active mesh manager. */
		SafePtr<MeshManager> m_MeshMgr = nullptr;

		/* @brief Pointer to the active shader manager. */
		SafePtr<ShaderManager> m_ShaderMgr = nullptr;

		/* @brief Pointer to the active audio manager. */
		SafePtr<AudioManager> m_AudioMgr = nullptr;

		/* @brief Pointer to the active event bus. */
		SafePtr<ev::EventBus> m_EventBus = nullptr;

		/* @brief Pointer to the active renderer. */
		SafePtr<render::Renderer> m_Renderer = nullptr;

	};

	/* @brief Scene and material file format parsing utilities.
	* Handles tokenization and parsing of .lsc (scene) and .lmt (material) files.
	*/
	namespace fmt {

		/* @brief Token types produced by the lexer. */
		enum class TokenType {
			Identifier, // Named keyword or symbol
			LBracket,   // {
			RBracket,   // }
			LSquareBracket, // [
			RSquareBracket, // ]
			Parameter,  // Parameter keyword
			Component,  // Component keyword
			Colon,      // :
			Dollar,     // $
			String,     // String value
			Number,     // Numeric value
			EndOfLine   // End of line marker
		};

		/* @brief Supported file format types. */
		enum class FileFormat {
			Scene,    // .lsc scene file
			Material  // .lmt material file
		};

		/* @brief A single token produced by the lexer. */
		struct Token {
			uint32		m_Line{};	// Token line in text file
			TokenType	m_Type{};	// Token type
			String		m_Value{};   // Raw string value of the token
			Path		m_FilePath{};
		};

		/* @brief Context passed through the scene parsing pipeline.
		* Holds references to the active scene, current entity and all resource managers.
		*/
		struct DeserializeContext {

			/* @brief Reference to the scene being populated. */
			SceneInstance& m_Scene;

			std::unordered_map<uint64, EntityID> m_PersistentToEntity{};
			std::unordered_map<EntityID, std::vector<EntityID>> m_PersistentChildren{};

			/* @brief Entity currently being parsed and populated with components. */
			EntityID m_CurrentEntity{};

			/* @brief Resource manager context used to load and resolve assets. */
			SceneManagerContext m_Ctx{};

		};

		/* @brief Function pointer type for deserialize dispatch functions. */
		using DeserializeFn = void(*)(std::vector<Token>&, int32&, DeserializeContext&);
		using SerializeFn = void(*)(StringBuilder&, ComponentBase*);

		struct SceneComponentInfo {

			String			m_SerializationName{};
			uint64			m_TypeId{};
			DeserializeFn	m_DeserializeFn = nullptr;
			SerializeFn		m_SerializeFn = nullptr;

		};

		/*
		struct FieldTypeInfo {

			String m_Name{};
			String m_TypeName{};

			usize m_ByteSize = 0;
			usize m_ByteOffset = 0;

		};
		*/

		void RegisterSceneComponents( std::vector<SceneComponentInfo>& infos );

		/* @brief Validates whether a file path matches the expected format extension.
		* @param file File path to validate.
		* @param format Expected format type.
		* @return True if extension matches the format.
		*/
		inline bool IsValidFormat( const Path& file, FileFormat format ) {

			String ext = file.Extension( );
			if (ext == ".lsc" && format == FileFormat::Scene)
				return true;
			else if (ext == ".lmt" && format == FileFormat::Material)
				return true;

			return false;

		}

		/* @brief Internal parsing helpers — not intended for direct use. */
		namespace detail {

			// -------------------------------------------------------------------------
			// Token checks
			// -------------------------------------------------------------------------

			/* @brief Checks whether the token stream is still inside a curly bracket block. */
			inline bool InBlock( std::vector<Token>& tokens, int32 i ) {
				return i < tokens.size( ) && tokens[ i ].m_Type != TokenType::RBracket;
			}

			/* @brief Checks whether the token stream is still inside a square bracket block. */
			inline bool InSquareBlock( std::vector<Token>& tokens, int32 i ) {
				return i < tokens.size( ) && tokens[ i ].m_Type != TokenType::RSquareBracket;
			}

			/* @brief Checks whether the current token matches the given string. */
			inline bool IsString( std::vector<Token>& tokens, int32& i, StringView str ) {
				return tokens[ i ].m_Value == ToLower( str );
			}

			/* @brief Checks whether the current token has the given type. */
			inline bool IsToken( std::vector<Token>& tokens, int32 i, TokenType type ) {
				return (i < tokens.size( ) && tokens[ i ].m_Type == type);
			}


			// -------------------------------------------------------------------------
			// Token expectations
			// -------------------------------------------------------------------------

			inline void ExceptOpeningBracketInPlace( std::vector<Token>& tokens, int32 i ) {
				if (!IsToken( tokens, i, TokenType::LBracket )) {
					throw DeserializeException(
						"Opening bracket expected at line {} in file {}",
						tokens[ i ].m_Line,
						tokens[ i ].m_FilePath.ToString( ).c_str( )
					);
				}
			}

			inline void ExpectOpeningBracketNext( std::vector<Token>& tokens, int32& i ) {
				++i;
				ExceptOpeningBracketInPlace( tokens, i );
				++i;
			}


			inline void ExceptOpeningSquareBracketInPlace( std::vector<Token>& tokens, int32 i ) {
				if (!IsToken( tokens, i, TokenType::LSquareBracket )) {
					throw DeserializeException(
						"Opening square bracket expected at line {} in file {}",
						tokens[ i ].m_Line,
						tokens[ i ].m_FilePath.ToString( ).c_str( )
					);
				}
			}

			inline void ExpectOpeningSquareBracketNext( std::vector<Token>& tokens, int32& i ) {
				++i;
				ExceptOpeningSquareBracketInPlace( tokens, i );
				++i;
			}


			inline void ExceptColonInPlace( std::vector<Token>& tokens, int32 i ) {
				if (!IsToken( tokens, i, TokenType::Colon )) {
					throw DeserializeException(
						"Colon expected at line {} in file {}",
						tokens[ i ].m_Line,
						tokens[ i ].m_FilePath.ToString( ).c_str( )
					);
				}
			}

			inline void ExceptColonNext( std::vector<Token>& tokens, int32& i ) {
				++i;
				ExceptColonInPlace( tokens, i );
				++i;
			}


			// -------------------------------------------------------------------------
			// Value readers
			// -------------------------------------------------------------------------

			inline String ReadString( std::vector<Token>& tokens, int32 i ) {
				if (!IsToken( tokens, i, TokenType::String )) {
					throw DeserializeException(
						"String expected at line {} in file {}",
						tokens[ i ].m_Line,
						tokens[ i ].m_FilePath.ToString( ).c_str( )
					);
				}
				return tokens[ i ].m_Value;
			}

			inline String ReadStringParameter( std::vector<Token>& tokens, int32& i ) {
				ExceptColonNext( tokens, i );
				return ReadString( tokens, i );
			}


			inline bool ReadBool( std::vector<Token>& tokens, int32 i ) {
				if (!IsToken( tokens, i, TokenType::Number )) {
					throw DeserializeException(
						"Bool expected at line {} in file {}",
						tokens[ i ].m_Line,
						tokens[ i ].m_FilePath.ToString( ).c_str( )
					);
				}
				return std::stof( tokens[ i ].m_Value ) > 0;
			}

			inline bool ReadBoolParameter( std::vector<Token>& tokens, int32& i ) {
				ExceptColonNext( tokens, i );
				return ReadBool( tokens, i );
			}


			inline float32 ReadFloat( std::vector<Token>& tokens, int32 i ) {
				if (!IsToken( tokens, i, TokenType::Number )) {
					throw DeserializeException(
						"Float expected at line {} in file {}",
						tokens[ i ].m_Line,
						tokens[ i ].m_FilePath.ToString( ).c_str( )
					);
					return 0;
				}
				return std::stof( tokens[ i ].m_Value );
			}

			inline float32 ReadFloatParameter( std::vector<Token>& tokens, int32& i ) {
				ExceptColonNext( tokens, i );
				return ReadFloat( tokens, i );
			}


			inline int64 ReadInt( std::vector<Token>& tokens, int32 i ) {
				if (!IsToken( tokens, i, TokenType::Number )) {
					throw DeserializeException(
						"Integer expected at line {} in file {}",
						tokens[ i ].m_Line,
						tokens[ i ].m_FilePath.ToString( ).c_str( )
					);
				}
				return std::stoll( tokens[ i ].m_Value );
			}

			inline int64 ReadIntParameter( std::vector<Token>& tokens, int32& i ) {
				ExceptColonNext( tokens, i );
				return ReadInt( tokens, i );
			}


			inline Vector3 ReadVec3( std::vector<Token>& tokens, int32 i ) {
				float32 vec[ 3 ]{};

				for (int32 it = 0; it < 3; it++) {
					int32 tokenIndex = i + it;

					if (!IsToken( tokens, tokenIndex, TokenType::Number )) {
						throw DeserializeException(
							"Vector3 expected at line {} in file {}",
							tokens[ tokenIndex ].m_Line,
							tokens[ tokenIndex ].m_FilePath.ToString( ).c_str( )
						);
					}

					vec[ it ] = std::stof( tokens[ tokenIndex ].m_Value );
				}

				return Vector3( vec[ 0 ], vec[ 1 ], vec[ 2 ] );
			}

			inline Vector3 ReadVec3Parameter( std::vector<Token>& tokens, int32& i ) {
				ExceptColonNext( tokens, i );
				return ReadVec3( tokens, i );
			}


			inline Vector2 ReadVec2( std::vector<Token>& tokens, int32 i ) {
				float32 vec[ 2 ]{};

				for (int32 it = 0; it < 2; it++) {
					int32 tokenIndex = i + it;

					if (!IsToken( tokens, tokenIndex, TokenType::Number )) {
						throw DeserializeException(
							"Vector2 expected at line {} in file {}",
							tokens[ tokenIndex ].m_Line,
							tokens[ tokenIndex ].m_FilePath.ToString( ).c_str( )
						);
					}

					vec[ it ] = std::stof( tokens[ tokenIndex ].m_Value );
				}

				return Vector2( vec[ 0 ], vec[ 1 ] );
			}

			inline Vector2 ReadVec2Parameter( std::vector<Token>& tokens, int32& i ) {
				ExceptColonNext( tokens, i );
				return ReadVec2( tokens, i );
			}


			// -------------------------------------------------------------------------
			// Value writers
			// -------------------------------------------------------------------------

			inline void WriteStringParameter( const String& val, StringBuilder& sb ) {
				sb.Append( "\t\t\t" );
				sb.Append( "\"" );
				sb.Append( val );
				sb.AppendLine( "\"" );
			}

			template<usize tSize>
			inline void WriteStringParameter( const FixedString<tSize>& val, StringBuilder& sb ) {
				sb.Append( "\t\t\t" );
				sb.Append( "\"" );
				sb.Append( val.Data( ) );
				sb.AppendLine( "\"" );
			}


			inline void WriteBoolParameter( const bool val, StringBuilder& sb ) {
				sb.Append( "\t\t\t" );
				sb.AppendLine( val ? "1" : "0" );
			}


			inline void WriteFloatParameter( const float32 val, StringBuilder& sb ) {
				sb.Append( "\t\t\t" );
				sb.AppendLine( val );
			}


			inline void WriteIntParameter( const int64 val, StringBuilder& sb ) {
				sb.Append( "\t\t\t" );
				sb.AppendLine( val );
			}


			inline void WriteVec3Parameter( const Vector3& val, StringBuilder& sb ) {
				sb.Append( "\t\t\t" );
				sb.Append( val.m_X );
				sb.Append( " " );
				sb.Append( val.m_Y );
				sb.Append( " " );
				sb.AppendLine( val.m_Z );
			}


			inline void WriteVec2Parameter( const Vector2& val, StringBuilder& sb ) {
				sb.Append( "\t\t\t" );
				sb.Append( val.m_X );
				sb.Append( " " );
				sb.AppendLine( val.m_Y );
			}

		} // namespace lum::fmt::detail

	} // namespace lum::fmt

} // namespace lum