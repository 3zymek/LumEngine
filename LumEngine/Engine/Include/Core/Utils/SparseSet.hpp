//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Dense buffer + sparse index map. O(1) insert/erase without holes.
//          Perfect for ECS component storage (fast random access by entity ID).
//
//=============================================================================//
#pragma once

#include "Core/CoreCommon.hpp"

namespace lum::cstd {


	/* @brief Cache-friendly sparse set container.
	*
	* Maps sparse integer keys to densely packed values.
	* Provides O(1) insert, remove, and lookup while keeping
	* values contiguous in memory for fast iteration.
	*
	* @tparam tSparseVal Arithmetic type used as the key (e.g. entity ID).
	* @tparam tDenseVal  Type of the stored values (e.g. components).
	*/
	template<cArithmetic tSparse, typename tDense>
	class SparseSet {

		using SparseT = tSparse;

	public:

		/* @brief Constructs the sparse set with a fixed maximum key capacity.
		* @param size Maximum sparse key value that can be stored.
		*/
		constexpr SparseSet( SparseT maxSize ) : k_MaxSize( maxSize ) { init( maxSize ); }
		~SparseSet( ) = default;


		// Standard iterators over dense value array.
		using Iterator = typename std::vector<tDense>::iterator;
		using ConstIterator = typename std::vector<tDense>::const_iterator;
		Iterator begin( ) { return m_Dense.begin( ); }
		Iterator end( ) { return m_Dense.end( ); }
		ConstIterator begin( ) const { return m_Dense.begin( ); }
		ConstIterator end( ) const { return m_Dense.end( ); }


		/* @brief Iterator that yields key-value pairs during iteration.
		*
		* Allows range-based for loops with structured bindings:
		* for (auto [key, value] : set.Each()) { ... }
		*/
		struct KeyValIterator {

			tDense*		m_Dense{};          // Pointer to dense value array.
			SparseT*	m_DenseToSparse{};  // Pointer to key array parallel to dense.
			SparseT		m_Index{};          // Current position in dense array.

			/* @brief Returns the current key-value pair.
			* @return Pair of (sparse key, pointer to dense value).
			*/
			std::pair<SparseT, tDense*> operator*( ) {
				return { m_DenseToSparse[ m_Index ], &m_Dense[ m_Index ] };
			}
			KeyValIterator& operator++( ) { m_Index++; return *this; }
			bool operator!=( const KeyValIterator& other ) { return m_Index != other.m_Index; }

		};

		/* @brief Range wrapper returned by Each(), enables range-based for. */
		struct KeyValRange {

			KeyValIterator m_Begin{};
			KeyValIterator m_End{};
			KeyValIterator begin( ) { return m_Begin; }
			KeyValIterator end( ) { return m_End; }

		};

		/* @brief Returns a key-value range for structured binding iteration.
		* @return KvRange over all currently stored key-value pairs.
		*/
		KeyValRange Each( ) {
			return {
				{ m_Dense.data( ), m_DenseToSparse.data( ), 0 },
				{ m_Dense.data( ), m_DenseToSparse.data( ), static_cast< SparseT >(m_Dense.size( )) }
			};
		}


		/* @brief Returns value at sparse index. No bounds checking.
		* @param idx Sparse key to look up.
		*/
		inline constexpr tDense& operator[]( SparseT idx ) {
			return m_Dense[ m_Sparse[ idx ] ];
		}

		/* @brief Returns value at sparse index with bounds and null checking.
		* @param idx Sparse key to look up.
		*/
		constexpr tDense* Get( SparseT idx ) {

			if ((idx > k_MaxSize) || (m_Sparse[ idx ] == sk_NullSparse)) return nullptr;

			return &m_Dense[ m_Sparse[ idx ] ];

		}

		/* @brief Inserts a value at the given sparse key.
		* Does nothing if the key is out of range or already occupied.
		* @param value Value to insert (forwarded).
		* @param idx   Sparse key to insert at.
		*/
		constexpr void Append( tDense value, SparseT idx ) {

			if ((idx >= k_MaxSize) || (m_Sparse[ idx ] != sk_NullSparse)) return;

			m_Dense.emplace_back( std::move( value ) );
			m_Sparse[ idx ] = m_Dense.size( ) - 1;
			m_DenseToSparse.push_back( idx );

		}

		inline constexpr SparseT MaxSize( )	const noexcept { return k_MaxSize; }   // Maximum sparse capacity.
		inline constexpr SparseT DenseSize( )	const noexcept { return m_Dense.size( ); } // Number of stored elements.
		inline constexpr bool    DenseEmpty( )	const noexcept { return m_Dense.empty( ); }

		/* @brief Clears all stored values and resets the container.
		* Sparse array is refilled with null sentinels after clear.
		*/
		inline constexpr void Clear( ) {

			m_Dense.clear( );
			m_Sparse.clear( );
			m_DenseToSparse.clear( );

			init( k_MaxSize );

		}

		/* @brief Removes the value at the given sparse key.
		* Uses swap-with-last to maintain dense packing. O(1).
		* @param idx Sparse key to remove.
		*/
		void Remove( SparseT idx ) {

			if ((idx > k_MaxSize) || (m_Sparse[ idx ] == sk_NullSparse)) return;

			SparseT toDelete = m_Sparse[ idx ];
			SparseT last = m_Dense.size( ) - 1;

			if (toDelete != last) {

				m_Dense[ toDelete ] = m_Dense[ last ];
				SparseT moved = m_DenseToSparse[ last ];
				m_Sparse[ moved ] = toDelete;
				m_DenseToSparse[ toDelete ] = moved;

			}

			m_Dense.pop_back( );
			m_DenseToSparse.pop_back( );

			m_Sparse[ idx ] = sk_NullSparse;

		}

		/* @brief Checks whether a value exists at the given sparse key.
		* @param idx Sparse key to check.
		* @return True if the key is occupied.
		*/
		inline constexpr bool Contains( SparseT idx ) const noexcept {
			return idx < k_MaxSize && m_Sparse[ idx ] != sk_NullSparse;
		}

	private:

		static constexpr SparseT sk_NullSparse = MaxValue<SparseT>( ); // Sentinel value for empty slots.
		const SparseT k_MaxSize = 0;

		std::vector<tDense>		m_Dense;         // Contiguous value storage.
		std::vector<SparseT>	m_Sparse;        // Maps sparse key → dense index.
		std::vector<SparseT>	m_DenseToSparse; // Maps dense index → sparse key.

		inline constexpr void init( SparseT size ) {
			m_Dense.reserve( size );
			m_DenseToSparse.reserve( size );

			m_Sparse.resize( size, sk_NullSparse );
		}

	};

}