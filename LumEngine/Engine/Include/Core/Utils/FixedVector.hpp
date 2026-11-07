/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#pragma once

#include "Core/CoreTypes.hpp"
#include "Core/CoreDefines.hpp"
#include "Core/Utils/Assert.hpp"

namespace lum {

	template<typename tType, usize tSize>
	class FixedVector {
	public:

		void PushBack( tType&& val ) {
			LUM_ASSERT( m_CurrentSize < tSize, "FixedVector<T> limit reached" );
			m_Data[ m_CurrentSize++ ] = std::move( val );
		}
		void PushBack( const tType& val ) {
			LUM_ASSERT( m_CurrentSize < tSize, "FixedVector<T> limit reached" );
			m_Data[ m_CurrentSize++ ] = val;
		}

		void PopBack( ) {
			if (m_CurrentSize > 0)
				m_CurrentSize--;
		}

		void Clear( ) {
			m_CurrentSize = 0;
		}

		tType& operator[]( const usize index ) {
			LUM_ASSERT( index < m_CurrentSize, "Invalid FixedArray index" );
			return m_Data[ index ];
		}

		const tType& operator[]( const usize index ) const {
			LUM_ASSERT( index < m_CurrentSize, "Invalid FixedArray index" );
			return m_Data[ index ];
		}

		LUM_NODISCARD usize Size( ) const noexcept {
			return m_CurrentSize;
		}
		LUM_NODISCARD usize Capacity( ) const noexcept {
			return tSize;
		}

		LUM_NODISCARD tType* Data( ) noexcept {
			return m_Data;
		}

		LUM_NODISCARD const tType* Data( ) const noexcept {
			return m_Data;
		}

		LUM_NODISCARD tType* begin( ) {
			return m_Data;
		}
		LUM_NODISCARD tType* end( ) {
			return m_Data + m_CurrentSize;
		}

		LUM_NODISCARD const tType* begin( ) const {
			return m_Data;
		}
		LUM_NODISCARD const tType* end( ) const {
			return m_Data + m_CurrentSize;
		}

	private:

		tType m_Data[ tSize ]{};
		usize m_CurrentSize = 0;

	};

} // namespace lum