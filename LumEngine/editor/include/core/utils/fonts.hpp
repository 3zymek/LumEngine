#pragma once
#include "core/utils/asset_loader.hpp"
#include "core/utils/icons_font_awesome.h"

namespace lum::editor {
	struct Fonts {
		inline static ImFont* sDefaultSmall = nullptr;
        inline static ImFont* sDefaultMedium = nullptr;
        inline static ImFont* sDefaultBig = nullptr;
		inline static ImFont* sConsole = nullptr;
		inline static void Initialize( ) {

			auto& io = ImGui::GetIO( );

            static const ImWchar faRanges[ ] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

            ImFontConfig faConfig;
            faConfig.MergeMode = true;
            faConfig.PixelSnapH = true;
            faConfig.GlyphMinAdvanceX = 16.0f;

            sDefaultSmall = io.Fonts->AddFontFromFileTTF( AssetLoader::ResolvePath( RootID::Internal, "fonts/Inter.ttf" ).data( ), 16.0f );
            io.Fonts->AddFontFromFileTTF(
                AssetLoader::ResolvePath( RootID::Internal, "fonts/FontAwesome.ttf" ).data( ),
                16.0f,
                &faConfig,
                faRanges
            );

            sDefaultMedium = io.Fonts->AddFontFromFileTTF( AssetLoader::ResolvePath( RootID::Internal, "fonts/Inter.ttf" ).data( ), 20.0f );
            io.Fonts->AddFontFromFileTTF(
                AssetLoader::ResolvePath( RootID::Internal, "fonts/FontAwesome.ttf" ).data( ),
                16.0f,
                &faConfig,
                faRanges
            );

            sDefaultBig = io.Fonts->AddFontFromFileTTF( AssetLoader::ResolvePath( RootID::Internal, "fonts/Inter.ttf" ).data( ), 23.0f );
            io.Fonts->AddFontFromFileTTF(
                AssetLoader::ResolvePath( RootID::Internal, "fonts/FontAwesome.ttf" ).data( ),
                16.0f,
                &faConfig,
                faRanges
            );

			sConsole = io.Fonts->AddFontFromFileTTF( AssetLoader::ResolvePath( RootID::Internal, "fonts/JetBrainsMono.ttf" ).data( ), 18.0f );

            ImGui::GetIO( ).Fonts->Build( );

		}
	};
}