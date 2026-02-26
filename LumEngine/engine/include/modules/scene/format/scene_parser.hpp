#pragma once

#include "scene/format/common.hpp"
#include "entity/ecs_manager.hpp"
#include "scene/scene_manager.hpp"

namespace lum {

	struct Scene;

}

namespace lum::fmt {

	struct Tokenizer;
	struct FToken;

	struct FParseContext {
		Scene& mScene;
		ecs::EntityID mEntity;
		FSceneManagerContext mContext;
	};

	class SceneParser {
	public:

		SceneParser( Tokenizer& tokenizer, FSceneManagerContext& ctx ) : mTokenizer( tokenizer ), mContext( ctx ) { }

		void Parse( Scene& scene );

	private:

		static inline bool in_block(std::vector<FToken>& tokens, int32 i) {
			return i < tokens.size() && tokens[i].mType != ETokenType::RBracket;
		}

		static void parse_world( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );

		static void parse_entity	( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );
		static void parse_transform	( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );
		static void parse_smesh		( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );
		static void parse_camera	( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );
		static void parse_render	( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );
		static void parse_material	( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );
		static void parse_name		( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );
		static void parse_directional_light( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );

		Tokenizer& mTokenizer;
		FSceneManagerContext mContext;

		using ParseFn = void(*)(std::vector<FToken>&, int32&, FParseContext&);

		static inline std::unordered_map<uint64, ParseFn> sIdentifiersParseFunctions = {
			{ HashStr("entity"), parse_entity },
			{ HashStr("world"), parse_world }

		};

		static inline std::unordered_map<uint64, ParseFn> sComponentsParseFunctions = {
			{ HashStr("transform"), parse_transform },
			{ HashStr("static_mesh"), parse_smesh },
			{ HashStr("camera"), parse_camera },
			{ HashStr("render"), parse_render },
			{ HashStr("material"), parse_material },
			{ HashStr("name"), parse_name },
			{ HashStr("directional_light"), parse_directional_light }, 
		};

	};

} // namespace lum::fmt