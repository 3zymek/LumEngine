#pragma once
#include "core/utils/asset_loader.hpp"
namespace lum::editor {
	struct Fonts {
		inline static ImFont* sDefault = nullptr;
		inline static ImFont* sConsole = nullptr;
		inline static void Initialize( ) {
			auto& io = ImGui::GetIO( );
			sDefault = io.Fonts->AddFontFromFileTTF( AssetLoader::ResolvePath( RootID::Internal, "fonts/Roboto.ttf" ).data( ), 16.0f );
			sConsole = io.Fonts->AddFontFromFileTTF( AssetLoader::ResolvePath( RootID::Internal, "fonts/JetBrainsMono.ttf" ).data( ), 14.0f );
		}
	};
}