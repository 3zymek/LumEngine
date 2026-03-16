//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Watched<T> — a wrapper that tracks modifications to a contained value.
//          Marks itself when accessed via operator->(), allowing systems to detect
//          and flush changes without per-field dirty flags.
//
//=============================================================================//
#pragma once

#include "core/core_common.hpp"

namespace lum::cstd {

	/* @brief Wrapper that tracks whether the contained value has been modified.
	*
	* Access through operator->() marks the value as modified.
	* Use IsMarked() to check for changes and Unmark() to reset after processing.
	*/
	template<typename tType>
	struct Watched {
	public:

		/* @brief Marks the value as modified and returns a pointer to it. */
		tType* operator->( ) { bMarked = true; return &mValue; }

		/* @brief Returns a pointer to the value without marking. */
		const tType* operator->( ) const { return &mValue; }

		/* @brief Returns a const reference to the contained value without marking. */
		const tType& Get( ) const { return mValue; }

		/* @brief Returns true if the value has been modified since the last Unmark(). */
		bool IsMarked( ) const { return bMarked; }

		/* @brief Clears the modified mark. Call after processing the change. */
		void Unmark( ) { bMarked = false; }

	private:

		tType mValue;
		bool  bMarked = false;

	};

} // namespace lum::cstd