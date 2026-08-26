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

		/* @brief Slot index identifying the object location.
		*  MaxValue indicates an invalid handle.
		*/
		tType m_Id{};

		/* @brief Generation value used to invalidate old handles after removal. */
		tType m_Generation{};

		/* @brief Constructs a handle with a slot ID and generation.
		* @param id Slot identifier.
		* @param gen Generation counter.
		*/
		constexpr BaseHandle( tType id, tType gen = 0 ) : m_Id( id ), m_Generation( gen ) {}

		/* @brief Constructs an invalid handle. */
		constexpr BaseHandle( ) : m_Id( MaxValue<tType>( ) ), m_Generation( 0 ) {}

		/* @brief Compares two handles.
		* @param other Handle to compare against.
		* @return True if both ID and generation match.
		*/
		constexpr bool operator==( const BaseHandle& other ) const noexcept {
			return m_Id == other.m_Id && m_Generation == other.m_Generation;
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
		constexpr HandlePool( SparseT maxSize ) : k_MaxSize( maxSize ) {
			init( maxSize );
		}

		/* @brief Default destructor. */
		~HandlePool( ) = default;


		/* @brief Iterator type over dense object storage. */
		using Iterator = typename std::vector<tValue>::iterator;

		/* @brief Const iterator type over dense object storage. */
		using ConstIterator = typename std::vector<tValue>::const_iterator;


		/* @brief Returns iterator to the first stored object. */
		Iterator begin( ) { return m_Dense.begin( ); }

		/* @brief Returns iterator past the last stored object. */
		Iterator end( ) { return m_Dense.end( ); }

		/* @brief Returns const iterator to the first stored object. */
		ConstIterator begin( ) const { return m_Dense.begin( ); }

		/* @brief Returns const iterator past the last stored object. */
		ConstIterator end( ) const { return m_Dense.end( ); }


		/* @brief Iterator yielding handle and object pairs.
		*
		* Enables iteration using structured bindings:
		* for (auto [handle, value] : pool.Each()) { }
		*/
		struct HandleIterator {

			/* @brief Pointer to dense object storage. */
			tValue* m_Dense;

			/* @brief Mapping from dense indices to slot indices. */
			SparseT* m_DenseToSparse;

			/* @brief Generation counters for each slot. */
			GenerationT* m_Generations;

			/* @brief Current dense array index. */
			SparseT m_Index;


			/* @brief Returns the current handle and object pointer pair.
			* @return Pair containing handle and object address.
			*/
			std::pair<tHandle, tValue*> operator*( ) {
				SparseT slot = m_DenseToSparse[ m_Index ];

				tHandle handle;
				handle.m_Id = slot;
				handle.m_Generation = m_Generations[ slot ];

				return { handle, &m_Dense[ m_Index ] };
			}

			HandleIterator& operator++( ) {
				m_Index++;
				return *this;
			}

			bool operator!=( HandleIterator other ) {
				return m_Index != other.m_Index;
			}
		};


		/* @brief Range object used by Each() for range-based iteration. */
		struct HandleRange {

			/* @brief Beginning iterator. */
			HandleIterator m_Begin{};

			/* @brief Ending iterator. */
			HandleIterator m_End{};


			HandleIterator begin( ) {
				return m_Begin;
			}

			HandleIterator end( ) {
				return m_End;
			}
		};


		/* @brief Returns an iterable range containing handles and objects.
		* @return Range for structured binding iteration.
		*/
		HandleRange Each( ) {
			return {
				{ m_Dense.data( ), m_DenseToSlot.data( ), m_Generations.data( ), 0 },
				{ m_Dense.data( ), m_DenseToSlot.data( ), m_Generations.data( ), static_cast<SparseT>(m_Dense.size( )) }
			};
		}


		/* @brief Accesses an object by slot index.
		* No validity checking is performed.
		* @param id Slot identifier.
		* @return Reference to stored object.
		*/
		inline constexpr tValue& operator[]( SparseT id ) {
			return m_Dense[ m_SlotToDense[ id ] ];
		}


		/* @brief Accesses an object using a handle.
		* No validity checking is performed.
		* @param handle Object handle.
		* @return Reference to stored object.
		*/
		inline constexpr tValue& operator[]( tHandle handle ) {
			return m_Dense[ m_SlotToDense[ handle.m_Id ] ];
		}


		/* @brief Retrieves an object by handle.
		* @param handle Handle to lookup.
		* @return Pointer to object or nullptr if handle is invalid.
		*/
		tValue* Get( tHandle handle ) {
			if (Contains( handle ))
				return &m_Dense[ m_SlotToDense[ handle.m_Id ] ];

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

			if (m_Dense.size( ) >= k_MaxSize)
				throw std::runtime_error( "Handle pool full" );

			SparseT slot;

			if (!m_FreeSlots.empty( )) {
				slot = m_FreeSlots.back( );
				m_FreeSlots.pop_back( );
			}
			else {
				slot = static_cast<SparseT>(m_NextSlot++);
			}

			m_Dense.emplace_back( std::move( value ) );

			SparseT lastDense = m_Dense.size( ) - 1;

			m_SlotToDense[ slot ] = static_cast<SparseT>(lastDense);
			m_DenseToSlot.push_back( slot );

			tHandle handle;
			handle.m_Id = slot;
			handle.m_Generation = m_Generations[ slot ];

			return handle;
		}


		/* @brief Returns maximum pool capacity. */
		inline constexpr SparseT MaxSize( ) const {
			return k_MaxSize;
		}

		/* @brief Returns number of currently stored objects. */
		inline constexpr SparseT DenseSize( ) const {
			return m_Dense.size( );
		}

		/* @brief Checks whether the pool contains no objects. */
		inline constexpr bool DenseEmpty( ) const {
			return m_Dense.empty( );
		}


		/* @brief Removes all objects and resets the pool.
		*
		* All previously created handles become invalid.
		*/
		inline constexpr void Clear( ) {

			m_Dense.clear( );
			m_SlotToDense.clear( );
			m_DenseToSlot.clear( );
			m_FreeSlots.clear( );
			m_Generations.clear( );

			init( k_MaxSize );

			m_NextSlot = 0;
		}


		/* @brief Removes an object using a handle.
		*
		* Uses swap-with-last removal to keep dense storage contiguous.
		* Invalidates existing handles to the removed object.
		*
		* @param handle Handle of the object to remove.
		*/
		void Remove( tHandle handle ) {

			SparseT slot = static_cast<SparseT>(handle.m_Id);

			if (slot >= k_MaxSize)
				return;

			if (m_Generations[ slot ] != handle.m_Generation)
				return;


			SparseT denseIndex = m_SlotToDense[ slot ];
			SparseT lastIndex = m_Dense.size( ) - 1;


			if (denseIndex != lastIndex) {

				std::swap( m_Dense[ denseIndex ], m_Dense[ lastIndex ] );

				SparseT movedSlot = m_DenseToSlot[ lastIndex ];

				m_SlotToDense[ movedSlot ] = static_cast<SparseT>(denseIndex);
				m_DenseToSlot[ denseIndex ] = movedSlot;
			}


			m_Dense.pop_back( );
			m_DenseToSlot.pop_back( );

			m_Generations[ slot ]++;
			m_FreeSlots.push_back( slot );

			m_SlotToDense[ slot ] = sk_NullSlot;
		}


		/* @brief Checks whether a handle references a valid object.
		* @param handle Handle to validate.
		* @return True if handle is active and generation matches.
		*/
		constexpr inline bool Contains( tHandle handle ) const noexcept {
			return (handle.m_Id < m_Generations.size( )) &&
				(handle.m_Generation == m_Generations[ handle.m_Id ]);
		}


	private:

		/* @brief Invalid slot sentinel value. */
		static constexpr SparseT sk_NullSlot = MaxValue<SparseT>( );

		/* @brief Maximum number of stored objects. */
		const SparseT k_MaxSize = 0;

		/* @brief Next unused slot index. */
		SparseT m_NextSlot = 0;


		/* @brief Dense object storage. */
		std::vector<tValue> m_Dense{};

		/* @brief Maps slot indices to dense array indices. */
		std::vector<SparseT> m_SlotToDense{};

		/* @brief Maps dense array indices to slot indices. */
		std::vector<SparseT> m_DenseToSlot{};

		/* @brief Generation counters for each slot. */
		std::vector<SparseT> m_Generations{};

		/* @brief Free slot indices available for reuse. */
		std::vector<SparseT> m_FreeSlots{};


		/* @brief Initializes internal storage arrays.
		* @param size Capacity of the pool.
		*/
		inline constexpr void init( SparseT size ) {

			m_Dense.reserve( size );
			m_FreeSlots.reserve( size );
			m_DenseToSlot.reserve( size );

			m_SlotToDense.resize( size, sk_NullSlot );
			m_Generations.resize( size, 0 );
		}

	};

} // namespace lum