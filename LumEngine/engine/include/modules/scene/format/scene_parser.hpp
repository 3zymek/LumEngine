//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Parser for .lsc scene format files.
//
//=============================================================================//
#pragma once

#include "scene/format/common.hpp"
#include "entity/ecs_manager.hpp"
#include "scene/scene_manager.hpp"

namespace lum {

	struct FScene;

}
namespace lum::fmt {

	struct Tokenizer;
	struct FToken;

	/* @brief Context passed through the scene parsing pipeline.
	* Holds references to the active scene, current entity and all resource managers.
	*/
	struct FParseContext {

		/* @brief Reference to the scene being populated. */
		FScene& mScene;

		/* @brief Entity currently being parsed and populated with components. */
		ecs::EntityID mEntity;

		/* @brief Resource manager context used to load and resolve assets. */
		FSceneManagerContext mContext;

	};

	/* @brief Parses a tokenized .lsc scene file and populates a FScene.
	* Dispatches identifiers and component blocks to their respective parse functions
	* via hash-keyed lookup tables for efficient routing.
	*/
	class SceneParser {
	public:
		/* @brief Constructs the parser with a tokenizer and resource manager context.
		* @param tokenizer Tokenizer containing the pre-tokenized scene file.
		* @param ctx       Resource manager context for asset resolution.
		*/
		SceneParser( Tokenizer& tokenizer, FSceneManagerContext& ctx ) : mTokenizer( tokenizer ), mContext( ctx ) {}

		/* @brief Parses the token stream and populates the given scene.
		* @param scene Scene to populate with entities and components.
		*/
		void Parse( FScene& scene );

	private:

		/* @brief Returns true while the token stream is inside a block (before RBracket).
		* @param tokens Token stream to check.
		* @param i      Current token index.
		*/
		static inline bool in_block(std::vector<FToken>& tokens, int32 i) 
			{ return i < tokens.size() && tokens[i].mType != TokenType::RBracket; }

		/* @brief Parses the world block and applies global scene settings. */
		static void parse_world( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );

		/* @brief Parses an entity block and creates a new entity in the scene. */
		static void parse_entity( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );

		/* @brief Parses a transform component and attaches it to the current entity. */
		static void parse_transform( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );

		/* @brief Parses a static mesh component and attaches it to the current entity. */
		static void parse_smesh( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );

		/* @brief Parses a camera component and attaches it to the current entity. */
		static void parse_camera( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );

		/* @brief Parses a render component and attaches it to the current entity. */
		static void parse_render( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );

		/* @brief Parses a material component and attaches it to the current entity. */
		static void parse_material( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );

		/* @brief Parses a name component and attaches it to the current entity. */
		static void parse_name( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );

		/* @brief Parses a point light component and attaches it to the current entity. */
		static void parse_point_light( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );




		/* @brief Reference to the tokenizer holding the pre-tokenized scene file. */
		Tokenizer& mTokenizer;

		/* @brief Resource manager context used during parsing for asset resolution. */
		FSceneManagerContext mContext;

		/* @brief Function pointer type for parse dispatch functions. */
		using ParseFn = void(*)(std::vector<FToken>&, int32&, FParseContext&);

		/* @brief Lookup table mapping hashed identifier keywords to their parse functions.
		* Used to dispatch top-level scene constructs such as entity and world blocks.
		*/
		static inline std::unordered_map<uint64, ParseFn> sIdentifiersParseFunctions = {
			{ HashStr("entity"), parse_entity },
			{ HashStr("world"),  parse_world  }
		};

		/* @brief Lookup table mapping hashed component keywords to their parse functions.
		* Used to dispatch component blocks within an entity definition.
		*/
		static inline std::unordered_map<uint64, ParseFn> sComponentsParseFunctions = {
			{ HashStr("transform"),         parse_transform          },
			{ HashStr("static_mesh"),       parse_smesh              },
			{ HashStr("camera"),            parse_camera             },
			{ HashStr("render"),            parse_render             },
			{ HashStr("material"),          parse_material           },
			{ HashStr("name"),              parse_name               },
			{ HashStr("point_light"),       parse_point_light        }
		};

	};

} // namespace lum::fmt