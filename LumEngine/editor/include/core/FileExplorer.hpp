#pragma once
#include "Core/EditorCommon.hpp"
#include "Core/Utils/FixedString.hpp"
#include "Core/Utils/FileIcons.hpp"

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