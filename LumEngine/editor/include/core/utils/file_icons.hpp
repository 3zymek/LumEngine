#pragma once

#include "core/editor_common.hpp"
#include "core/core_common.hpp"

namespace lum::editor {

	static const ImVec4 skFileColorCode		= { 0.22f, 0.54f, 0.85f, 1.f };
	static const ImVec4 skFileColorShader	= { 0.39f, 0.78f, 0.46f, 1.f };
	static const ImVec4 skFileColorMesh		= { 0.85f, 0.35f, 0.19f, 1.f };
	static const ImVec4 skFileColorTexture	= { 0.83f, 0.33f, 0.49f, 1.f };
	static const ImVec4 skFileColorHDR		= { 0.11f, 0.62f, 0.46f, 1.f };
	static const ImVec4 skFileColorAudio	= { 0.73f, 0.46f, 0.09f, 1.f };
	static const ImVec4 skFileColorScene	= { 0.22f, 0.54f, 0.85f, 1.f };
	static const ImVec4 skFileColorMaterial = { 0.50f, 0.47f, 0.87f, 1.f };
	static const ImVec4 skFileColorData		= { 0.53f, 0.53f, 0.50f, 1.f };
	static const ImVec4 skFileColorFolder	= { 0.73f, 0.46f, 0.09f, 1.f };
	static const ImVec4 skFileColorDefault	= { 0.60f, 0.60f, 0.58f, 1.f };
	static const ImVec4 skFileColorError	= { 0.89f, 0.29f, 0.29f, 1.f };

	struct FileIconInfo {
		ccharptr mIcon = ICON_FA_CUBE;
		ImVec4 mColor = skFileColorDefault;
	};

	inline FileIconInfo GetFileIcon( const std::filesystem::path& path, bool isDir = false ) {

		if (isDir)
			return { ICON_FA_FOLDER, skFileColorFolder };

        static const std::unordered_map<HashedString, FileIconInfo> skIconsMap = {
            { HashString( ".cpp" ),      { ICON_FA_CODE,              skFileColorCode     }},
            { HashString( ".cc" ),       { ICON_FA_CODE,              skFileColorCode     }},
            { HashString( ".h" ),        { ICON_FA_CODE,              skFileColorCode     }},
            { HashString( ".hpp" ),      { ICON_FA_CODE,              skFileColorCode     }},
            { HashString( ".glsl" ),     { ICON_FA_MAGIC,             skFileColorShader   }},
            { HashString( ".vert" ),     { ICON_FA_MAGIC,             skFileColorShader   }},
            { HashString( ".frag" ),     { ICON_FA_MAGIC,             skFileColorShader   }},
            { HashString( ".hlsl" ),     { ICON_FA_MAGIC,             skFileColorShader   }},
            { HashString( ".fbx" ),      { ICON_FA_CUBE,              skFileColorMesh     }},
            { HashString( ".obj" ),      { ICON_FA_CUBE,              skFileColorMesh     }},
            { HashString( ".gltf" ),     { ICON_FA_CUBE,              skFileColorMesh     }},
            { HashString( ".glb" ),      { ICON_FA_CUBE,              skFileColorMesh     }},
            { HashString( ".png" ),      { ICON_FA_IMAGE,             skFileColorTexture  }},
            { HashString( ".jpg" ),      { ICON_FA_IMAGE,             skFileColorTexture  }},
            { HashString( ".jpeg" ),     { ICON_FA_IMAGE,             skFileColorTexture  }},
            { HashString( ".dds" ),      { ICON_FA_IMAGE,             skFileColorMaterial }},
            { HashString( ".hdr" ),      { ICON_FA_SUN,               skFileColorHDR      }},
            { HashString( ".exr" ),      { ICON_FA_SUN,               skFileColorHDR      }},
            { HashString( ".wav" ),      { ICON_FA_MUSIC,             skFileColorAudio    }},
            { HashString( ".mp3" ),      { ICON_FA_MUSIC,             skFileColorAudio    }},
            { HashString( ".ogg" ),      { ICON_FA_HEADPHONES,        skFileColorAudio    }},
            { HashString( ".json" ),     { ICON_FA_FILE_CODE,         skFileColorData     }},
            { HashString( ".yaml" ),     { ICON_FA_LIST,              skFileColorData     }},
            { HashString( ".toml" ),     { ICON_FA_COG,               skFileColorData     }},
            { HashString( ".lsc" ),      { ICON_FA_MAP,               skFileColorScene    }},
            { HashString( ".lmt" ),      { ICON_FA_ADJUST,            skFileColorMaterial }},
            { HashString( ".lua" ),      { ICON_FA_FILE_ALT,          skFileColorData     }},
            { HashString( ".txt" ),      { ICON_FA_FILE_ALT,          skFileColorDefault  }},
            { HashString( ".pdf" ),      { ICON_FA_FILE_PDF,          skFileColorError    }},
            { HashString( ".zip" ),      { ICON_FA_FILE_ARCHIVE,      skFileColorData     }},
            { HashString( ".mp4" ),      { ICON_FA_FILE_VIDEO,        skFileColorError    }},
            { HashString( ".dll" ),      { ICON_FA_PUZZLE_PIECE,      skFileColorError    }},
        };

		HashedString hash = HashString( path.extension( ).string( ).c_str( ) );
		auto it = skIconsMap.find( hash );
		if (it != skIconsMap.end( ))
			return it->second;

		return {};

	}

}