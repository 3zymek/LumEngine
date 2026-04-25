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

        static const std::unordered_map<HashedStr, FileIconInfo> skIconsMap = {
			{ HashStr( ".cpp" ),      { ICON_FA_CODE,         skFileColorCode     }},
			{ HashStr( ".cc" ),       { ICON_FA_CODE,         skFileColorCode     }},
			{ HashStr( ".h" ),        { ICON_FA_CODE,         skFileColorCode     }},
			{ HashStr( ".hpp" ),      { ICON_FA_CODE,         skFileColorCode     }},
			{ HashStr( ".glsl" ),     { ICON_FA_MAGIC,        skFileColorShader   }},
			{ HashStr( ".vert" ),     { ICON_FA_MAGIC,        skFileColorShader   }},
			{ HashStr( ".frag" ),     { ICON_FA_MAGIC,        skFileColorShader   }},
			{ HashStr( ".hlsl" ),     { ICON_FA_MAGIC,        skFileColorShader   }},
			{ HashStr( ".fbx" ),      { ICON_FA_CUBE,         skFileColorMesh     }},
			{ HashStr( ".obj" ),      { ICON_FA_CUBE,         skFileColorMesh     }},
			{ HashStr( ".gltf" ),     { ICON_FA_CUBE,         skFileColorMesh     }},
			{ HashStr( ".glb" ),      { ICON_FA_CUBE,         skFileColorMesh     }},
			{ HashStr( ".png" ),      { ICON_FA_FILE_IMAGE_O, skFileColorTexture  }},
			{ HashStr( ".jpg" ),      { ICON_FA_FILE_IMAGE_O, skFileColorTexture  }},
			{ HashStr( ".jpeg" ),     { ICON_FA_FILE_IMAGE_O, skFileColorTexture  }},
			{ HashStr( ".dds" ),      { ICON_FA_FILE_IMAGE_O, skFileColorMaterial }},
			{ HashStr( ".hdr" ),      { ICON_FA_SUN_O,        skFileColorHDR      }},
			{ HashStr( ".exr" ),      { ICON_FA_SUN_O,        skFileColorHDR      }},
			{ HashStr( ".wav" ),      { ICON_FA_MUSIC,        skFileColorAudio    }},
			{ HashStr( ".mp3" ),      { ICON_FA_MUSIC,        skFileColorAudio    }},
			{ HashStr( ".ogg" ),      { ICON_FA_FILE_AUDIO_O, skFileColorAudio    }},
			{ HashStr( ".json" ),     { ICON_FA_FILE_CODE_O,  skFileColorData     }},
			{ HashStr( ".yaml" ),     { ICON_FA_LIST,         skFileColorData     }},
			{ HashStr( ".toml" ),     { ICON_FA_COG,          skFileColorData     }},
			{ HashStr( ".lsc" ),	  { ICON_FA_MAP,          skFileColorScene    }},
			{ HashStr( ".lmt" ),      { ICON_FA_ADJUST,       skFileColorMaterial }},
			{ HashStr( ".lua" ),      { ICON_FA_FILE_TEXT_O,  skFileColorData     }},
			{ HashStr( ".txt" ),      { ICON_FA_FILE_TEXT_O,  skFileColorDefault  }},
			{ HashStr( ".pdf" ),      { ICON_FA_FILE_PDF_O,   skFileColorError    }},
			{ HashStr( ".zip" ),      { ICON_FA_FILE_ARCHIVE_O, skFileColorData   }},
			{ HashStr( ".mp4" ),      { ICON_FA_FILE_VIDEO_O, skFileColorError    }},
			{ HashStr( ".dll" ),      { ICON_FA_PUZZLE_PIECE, skFileColorError    }},
        };

		HashedStr hash = HashStr( path.extension( ).string( ).c_str( ) );
		auto it = skIconsMap.find( hash );
		if (it != skIconsMap.end( ))
			return it->second;

		return {};

	}

}