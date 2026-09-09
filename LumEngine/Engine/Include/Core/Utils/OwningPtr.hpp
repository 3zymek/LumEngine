//========= Copyright (C) 2025-present 3zymek, MIT License  ============//
//
// Purpose: Owning pointer wrapper with runtime safety assertions.
//
//=============================================================================//
#pragma once

#include "Core/CoreCommon.hpp"

namespace lum {

	namespace detail {

		 // @brief Verifies that tDestructor provides a static Destroy(tObj*) method returning void without throwing.
		template<typename tObj, typename tDestructor>
		concept cValidDestructor = requires(tObj * obj) {
			{ tDestructor::Destroy( obj ) } noexcept -> std::same_as<void>;
		};

		// @brief Ensures safe pointer conversion between derived/base types while validating matching destructors.
		template<typename tFirst, typename tFirstDestructor, typename tOther, typename tOtherDestructor>
		concept cDerivedConvertion = (
			!std::same_as<tFirst, tOther> &&
			std::is_convertible_v<tOther*, tFirst*> &&
			cValidDestructor<tOther, tOtherDestructor>
		);
	}

	/*@brief Default policy for object destruction using standard C++ 'delete'.
	* @tparam tType Type of object to be deleted.
	*/
	template<typename tType>
	class DefaultOwningDestructor {
	public:
		static void Destroy( tType* ptr ) noexcept {
			delete ptr;
		}
	};

	/* @brief Explicit unique-ownership smart pointer with custom destruction policy and source-location bounds assertions.
	*
	* Prevents implicit copies and enforces source-tracked nullptr checking via functional access operator().
	*
	* @tparam tType Managed object type (must not be a raw pointer).
	* @tparam tDestructor Policy class defining a static Destroy(tType*) method.
	*/
	template<cNonPointer tType, typename tDestructor = DefaultOwningDestructor<tType>>
		requires detail::cValidDestructor<tType, tDestructor>
	class OwningPtr {

		template<cNonPointer tOther, typename tOtherDestructor>
			requires detail::cValidDestructor<tOther, tOtherDestructor>
		friend class OwningPtr;

	public:

		// =========================================================================
		// Construction & Destruction
		// =========================================================================

		template<typename... tArgs>
		static OwningPtr Create( tArgs&&... args ) {
			return OwningPtr( new tType( std::forward<tArgs>( args )... ) );
		}

		OwningPtr( ) noexcept : m_Value( nullptr ) {}
		OwningPtr( std::nullptr_t ) noexcept : m_Value( nullptr ) {}

		OwningPtr( OwningPtr&& other ) noexcept : m_Value( other.m_Value ) {
			other.m_Value = nullptr;
		}

		template<cNonPointer tOther, typename tOtherDestructor>
			requires detail::cDerivedConvertion<tType, tDestructor, tOther, tOtherDestructor>
		OwningPtr( OwningPtr<tOther, tOtherDestructor>&& other ) noexcept : m_Value( other.Release() ) {}

		~OwningPtr( ) {
			Clear( );
		}

		// Delete copying by value
		OwningPtr( const OwningPtr& ) = delete;
		OwningPtr& operator=( const OwningPtr& ) = delete;

		// =========================================================================
		// Primary Access Operator
		// =========================================================================

		/* @brief Accesses the owned object.
		*
		* @return Reference to the owned object.
		*
		* Asserts at runtime if the OwningPtr is empty. The source location of the
		* access is included in the assertion message.
		* PLEASE USE THIS AS YOUR PRIMARY POINTER ACCESS FOR SAFETY
		*/
		LUM_NODISCARD tType& operator()( std::source_location loc = std::source_location::current( ) ) const {

			LUM_ASSERT(
				m_Value != nullptr,
				"Attempted to access nullptr (OwningPtr) at {}: {} ({})",
				loc.file_name( ),
				loc.line( ),
				loc.function_name( )
			);
			return *m_Value;

		}

		/* @brief Releases ownership of the managed object without destroying it.
		*
		* @return Raw pointer to the managed object, or nullptr if empty.
		*
		* Resets the internal pointer to nullptr. The caller assumes full responsibility
		* for managing the life cycle and destruction of the returned pointer.
		*/
		LUM_NODISCARD tType* Release( ) noexcept {
			tType* ptr = m_Value;
			m_Value = nullptr;
			return ptr;
		}

		// =========================================================================
		// Assignment Operators
		// =========================================================================

		OwningPtr& operator=( OwningPtr&& other ) noexcept {

			if (this != &other) {

				Clear( );
				m_Value = other.m_Value;
				other.m_Value = nullptr;

			}
			return *this;

		}

		template<cNonPointer tOther, typename tOtherDestructor>
			requires detail::cDerivedConvertion<tType, tDestructor, tOther, tOtherDestructor>
		OwningPtr& operator=( OwningPtr<tOther, tOtherDestructor>&& other ) noexcept {

			if (static_cast<const void*>(this) != static_cast<const void*>(&other)) {
				Clear( );
				m_Value = other.m_Value;
				other.m_Value = nullptr;
			}

			return *this;

		}

		OwningPtr& operator=( std::nullptr_t ) noexcept {
			Clear( );
			return *this;
		}

		// =========================================================================
		// Accessors & Utility
		// =========================================================================

		/* @brief Returns the underlying raw pointer.
		* @return Pointer to the referenced object, or nullptr if empty.
		*/
		LUM_NODISCARD tType* Ptr( ) const noexcept {
			return m_Value;
		}

		/* @brief Returns a reference to the pointed object.
		* @return Reference to the pointed object.
		* Asserts at runtime if the OwningPtr is empty.
		*/
		LUM_NODISCARD tType& Ref( std::source_location loc = std::source_location::current( ) ) const {
			return (*this)(loc);
		}

		/* @brief Returns if Ptr != nullptr */
		explicit operator bool( ) const noexcept {
			return m_Value != nullptr;
		}

		bool operator==( const void* other ) const noexcept {
			return (m_Value == other);
		}

		bool operator!=( const void* other ) const noexcept {
			return !((*this) == other);
		}

		/* @brief Deletes pointed object and clears the owning pointer.
		* After calling Clear(), the OwningPtr no longer owns an object.
		*/
		void Clear( ) noexcept {
			if (m_Value)
				tDestructor::Destroy( m_Value );
			m_Value = nullptr;
		}

		/* @brief Logs the underlying pointer for debugging purposes. */
		void PrintDebug( ) const noexcept {
			LUM_LOG_DEBUG( "{}", m_Value );
		}

	private:

		// Protected pointer constructor for safety
		OwningPtr( tType* ptr ) noexcept : m_Value( ptr ) {}

		tType* m_Value = nullptr;

	};

}