//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Custom data structure to storage objects by handles
// 
//=============================================================================//
#pragma once

#include "core/core_pch.hpp"
#include "core/core_common.hpp"

namespace lum::cstd {

	/* @brief Base type for all handle types.
	*
	* Stores a slot ID and a generation counter to detect use-after-free.
	* Two handles are equal only if both ID and generation match.
	*
	* @tparam tType Unsigned arithmetic type used for ID and generation.
	*/
	template<typename tType = uint32>
		requires std::is_arithmetic_v<tType> && std::is_unsigned_v<tType>
	struct alignas(8) BaseHandle {
		tType mID = lum::MaxVal<tType>();  // Slot index. MaxVal means invalid/null.
		tType mGeneration = 0;             // Incremented on Remove to invalidate old handles.

		bool operator==(const BaseHandle& other) const noexcept {
			return mID == other.mID && mGeneration == other.mGeneration;
		}
	};
	
	/* @brief Generational handle pool with stable references.
	*
	* Stores objects in a dense array for cache-friendly iteration.
	* Objects are accessed via typed handles that carry a generation counter,
	* which allows detecting dangling handles after removal.
	* Removed slots are recycled via a free list.
	*
	* @tparam tHandle      Handle type derived from BaseHandle.
	* @tparam tDense       Type of stored objects.
	* @tparam tArithmetic  Unsigned integer type used for indices and generations.
	*/
	template<typename tHandle, typename tDense, typename tArithmetic = uint32>
		requires std::is_arithmetic_v<tArithmetic> && std::is_unsigned_v<tArithmetic>
	class HandlePool {

		using GenerationT	= tArithmetic;
		using SparseT		= tArithmetic;

	public:

		/* @brief Constructs the pool with a fixed maximum capacity.
		* @param maxSize Maximum number of objects that can be stored simultaneously.
		*/
		constexpr HandlePool( SparseT maxSize ) : kMaxSize( maxSize ) { init(maxSize); }
		~HandlePool( ) = default;

		// Standard iterators over dense value array.
		using Iterator = typename std::vector<tDense>::iterator;
		using ConstIterator = typename std::vector<tDense>::const_iterator;
		Iterator begin( ) { return mDense.begin(); }
		Iterator end( ) { return mDense.end(); }
		ConstIterator begin( ) const { return mDense.begin(); }
		ConstIterator end( ) const { return mDense.end(); }
		
		/* @brief Iterator that yields slot-value pairs during iteration.
		*
		* Allows range-based for loops with structured bindings:
		* for (auto [slot, value] : pool.Each()) { ... }
		*/
		struct KeyValIterator {
			tDense* mDense;          // Pointer to dense value array.
			SparseT* mDenseToSparse;  // Pointer to slot index array parallel to dense.
			SparseT  mIndex;          // Current position in dense array.

			/* @brief Returns the current slot-value pair.
			* @return Pair of (slot index, pointer to dense value).
			*/
			std::pair<SparseT, tDense*> operator*() {
				return { mDenseToSparse[mIndex], &mDense[mIndex] };
			}
			KeyValIterator& operator++() { mIndex++; return *this; }
			bool operator!=(KeyValIterator other) { return mIndex != other.mIndex; }
		};

		/* @brief Range wrapper returned by Each(), enables range-based for. */
		struct KeyValRange {
			KeyValIterator mBegin;
			KeyValIterator mEnd;
			KeyValIterator begin() { return mBegin; }
			KeyValIterator end() { return mEnd; }
		};
		
		/* @brief Returns a slot-value range for structured binding iteration.
		* @return KeyValRange over all currently stored slot-value pairs.
		*/
		KeyValRange Each() {
			return {
				{ mDense.data(), mDenseToSparse.data(), 0 },
				{ mDense.data(), mDenseToSparse.data(), static_cast<SparseT>(mDense.size()) }
			};
		}

		/* @brief Returns object at slot index. No bounds or validity checking.
		* @param id Slot index to look up.
		*/
		inline constexpr tDense& operator[](SparseT id) {
			return mDense[mSparse[id]];
		}

		/* @brief Returns object by handle. No validity checking.
		* @param handle Handle to look up.
		*/
		inline constexpr tDense& operator[](tHandle handle) {
			return mDense[mSparse[handle.mID]];
		}

		/* @brief Returns pointer to object by handle, or nullptr if handle is invalid.
		* @param handle Handle to look up.
		* @return Pointer to object, or nullptr if handle is stale or out of range.
		*/
		tDense* Get( tHandle handle ) {
			if (Contains(handle))
				return &mDense[mSparse[handle.mID]];
			else
				return nullptr;
		} 

		/* @brief Inserts an object into the pool and returns its handle.
		* Reuses a previously freed slot if available, otherwise allocates a new one.
		* @param value Object to insert (moved into the pool).
		* @return Handle to the inserted object.
		* @throws std::runtime_error if the pool is full.
		*/
		constexpr tHandle Append( tDense value ) {

			if (mDense.size() >= kMaxSize)
				throw std::runtime_error("Handle pool full");

			SparseT slot;

			if (!mFreeSlots.empty()) {
				slot = mFreeSlots.back();
				mFreeSlots.pop_back();
			}
			else
				slot = static_cast<SparseT>(mNextSlot++);

			mDense.emplace_back(std::move(value));

			SparseT lastDense = mDense.size() - 1;

			mSparse[slot] = static_cast<SparseT>(lastDense);
			mDenseToSparse.push_back(slot);

			tHandle handle;
			handle.mID = slot;
			handle.mGeneration = mGenerations[slot];

			return handle;

		}

		inline constexpr SparseT MaxSize( )    const { return kMaxSize; }    // Maximum slot capacity.
		inline constexpr SparseT DenseSize( )  const { return mDense.size(); } // Current number of stored objects.
		inline constexpr bool    DenseEmpty( ) const { return mDense.empty(); }

		/* @brief Removes all objects and resets the pool to its initial state.
		* All existing handles become invalid after this call.
		*/
		inline constexpr void Clear( ) {

			mDense.clear();
			mSparse.clear();
			mDenseToSparse.clear();
			mFreeSlots.clear();
			mGenerations.clear();

			init(kMaxSize);

			mNextSlot = 0;

		}

		/* @brief Removes the object referenced by the given handle.
		* Increments the generation counter to invalidate existing handles to this slot.
		* Uses swap-with-last to maintain dense packing. O(1).
		* @param handle Handle to the object to remove.
		*/
		void Remove( tHandle handle ) {

			SparseT slot = static_cast<SparseT>(handle.mID);

			if (slot >= kMaxSize) return;
			if (mGenerations[slot] != handle.mGeneration) return;

			SparseT denseIndex = mSparse[slot];
			SparseT lastIndex = mDense.size() - 1;

			if (denseIndex != lastIndex) {

				std::swap(mDense[denseIndex], mDense[lastIndex]);

				SparseT movedSlot = mDenseToSparse[lastIndex];
				mSparse[movedSlot] = static_cast<SparseT>(denseIndex);
				mDenseToSparse[denseIndex] = static_cast<SparseT>(movedSlot);


			}

			mDense.pop_back();
			mDenseToSparse.pop_back();

			mGenerations[slot]++;
			mFreeSlots.push_back(slot);

			mSparse[slot] = skNullHandle;

		}

		/* @brief Checks whether a handle is valid and points to a live object.
		* @param handle Handle to validate.
		* @return True if the handle is valid and the object exists.
		*/
		constexpr inline bool Contains( tHandle handle ) const noexcept {
			return (handle.mID < mGenerations.size()) && (handle.mGeneration == mGenerations[handle.mID]);
		}

	private:

		static constexpr SparseT skNullHandle = MaxVal<SparseT>(); // Sentinel value for empty slots.
		const SparseT kMaxSize = 0; // Maximum capacity set at construction.
		
		SparseT mNextSlot = 0; // Next slot to allocate when free list is empty.

		std::vector<tDense>    mDense;         // Contiguous object storage.
		std::vector<SparseT>   mSparse;        // Maps slot index → dense index.
		std::vector<SparseT>   mDenseToSparse; // Maps dense index → slot index.
		std::vector<SparseT>   mGenerations;   // Generation counter per slot.
		std::vector<SparseT>   mFreeSlots;     // Recycled slot indices available for reuse.

		/* @brief Initializes internal arrays to the given capacity. */
		inline constexpr void init( SparseT size ) {
			mDense.reserve(size);
			mFreeSlots.reserve(size);
			mDenseToSparse.reserve(size);

			mSparse.resize(size, skNullHandle);
			mGenerations.resize(size, 0);
		}


	};
}