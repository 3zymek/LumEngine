#pragma once
#include "core/editor_common.hpp"
#include "core/utils/fixed_string.hpp"

namespace lum::editor {

	class FileExplorer {
	public:

		void Update( StringView root );

	private:

		FixedString<64> mFileFilter;

		void draw_directory( const std::filesystem::path& path );

	};

}