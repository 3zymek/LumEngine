//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Defines generational handle-based containers for storing objects
//          using stable references, slot recycling and generation tracking.
//
//=============================================================================//
#pragma once

#include "Core/CoreCommon.hpp"

namespace lum::cstd {

	/* @brief Base type for all handle types.
	*
	* Stores a slot identifier and generation counter used to detect stale handles.
	* Handles are considered equal only when both values match.
	*
	* @tparam tType Unsigned arithmetic type used for ID and generation storage.
	*/
	template<std::unsigned_integral tType = uint32>
	struct alignas(8) BaseHandle {

		/* @brief Constructs a handle with a slot ID and generation.
		* @param id Slot identifier.
		* @param gen Generation counter.
		*/
		constexpr BaseHandle( tType id, tType gen = 0 ) : mId( id ), mGeneration( gen ) { }

		/* @brief Constructs an invalid handle. */
		constexpr BaseHandle( ) : mId( MaxValue<tType>( ) ), mGeneration( 0 ) { }

		/* @brief Slot index identifying the object location.
		*  MaxValue indicates an invalid handle.
		*/
		tType mId;

		/* @brief Generation value used to invalidate old handles after removal. */
		tType mGeneration;

		/* @brief Compares two handles.
		* @param other Handle to compare against.
		* @return True if both ID and generation match.
		*/
		constexpr bool operator==( const BaseHandle& other ) const noexcept {
			return mId == other.mId && mGeneration == other.mGeneration;
		}

		/* @brief Checks whether two handles differ.
		* @param other Handle to compare against.
		* @return True if either ID or generation differs.
		*/
		constexpr bool operator!=( const BaseHandle& other ) const noexcept {
			return !(*this == other);
		}

	};


	/* @brief Generational object storage accessed through typed handles.
	*
	* Stores objects in a dense contiguous array for cache-friendly iteration.
	* Handles contain slot IDs and generation counters to detect invalid references.
	* Removed objects are replaced using swap-with-last and slots are recycled.
	*
	* @tparam tHandle Handle type derived from BaseHandle.
	* @tparam tValue Stored object type.
	* @tparam tArithmetic Unsigned integer type used for indices and generations.
	*/
	template<typename tHandle, typename tValue, std::unsigned_integral tArithmetic = uint32>
	class HandlePool {

		using GenerationT = tArithmetic;
		using SparseT = tArithmetic;

	public:

		/* @brief Creates a handle pool with a fixed maximum capacity.
		* @param maxSize Maximum number of simultaneously stored objects.
		*/
		constexpr HandlePool( SparseT maxSize ) : mkMaxSize( maxSize ) {
			init( maxSize );
		}

		/* @brief Default destructor. */
		~HandlePool( ) = default;


		/* @brief Iterator type over dense object storage. */
		using Iterator = typename std::vector<tValue>::iterator;

		/* @brief Const iterator type over dense object storage. */
		using ConstIterator = typename std::vector<tValue>::const_iterator;


		/* @brief Returns iterator to the first stored object. */
		Iterator begin( ) { return mDense.begin( ); }

		/* @brief Returns iterator past the last stored object. */
		Iterator end( ) { return mDense.end( ); }

		/* @brief Returns const iterator to the first stored object. */
		ConstIterator begin( ) const { return mDense.begin( ); }

		/* @brief Returns const iterator past the last stored object. */
		ConstIterator end( ) const { return mDense.end( ); }


		/* @brief Iterator yielding handle and object pairs.
		*
		* Enables iteration using structured bindings:
		* for (auto [handle, value] : pool.Each()) { }
		*/
		struct HandleIterator {

			/* @brief Pointer to dense object storage. */
			tValue* mDense;

			/* @brief Mapping from dense indices to slot indices. */
			SparseT* mDenseToSparse;

			/* @brief Generation counters for each slot. */
			GenerationT* mGenerations;

			/* @brief Current dense array index. */
			SparseT mIndex;


			/* @brief Returns the current handle and object pointer pair.
			* @return Pair containing handle and object address.
			*/
			std::pair<tHandle, tValue*> operator*( ) {
				SparseT slot = mDenseToSparse[ mIndex ];

				tHandle handle;
				handle.mId = slot;
				handle.mGeneration = mGenerations[ slot ];

				return { handle, &mDense[ mIndex ] };
			}

			HandleIterator& operator++( ) {
				mIndex++;
				return *this;
			}

			bool operator!=( HandleIterator other ) {
				return mIndex != other.mIndex;
			}
		};


		/* @brief Range object used by Each() for range-based iteration. */
		struct HandleRange {

			/* @brief Beginning iterator. */
			HandleIterator mBegin;

			/* @brief Ending iterator. */
			HandleIterator mEnd;


			HandleIterator begin( ) {
				return mBegin;
			}

			HandleIterator end( ) {
				return mEnd;
			}
		};


		/* @brief Returns an iterable range containing handles and objects.
		* @return Range for structured binding iteration.
		*/
		HandleRange Each( ) {
			return {
				{ mDense.data( ), mDenseToSlot.data( ), mGenerations.data( ), 0 },
				{ mDense.data( ), mDenseToSlot.data( ), mGenerations.data( ), static_cast< SparseT >(mDense.size( )) }
			};
		}


		/* @brief Accesses an object by slot index.
		* No validity checking is performed.
		* @param id Slot identifier.
		* @return Reference to stored object.
		*/
		inline constexpr tValue& operator[]( SparseT id ) {
			return mDense[ mSlotToDense[ id ] ];
		}


		/* @brief Accesses an object using a handle.
		* No validity checking is performed.
		* @param handle Object handle.
		* @return Reference to stored object.
		*/
		inline constexpr tValue& operator[]( tHandle handle ) {
			return mDense[ mSlotToDense[ handle.mId ] ];
		}


		/* @brief Retrieves an object by handle.
		* @param handle Handle to lookup.
		* @return Pointer to object or nullptr if handle is invalid.
		*/
		tValue* Get( tHandle handle ) {
			if (Contains( handle ))
				return &mDense[ mSlotToDense[ handle.mId ] ];

			return nullptr;
		}


		/* @brief Inserts an object into the pool.
		*
		* Reuses previously freed slots when available.
		*
		* @param value Object to move into storage.
		* @return Handle referencing the inserted object.
		*/
		constexpr tHandle Append( tValue value ) {

			if (mDense.size( ) >= mkMaxSize)
				throw std::runtime_error( "Handle pool full" );

			SparseT slot;

			if (!mFreeSlots.empty( )) {
				slot = mFreeSlots.back( );
				mFreeSlots.pop_back( );
			}
			else {
				slot = static_cast< SparseT >(mNextSlot++);
			}

			mDense.emplace_back( std::move( value ) );

			SparseT lastDense = mDense.size( ) - 1;

			mSlotToDense[ slot ] = static_cast< SparseT >(lastDense);
			mDenseToSlot.push_back( slot );

			tHandle handle;
			handle.mId = slot;
			handle.mGeneration = mGenerations[ slot ];

			return handle;
		}


		/* @brief Returns maximum pool capacity. */
		inline constexpr SparseT MaxSize( ) const {
			return mkMaxSize;
		}

		/* @brief Returns number of currently stored objects. */
		inline constexpr SparseT DenseSize( ) const {
			return mDense.size( );
		}

		/* @brief Checks whether the pool contains no objects. */
		inline constexpr bool DenseEmpty( ) const {
			return mDense.empty( );
		}


		/* @brief Removes all objects and resets the pool.
		*
		* All previously created handles become invalid.
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


		/* @brief Removes an object using a handle.
		*
		* Uses swap-with-last removal to keep dense storage contiguous.
		* Invalidates existing handles to the removed object.
		*
		* @param handle Handle of the object to remove.
		*/
		void Remove( tHandle handle ) {

			SparseT slot = static_cast< SparseT >(handle.mId);

			if (slot >= mkMaxSize)
				return;

			if (mGenerations[ slot ] != handle.mGeneration)
				return;


			SparseT denseIndex = mSlotToDense[ slot ];
			SparseT lastIndex = mDense.size( ) - 1;


			if (denseIndex != lastIndex) {

				std::swap( mDense[ denseIndex ], mDense[ lastIndex ] );

				SparseT movedSlot = mDenseToSlot[ lastIndex ];

				mSlotToDense[ movedSlot ] = static_cast< SparseT >(denseIndex);
				mDenseToSlot[ denseIndex ] = movedSlot;
			}


			mDense.pop_back( );
			mDenseToSlot.pop_back( );

			mGenerations[ slot ]++;
			mFreeSlots.push_back( slot );

			mSlotToDense[ slot ] = skNullSlot;
		}


		/* @brief Checks whether a handle references a valid object.
		* @param handle Handle to validate.
		* @return True if handle is active and generation matches.
		*/
		constexpr inline bool Contains( tHandle handle ) const noexcept {
			return (handle.mId < mGenerations.size( )) &&
				(handle.mGeneration == mGenerations[ handle.mId ]);
		}


	private:

		/* @brief Invalid slot sentinel value. */
		static constexpr SparseT skNullSlot = MaxValue<SparseT>( );

		/* @brief Maximum number of stored objects. */
		const SparseT mkMaxSize = 0;

		/* @brief Next unused slot index. */
		SparseT mNextSlot = 0;


		/* @brief Dense object storage. */
		std::vector<tValue> mDense;

		/* @brief Maps slot indices to dense array indices. */
		std::vector<SparseT> mSlotToDense;

		/* @brief Maps dense array indices to slot indices. */
		std::vector<SparseT> mDenseToSlot;

		/* @brief Generation counters for each slot. */
		std::vector<SparseT> mGenerations;

		/* @brief Free slot indices available for reuse. */
		std::vector<SparseT> mFreeSlots;


		/* @brief Initializes internal storage arrays.
		* @param size Capacity of the pool.
		*/
		inline constexpr void init( SparseT size ) {

			mDense.reserve( size );
			mFreeSlots.reserve( size );
			mDenseToSlot.reserve( size );

			mSlotToDense.resize( size, skNullSlot );
			mGenerations.resize( size, 0 );
		}

	};

} // namespace lum