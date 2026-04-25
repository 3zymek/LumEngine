#pragma once
#include "core/editor_common.hpp"
#include "core/utils/fixed_string.hpp"
#include "core/utils/file_icons.hpp"

namespace lum::editor {

	enum class FileType : byte {
		SourceCode,
		Mesh,
		Image,
		Audio,
		LumEngineAsset,
		ConfigData,
		Document,
		Video,
		Binary,
		Directory
	};

	class FileExplorer {
	public:

		void Update( StringView root );

	private:

		FixedString<64> mFileFilter;
		std::filesystem::path mSelectedPath{};

		void draw_directory( const std::filesystem::path& path );

	};

}