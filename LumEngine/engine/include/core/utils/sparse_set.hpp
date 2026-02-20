//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Dense buffer + sparse index map. O(1) insert/erase without holes.
//          Perfect for ECS component storage (fast random access by entity ID).
//
//=============================================================================//
#pragma once
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
namespace cstd {


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
		constexpr SparseSet(const SparseT& size) : mSparseSize(size) { Reserve(size); ResizeSparse(size); }
		~SparseSet() = default;


		// Standard iterators over dense value array.
		using Iterator = typename std::vector<tDense>::iterator;
		using ConstIterator = typename std::vector<tDense>::const_iterator;
		Iterator begin() { return mDense.begin(); }
		Iterator end() { return mDense.end(); }
		ConstIterator begin() const { return mDense.begin(); }
		ConstIterator end() const { return mDense.end(); }

		
		/* @brief Iterator that yields key-value pairs during iteration.
		*
		* Allows range-based for loops with structured bindings:
		* for (auto [key, value] : set.Each()) { ... }
		*/
		struct KvIterator {

			tDense*		mDense;          // Pointer to dense value array.
			SparseT*	mDenseToSparse;  // Pointer to key array parallel to dense.
			SparseT		mIndex;          // Current position in dense array.

			/* @brief Returns the current key-value pair.
			* @return Pair of (sparse key, pointer to dense value).
			*/
			std::pair<SparseT, tDense*> operator*() {
				return { mDenseToSparse[mIndex], &mDense[mIndex] };
			}
			KvIterator& operator++() { mIndex++; return *this; }
			bool operator!=(const KvIterator& other) { return mIndex != other.mIndex; }

		};

		/* @brief Range wrapper returned by Each(), enables range-based for. */
		struct KvRange {
			KvIterator mBegin;
			KvIterator mEnd;
			KvIterator begin() { return mBegin; }
			KvIterator end() { return mEnd; }
		};

		/* @brief Returns a key-value range for structured binding iteration.
		* @return KvRange over all currently stored key-value pairs.
		*/
		KvRange Each() {
			return {
				{ mDense.data(), mDenseToSparse.data(), 0 },
				{ mDense.data(), mDenseToSparse.data(), static_cast<SparseT>(mDense.size()) }
			};
		}


		/* @brief Returns value at sparse index. No bounds checking.
		* @param idx Sparse key to look up.
		*/
		constexpr tDense* operator[](SparseT idx) {
			return &mDense[mSparse[idx]];
		}

		/* @brief Returns value at sparse index with bounds and null checking.
		* @param idx Sparse key to look up.
		*/
		constexpr tDense* GetAt(SparseT idx) {
			if (idx > mSparseSize) return nullptr;
			if (mSparse[idx] == skNullSparse) return nullptr;
			return &mDense[mSparse[idx]];
		}

		/* @brief Inserts a value at the given sparse key.
		* Does nothing if the key is out of range or already occupied.
		* @param value Value to insert (forwarded).
		* @param idx   Sparse key to insert at.
		*/
		constexpr void Append(tDense& value, SparseT idx) {
			if ((idx >= mSparseSize) or (mSparse[idx] != skNullSparse)) return;

			mDense.emplace_back(std::forward<tDense>(value));
			mSparse[idx] = mDense.size() - 1;
			mDenseToSparse.push_back(idx);

			mDenseSize++;
		}

		/* @brief Reserves memory for all internal arrays.
		* @param size Number of elements to reserve space for.
		*/
		constexpr void ResizeSparse(SparseT size) noexcept {
			mSparse.resize(size);
			mDenseToSparse.resize(size);
		}
		/* @brief Fills sparse array with null sentinel values. */
		constexpr void Reserve(SparseT size) noexcept {
			mDense.reserve(size);
			mSparse.reserve(size);
			mDenseToSparse.reserve(size);
			mSparseSize = size;
		}
		inline constexpr SparseT Size()			{ return mSparseSize; }   // Maximum sparse capacity.
		inline constexpr SparseT DenseSize()	{ return mDense.size(); } // Number of stored elements.
		inline constexpr bool    Empty()		{ return mDense.empty(); }

		/* @brief Clears all stored values and resets the container. */
		inline constexpr void Clear() {
			mDense.clear();
			mSparse.clear();
			mDenseToSparse.clear();
		}

		/* @brief Removes the value at the given sparse key.
		* Uses swap-with-last to maintain dense packing. O(1).
		* @param idx Sparse key to remove.
		*/
		void Remove(SparseT idx) {
			if (idx > mSparseSize) return;
			if (mSparse[idx] == skNullSparse) return;
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
		constexpr bool Contains(SparseT idx) {
			return idx < mSparseSize and mSparse[idx] != skNullSparse;
		}

	private:

		static constexpr SparseT skNullSparse = lum::MaxVal<SparseT>(); // Sentinel value for empty slots.
		SparseT mSparseSize = 0; // Maximum key capacity.
		SparseT mDenseSize = 0; // Current number of stored elements.

		std::vector<tDense>		mDense;         // Contiguous value storage.
		std::vector<SparseT>	mSparse;        // Maps sparse key → dense index.
		std::vector<SparseT>	mDenseToSparse; // Maps dense index → sparse key.

	};

}