//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Dense buffer + sparse index map. O(1) insert/erase without holes.
//          Perfect for ECS component storage (fast random access by entity ID).
//
//=============================================================================//
#pragma once
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
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
	template<typename tSparse, typename tDense>
		requires std::is_arithmetic_v<tSparse>
	class SparseSet {

		using SparseT = tSparse;

	public:

		/* @brief Constructs the sparse set with a fixed maximum key capacity.
		* @param size Maximum sparse key value that can be stored.
		*/
		constexpr SparseSet(SparseT maxSize) : kMaxSize(maxSize) { init(maxSize); }
		~SparseSet( ) = default;


		// Standard iterators over dense value array.
		using Iterator = typename std::vector<tDense>::iterator;
		using ConstIterator = typename std::vector<tDense>::const_iterator;
		Iterator begin( ) { return mDense.begin(); }
		Iterator end( ) { return mDense.end(); }
		ConstIterator begin( ) const { return mDense.begin(); }
		ConstIterator end( ) const { return mDense.end(); }

		
		/* @brief Iterator that yields key-value pairs during iteration.
		*
		* Allows range-based for loops with structured bindings:
		* for (auto [key, value] : set.Each()) { ... }
		*/
		struct KeyValIterator {

			tDense*		mDense;          // Pointer to dense value array.
			SparseT*	mDenseToSparse;  // Pointer to key array parallel to dense.
			SparseT		mIndex;          // Current position in dense array.

			/* @brief Returns the current key-value pair.
			* @return Pair of (sparse key, pointer to dense value).
			*/
			std::pair<SparseT, tDense*> operator*( ) {
				return { mDenseToSparse[mIndex], &mDense[mIndex] };
			}
			KeyValIterator& operator++( ) { mIndex++; return *this; }
			bool operator!=( const KeyValIterator& other ) { return mIndex != other.mIndex; }

		};

		/* @brief Range wrapper returned by Each(), enables range-based for. */
		struct KeyValRange {
			KeyValIterator mBegin;
			KeyValIterator mEnd;
			KeyValIterator begin( ) { return mBegin; }
			KeyValIterator end( ) { return mEnd; }
		};

		/* @brief Returns a key-value range for structured binding iteration.
		* @return KvRange over all currently stored key-value pairs.
		*/
		KeyValRange Each( ) {
			return {
				{ mDense.data(), mDenseToSparse.data(), 0 },
				{ mDense.data(), mDenseToSparse.data(), static_cast<SparseT>(mDense.size()) }
			};
		}


		/* @brief Returns value at sparse index. No bounds checking.
		* @param idx Sparse key to look up.
		*/
		inline constexpr tDense& operator[]( SparseT idx ) {
			return mDense[mSparse[idx]];
		}

		/* @brief Returns value at sparse index with bounds and null checking.
		* @param idx Sparse key to look up.
		*/
		constexpr tDense* Get( SparseT idx ) {

			if ((idx > kMaxSize) && (mSparse[idx] == skNullSparse)) return nullptr;

			return &mDense[mSparse[idx]];
		}

		/* @brief Inserts a value at the given sparse key.
		* Does nothing if the key is out of range or already occupied.
		* @param value Value to insert (forwarded).
		* @param idx   Sparse key to insert at.
		*/
		constexpr void Append( tDense value, SparseT idx ) {

			if ((idx >= kMaxSize) || (mSparse[idx] != skNullSparse)) return;

			mDense.emplace_back(std::move(value));
			mSparse[idx] = mDense.size() - 1;
			mDenseToSparse.push_back(idx);

		}

		inline constexpr SparseT MaxSize	( )	const noexcept { return kMaxSize; }   // Maximum sparse capacity.
		inline constexpr SparseT DenseSize	( )	const noexcept { return mDense.size(); } // Number of stored elements.
		inline constexpr bool    DenseEmpty	( )	const noexcept { return mDense.empty(); }

		/* @brief Clears all stored values and resets the container.
		* Sparse array is refilled with null sentinels after clear.
		*/
		inline constexpr void Clear( ) {

			mDense.clear();
			mSparse.clear();
			mDenseToSparse.clear();

			init(kMaxSize);

		}

		/* @brief Removes the value at the given sparse key.
		* Uses swap-with-last to maintain dense packing. O(1).
		* @param idx Sparse key to remove.
		*/
		void Remove( SparseT idx ) {

			if ((idx > kMaxSize) && (mSparse[idx] == skNullSparse)) return;

			SparseT toDelete = mSparse[idx];
			SparseT last = mDense.size() - 1;

			if (toDelete != last) {

				mDense[toDelete] = mDense[last];
				SparseT moved = mDenseToSparse[last];
				mSparse[moved] = toDelete;
				mDenseToSparse[toDelete] = moved;

			}

			mDense.pop_back();
			mDenseToSparse.pop_back();

			mSparse[idx] = skNullSparse;

		}

		/* @brief Checks whether a value exists at the given sparse key.
		* @param idx Sparse key to check.
		* @return True if the key is occupied.
		*/
		inline constexpr bool Contains( SparseT idx ) const noexcept {
			return idx < kMaxSize && mSparse[idx] != skNullSparse;
		}

	private:

		static constexpr SparseT skNullSparse = MaxVal<SparseT>(); // Sentinel value for empty slots.
		const SparseT kMaxSize = 0;

		std::vector<tDense>		mDense;         // Contiguous value storage.
		std::vector<SparseT>	mSparse;        // Maps sparse key → dense index.
		std::vector<SparseT>	mDenseToSparse; // Maps dense index → sparse key.

		inline constexpr void init( SparseT size ) {
			mDense.reserve(size);
			mDenseToSparse.reserve(size);

			mSparse.resize(size, skNullSparse);
		}

	};

}