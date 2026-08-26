//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Watched<T> — a wrapper that tracks modifications to a contained value.
//          Marks itself when accessed via operator->(), allowing systems to detect
//          and flush changes without per-field dirty flags.
//
//=============================================================================//
#pragma once

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
		tType* operator->( ) { 
			m_Marked = true; 
			return &m_Value; 
		}

		/* @brief Returns a pointer to the value without marking. */
		const tType* operator->( ) const { 
			return &m_Value; 
		}

		/* @brief Returns true if the value has been modified since the last Unmark(). */
		bool IsMarked( ) const noexcept { 
			return m_Marked; 
		}

		/* @brief Clears the modified mark. Call after processing the change. */
		void Unmark( ) { 
			m_Marked = false; 
		}

	private:

		tType m_Value{};
		bool  m_Marked = false;

	};

} // namespace lum::cstd