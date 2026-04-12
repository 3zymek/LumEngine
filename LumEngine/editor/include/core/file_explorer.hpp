#pragma once
#include "core/editor_common.hpp"

namespace lum::editor {

	class FileExplorer {
	public:

		void Update( StringView root );
		void DrawDirectory( const std::filesystem::path& path );

	private:



	};

}