#pragma once
#include "core/utils/asset_loader.hpp"
#include "core/utils/fa_solid.h"
#include "core/utils/icons_font_awesome.h"
namespace lum::editor {
	struct Fonts {
		inline static ImFont* sDefault = nullptr;
		inline static ImFont* sConsole = nullptr;
		inline static void Initialize( ) {
			auto& io = ImGui::GetIO( );
			sDefault = io.Fonts->AddFontFromFileTTF( AssetLoader::ResolvePath( RootID::Internal, "fonts/Inter.ttf" ).data( ), 16.0f );

			static const ImWchar faRanges[ ] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
			ImFontConfig faConfig;
			faConfig.MergeMode = true;
			faConfig.PixelSnapH = true;
			faConfig.GlyphMinAdvanceX = 16.0f;
			io.Fonts->AddFontFromMemoryTTF(
				( void* ) fa_solid_900_ttf,
				fa_solid_900_ttf_len,
				16.0f, &faConfig, faRanges
			);

			sConsole = io.Fonts->AddFontFromFileTTF( AssetLoader::ResolvePath( RootID::Internal, "fonts/JetBrainsMono.ttf" ).data( ), 18.0f );

		}
	};
}