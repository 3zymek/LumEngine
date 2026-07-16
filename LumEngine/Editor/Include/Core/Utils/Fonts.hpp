#pragma once
#include "Core/Utils/ResourceLoader.hpp"
#include "Core/Utils/IconsFontAwesome.h"

namespace lum::editor {
	struct Fonts {

        inline static ImFont* sDefaultSuperSmall = nullptr;
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

            auto path = []( StringView name ) {
                return ResourceLoader::ResolvePath( ResourceRoot::Internal, name );
            };

            load_font( sDefaultSuperSmall, path( "Fonts/Inter.ttf"), 12.0f, &faConfig, faRanges );
            load_font( sDefaultSmall, path( "Fonts/Inter.ttf" ), 16.0f, &faConfig, faRanges );
            load_font( sDefaultMedium, path( "Fonts/Inter.ttf" ), 20.0f, &faConfig, faRanges );
            load_font( sDefaultBig, path( "Fonts/Inter.ttf" ), 23.0f, &faConfig, faRanges );

            sConsole = io.Fonts->AddFontFromFileTTF( path( "Fonts/JetBrainsMono.ttf" ).data( ), 18.0f );

            io.Fonts->Build( );

		}

    private:

        static void load_font( ImFont*& font, StringView path, usize size, const ImFontConfig* cfg, const ImWchar* ranges ) {

            ImGuiIO& io = ImGui::GetIO( );

            font = io.Fonts->AddFontFromFileTTF( path.data(), size );
            io.Fonts->AddFontFromFileTTF(
                ResourceLoader::ResolvePath( ResourceRoot::Internal, "fonts/FontAwesome.ttf" ).data( ),
                size,
                cfg,
                ranges
            );

        }

	};
}