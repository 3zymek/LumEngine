//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Custom data structure to storage objects by handles
// 
//=============================================================================//
#pragma once
#include "Core/CoreCommon.hpp"

namespace lum::cstd {

	/* @brief Base type for all handle types.
	*
	* Stores a slot ID and a generation counter to detect use-after-free.
	* Two handles are equal only if both ID and generation match.
	*
	* @tparam tType Unsigned arithmetic type used for ID and generation.
	*/
	template<std::unsigned_integral tType = uint32>
	struct alignas(8) BaseHandle {

		constexpr BaseHandle( tType id, tType gen = 0 ) : mId( id ), mGeneration( gen ) { }
		constexpr BaseHandle( ) : mId( MaxValue<tType>( ) ), mGeneration( 0 ) { }

		tType mId; // Slot index. MaxVal means invalid/null.
		tType mGeneration; // Incremented on Remove to invalidate old handles.

		constexpr bool operator==( const BaseHandle& other ) const noexcept {
			return mId == other.mId && mGeneration == other.mGeneration;
		}
		constexpr bool operator!=( const BaseHandle& other ) const noexcept {
			return !(*this == other);
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
	template<typename tHandle, typename tValue, std::unsigned_integral tArithmetic = uint32>
	class HandlePool {

		using GenerationT = tArithmetic;
		using SparseT = tArithmetic;

	public:

		/* @brief Constructs the pool with a fixed maximum capacity.
		* @param maxSize Maximum number of objects that can be stored simultaneously.
		*/
		constexpr HandlePool( SparseT maxSize ) : mkMaxSize( maxSize ) { init( maxSize ); }
		~HandlePool( ) = default;

		// Standard iterators over dense value array.
		using Iterator = typename std::vector<tValue>::iterator;
		using ConstIterator = typename std::vector<tValue>::const_iterator;
		Iterator begin( ) { return mDense.begin( ); }
		Iterator end( ) { return mDense.end( ); }
		ConstIterator begin( ) const { return mDense.begin( ); }
		ConstIterator end( ) const { return mDense.end( ); }

		/* @brief Iterator that yields slot-value pairs during iteration.
		*
		* Allows range-based for loops with structured bindings:
		* for (auto [slot, value] : pool.Each()) { ... }
		*/
		struct HandleIterator {
			tValue*			mDense;          // Pointer to dense value array.
			SparseT*		mDenseToSparse;  // Pointer to slot index array parallel to dense.
			GenerationT*	mGenerations;
			SparseT			mIndex;          // Current position in dense array.

			/* @brief Returns the current slot-value pair.
			* @return Pair of (slot index, pointer to dense value).
			*/
			std::pair<tHandle, tValue*> operator*( ) {
				SparseT slot = mDenseToSparse[ mIndex ];
				tHandle handle;
				handle.mId = slot;
				handle.mGeneration = mGenerations[ slot ];
				return { handle, &mDense[ mIndex ] };
			}
			HandleIterator& operator++( ) { mIndex++; return *this; }
			bool operator!=( HandleIterator other ) { return mIndex != other.mIndex; }
		};

		/* @brief Range wrapper returned by Each(), enables range-based for. */
		struct HandleRange {
			HandleIterator mBegin;
			HandleIterator mEnd;
			HandleIterator begin( ) { return mBegin; }
			HandleIterator end( ) { return mEnd; }
		};

		/* @brief Returns a slot-value range for structured binding iteration.
		* @return KeyValRange over all currently stored slot-value pairs.
		*/
		HandleRange Each( ) {
			return {
				{ mDense.data( ), mDenseToSlot.data( ), mGenerations.data( ), 0 },
				{ mDense.data( ), mDenseToSlot.data( ), mGenerations.data( ), static_cast< SparseT >(mDense.size( )) }
			};
		}

		/* @brief Returns object at slot index. No bounds or validity checking.
		* @param id Slot index to look up.
		*/
		inline constexpr tValue& operator[]( SparseT id ) {
			return mDense[ mSlotToDense[ id ] ];
		}

		/* @brief Returns object by handle. No validity checking.
		* @param handle Handle to look up.
		*/
		inline constexpr tValue& operator[]( tHandle handle ) {
			return mDense[ mSlotToDense[ handle.mId ] ];
		}

		/* @brief Returns pointer to object by handle, or nullptr if handle is invalid.
		* @param handle Handle to look up.
		* @return Pointer to object, or nullptr if handle is stale or out of range.
		*/
		tValue* Get( tHandle handle ) {
			if (Contains( handle ))
				return &mDense[ mSlotToDense[ handle.mId ] ];
			else
				return nullptr;
		}

		/* @brief Inserts an object into the pool and returns its handle.
		* Reuses a previously freed slot if available, otherwise allocates a new one.
		* @param value Object to insert (moved into the pool).
		* @return Handle to the inserted object.
		* @throws std::runtime_error if the pool is full.
		*/
		constexpr tHandle Append( tValue value ) {

			if (mDense.size( ) >= mkMaxSize)
				throw std::runtime_error( "Handle pool full" );

			SparseT slot;

			if (!mFreeSlots.empty( )) {
				slot = mFreeSlots.back( );
				mFreeSlots.pop_back( );
			}
			else
				slot = static_cast< SparseT >(mNextSlot++);

			mDense.emplace_back( std::move( value ) );

			SparseT lastDense = mDense.size( ) - 1;

			mSlotToDense[ slot ] = static_cast< SparseT >(lastDense);
			mDenseToSlot.push_back( slot );

			tHandle handle;
			handle.mId = slot;
			handle.mGeneration = mGenerations[ slot ];

			return handle;

		}

		inline constexpr SparseT MaxSize( )    const { return mkMaxSize; }    // Maximum slot capacity.
		inline constexpr SparseT DenseSize( )  const { return mDense.size( ); } // Current number of stored objects.
		inline constexpr bool    DenseEmpty( ) const { return mDense.empty( ); }

		/* @brief Removes all objects and resets the pool to its initial state.
		* All existing handles become invalid after this call.
		*/
		inline constexpr void Clear( ) {

			mDense.clear( );
			mSlotToDense.clear( );
			mDenseToSlot.clear( );
			mFreeSlots.clear( );
			mGenerations.clear( );

			init( mkMaxSize );

			mNextSlot = 0;

		}

		/* @brief Removes the object referenced by the given handle.
		* Increments the generation counter to invalidate existing handles to this slot.
		* Uses swap-with-last to maintain dense packing. O(1).
		* @param handle Handle to the object to remove.
		*/
		void Remove( tHandle handle ) {

			SparseT slot = static_cast< SparseT >(handle.mId);

			if (slot >= mkMaxSize) return;
			if (mGenerations[ slot ] != handle.mGeneration) return;

			SparseT denseIndex = mSlotToDense[ slot ];
			SparseT lastIndex = mDense.size( ) - 1;

			if (denseIndex != lastIndex) {

				std::swap( mDense[ denseIndex ], mDense[ lastIndex ] );

				SparseT movedSlot = mDenseToSlot[ lastIndex ];
				mSlotToDense[ movedSlot ] = static_cast< SparseT >(denseIndex);
				mDenseToSlot[ denseIndex ] = static_cast< SparseT >(movedSlot);


			}

			mDense.pop_back( );
			mDenseToSlot.pop_back( );

			mGenerations[ slot ]++;
			mFreeSlots.push_back( slot );

			mSlotToDense[ slot ] = skNullSlot;

		}

		/* @brief Checks whether a handle is valid and points to a live object.
		* @param handle Handle to validate.
		* @return True if the handle is valid and the object exists.
		*/
		constexpr inline bool Contains( tHandle handle ) const noexcept {
			return (handle.mId < mGenerations.size( )) && (handle.mGeneration == mGenerations[ handle.mId ]);
		}

	private:

		static constexpr SparseT skNullSlot = MaxValue<SparseT>( ); // Sentinel value for empty slots.
		const SparseT mkMaxSize = 0; // Maximum capacity set at construction.

		SparseT mNextSlot = 0; // Next slot to allocate when free list is empty.

		std::vector<tValue>    mDense;         // Contiguous object storage.
		std::vector<SparseT>   mSlotToDense;   // Maps slot index → dense index.
		std::vector<SparseT>   mDenseToSlot; // Maps dense index → slot index.
		std::vector<SparseT>   mGenerations;   // Generation counter per slot.
		std::vector<SparseT>   mFreeSlots;     // Recycled slot indices available for reuse.

		/* @brief Initializes internal arrays to the given capacity. */
		inline constexpr void init( SparseT size ) {
			mDense.reserve( size );
			mFreeSlots.reserve( size );
			mDenseToSlot.reserve( size );

			mSlotToDense.resize( size, skNullSlot );
			mGenerations.resize( size, 0 );
		}


	};
} // namespace lum