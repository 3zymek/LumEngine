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
		SafePtr<TextureManager> mTextureMgr = nullptr;

		/* @brief Pointer to the active material manager. */
		SafePtr<MaterialManager> mMaterialMgr = nullptr;

		/* @brief Pointer to the active mesh manager. */
		SafePtr<MeshManager> mMeshMgr = nullptr;

		/* @brief Pointer to the active shader manager. */
		SafePtr<ShaderManager> mShaderMgr = nullptr;

		/* @brief Pointer to the active audio manager. */
		SafePtr<AudioManager> mAudioMgr = nullptr;

		/* @brief Pointer to the active event bus. */
		SafePtr<ev::EventBus> mEventBus = nullptr;

		/* @brief Pointer to the active renderer. */
		SafePtr<render::Renderer> mRenderer = nullptr;

	};

	/* @brief Scene and material file format parsing utilities.
	* Handles tokenization and parsing of .lsc (scene) and .lmt (material) files.
	*/
	namespace fmt {

		/* @brief Token types produced by the lexer. */
		enum class TokenType : byte {
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
		enum class FileFormat : byte {
			Scene,    // .lsc scene file
			Material  // .lmt material file
		};

		/* @brief A single token produced by the lexer. */
		struct Token {
			uint32		mLine{};	// Token line in text file
			TokenType	mType{};	// Token type
			String		mValue{};   // Raw string value of the token
			Path		mFilePath{};
		};

		/* @brief Context passed through the scene parsing pipeline.
		* Holds references to the active scene, current entity and all resource managers.
		*/
		struct DeserializeContext {

			/* @brief Reference to the scene being populated. */
			SceneInstance& mScene;

			std::unordered_map<uint64, EntityID> mPersistentToEntity{};
			std::unordered_map<EntityID, std::vector<EntityID>> mPersistentChildren{};

			/* @brief Entity currently being parsed and populated with components. */
			EntityID mCurrentEntity;

			/* @brief Resource manager context used to load and resolve assets. */
			SceneManagerContext mCtx;

		};

		/* @brief Function pointer type for deserialize dispatch functions. */
		using DeserializeFn = void(*)(std::vector<Token>&, int32&, DeserializeContext&);
		using SerializeFn = void(*)(StringBuilder&, ComponentBase*);

		struct SceneComponentInfo {

			String			mSerializationName{};
			uint64			mTypeId{};
			DeserializeFn	mDeserializeFn = nullptr;
			SerializeFn		mSerializeFn = nullptr;

		};

		/*
		struct FieldTypeInfo {

			String mName{};
			String mTypeName{};

			usize mByteSize = 0;
			usize mByteOffset = 0;

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
				return i < tokens.size( ) && tokens[ i ].mType != TokenType::RBracket;
			}

			/* @brief Checks whether the token stream is still inside a square bracket block. */
			inline bool InSquareBlock( std::vector<Token>& tokens, int32 i ) {
				return i < tokens.size( ) && tokens[ i ].mType != TokenType::RSquareBracket;
			}

			/* @brief Checks whether the current token matches the given string. */
			inline bool IsString( std::vector<Token>& tokens, int32& i, StringView str ) {
				return tokens[ i ].mValue == ToLower( str );
			}

			/* @brief Checks whether the current token has the given type. */
			inline bool IsToken( std::vector<Token>& tokens, int32 i, TokenType type ) {
				return (i < tokens.size( ) && tokens[ i ].mType == type);
			}


			// -------------------------------------------------------------------------
			// Token expectations
			// -------------------------------------------------------------------------

			inline void ExceptOpeningBracketInPlace( std::vector<Token>& tokens, int32 i ) {
				if (!IsToken( tokens, i, TokenType::LBracket )) {
					throw DeserializeException(
						"Opening bracket expected at line %llu in file %s",
						tokens[ i ].mLine,
						tokens[ i ].mFilePath.ToString( ).c_str( )
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
						"Opening square bracket expected at line %llu in file %s",
						tokens[ i ].mLine,
						tokens[ i ].mFilePath.ToString( ).c_str( )
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
						"Colon expected at line %llu in file %s",
						tokens[ i ].mLine,
						tokens[ i ].mFilePath.ToString( ).c_str( )
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
						"String expected at line %llu in file %s",
						tokens[ i ].mLine,
						tokens[ i ].mFilePath.ToString( ).c_str( )
					);
				}
				return tokens[ i ].mValue;
			}

			inline String ReadStringParameter( std::vector<Token>& tokens, int32& i ) {
				ExceptColonNext( tokens, i );
				return ReadString( tokens, i );
			}


			inline bool ReadBool( std::vector<Token>& tokens, int32 i ) {
				if (!IsToken( tokens, i, TokenType::Number )) {
					throw DeserializeException(
						"Bool expected at line %llu in file %s",
						tokens[ i ].mLine,
						tokens[ i ].mFilePath.ToString( ).c_str( )
					);
				}
				return std::stof( tokens[ i ].mValue ) > 0;
			}

			inline bool ReadBoolParameter( std::vector<Token>& tokens, int32& i ) {
				ExceptColonNext( tokens, i );
				return ReadBool( tokens, i );
			}


			inline float32 ReadFloat( std::vector<Token>& tokens, int32 i ) {
				if (!IsToken( tokens, i, TokenType::Number )) {
					throw DeserializeException(
						"Float expected at line %llu in file %s",
						tokens[ i ].mLine,
						tokens[ i ].mFilePath.ToString( ).c_str( )
					);
					return 0;
				}
				return std::stof( tokens[ i ].mValue );
			}

			inline float32 ReadFloatParameter( std::vector<Token>& tokens, int32& i ) {
				ExceptColonNext( tokens, i );
				return ReadFloat( tokens, i );
			}


			inline int64 ReadInt( std::vector<Token>& tokens, int32 i ) {
				if (!IsToken( tokens, i, TokenType::Number )) {
					throw DeserializeException(
						"Integer expected at line %llu in file %s",
						tokens[ i ].mLine,
						tokens[ i ].mFilePath.ToString( ).c_str( )
					);
				}
				return std::stoll( tokens[ i ].mValue );
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
							"Vector3 expected at line %llu in file %s",
							tokens[ tokenIndex ].mLine,
							tokens[ tokenIndex ].mFilePath.ToString( ).c_str( )
						);
					}

					vec[ it ] = std::stof( tokens[ tokenIndex ].mValue );
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
							"Vector2 expected at line %llu in file %s",
							tokens[ tokenIndex ].mLine,
							tokens[ tokenIndex ].mFilePath.ToString( ).c_str( )
						);
					}

					vec[ it ] = std::stof( tokens[ tokenIndex ].mValue );
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
				sb.Append( val.mX );
				sb.Append( " " );
				sb.Append( val.mY );
				sb.Append( " " );
				sb.AppendLine( val.mZ );
			}


			inline void WriteVec2Parameter( const Vector2& val, StringBuilder& sb ) {
				sb.Append( "\t\t\t" );
				sb.Append( val.mX );
				sb.Append( " " );
				sb.AppendLine( val.mY );
			}

		} // namespace lum::fmt::detail

	} // namespace lum::fmt

} // namespace lum