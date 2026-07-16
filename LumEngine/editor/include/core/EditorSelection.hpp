#pragma once

#include "Core/EditorCommon.hpp"

namespace lum::editor {

	/* @brief Type of the currently selected object in the editor. */
	enum class SelectionType { None, Entity, File };

	class EditorSelection {
	public:

		static SelectionType GetSelectionType( ) noexcept { return mType; }
		static void SetSelectionType( SelectionType type ) { mType = type; }

		static EntityID GetSelectedEntity( ) noexcept { return mEntity; }
		static void SetSelectedEntity( EntityID id ) noexcept { mEntity = id; }

		static std::filesystem::path GetSelectedFilepath( ) { return mFilepath; }
		static void SetSelectedFilepath( const std::filesystem::path& path ) { mFilepath = path; }

		static void SelectEntity( EntityID id ) {
			mType = SelectionType::Entity;
			mEntity = id;
		}
		static void SelectFile( const std::filesystem::path& path ) {
			mType = SelectionType::File;
			mFilepath = path;
		}
		static void Clear( ) {
			mType = SelectionType::None;
		}

	private:

		inline static SelectionType mType = SelectionType::None;
		inline static EntityID mEntity = ecs::kNullEntity;
		inline static std::filesystem::path mFilepath;

	};

} // namespace lum::editor