//========= Copyright (C) 2025-present 3zymek, MIT License  ============//
//
// Purpose: Non-owning pointer wrapper with runtime safety assertions.
//
//=============================================================================//
#pragma once

#include "Core/Utils/Logger.hpp"
#include "Core/Utils/LumAssert.hpp"
#include "Core/CoreDefines.hpp"
#include "Core/Types.hpp"
#include "Core/Utils/OwningPtr.hpp"

namespace lum {

	/*
	* @brief Non-owning pointer wrapper providing safe access to an object.
	*
	* SafePtr does not manage the lifetime of the referenced object. It only
	* stores a raw pointer and provides runtime validation when accessing it.
	*
	* @tparam tType The type of the object being referenced.
	*/
	template<cNonPointer tType>
	class SafePtr {

		template<cNonPointer tOther>
		friend class SafePtr;

	public:

		// =========================================================================
		// Construction & Destruction
		// =========================================================================

		/*
		* @brief Constructs an empty SafePtr.
		*/
		SafePtr( ) : m_Value( nullptr ) {}

		/*
		* @brief Constructs a SafePtr from a raw pointer.
		*
		* @param val Pointer to the object to reference.
		*/
		SafePtr( tType* val ) : m_Value( val ) {}

		/*
		* @brief Constructs a SafePtr from an object reference.
		*
		* @param val Object to reference.
		*/
		SafePtr( tType& val ) : m_Value( &val ) {}

		/*
		* @brief Constructs a SafePtr by copying another SafePtr.
		*
		* @param val SafePtr whose referenced object will be copied.
		*/
		SafePtr( const SafePtr<tType>& val ) : m_Value( val.m_Value ) {}

		/*
		* @brief Constructs a SafePtr from a convertible pointer type.
		*
		* @tparam tOther Source pointer type.
		* @param val Pointer to the object to reference.
		*/
		template<cNonPointer tOther>
			requires(std::is_convertible_v<tOther*, tType*>)
		SafePtr( tOther* val ) : m_Value( val ) {}

		/*
		* @brief Constructs a SafePtr from another convertible SafePtr type.
		*
		* @tparam tOther Source SafePtr type.
		* @param val SafePtr whose referenced object will be copied.
		*/
		template<cNonPointer tOther>
			requires(std::is_convertible_v<tOther*, tType*>)
		SafePtr( const SafePtr<tOther>& val ) : m_Value( val.m_Value ) {}

		~SafePtr( ) = default;

		// =========================================================================
		// Primary Access Operator
		// =========================================================================

		/* @brief Accesses the referenced object.
		*
		* @return Reference to the referenced object.
		*
		* Asserts at runtime if the SafePtr is empty. The source location of the
		* access is included in the assertion message.
		* PLEASE USE THIS AS YOUR PRIMARY POINTER ACCESS FOR SAFETY
		*/
		LUM_NODISCARD tType& operator()( std::source_location loc = std::source_location::current( ) ) const {

			LUM_ASSERT(
				m_Value != nullptr,
				"Attempted to access nullptr (SafePtr) at {}: {} ({})",
				loc.file_name( ),
				loc.line( ),
				loc.function_name( )
			);

			return *m_Value;
		}

		// =========================================================================
		// Assignment Operators
		// =========================================================================

		SafePtr& operator=( tType* other ) {
			m_Value = other;
			return *this;
		}

		SafePtr& operator=( tType& other ) {
			m_Value = &other;
			return *this;
		}

		SafePtr& operator=( const SafePtr<tType>& other ) {
			m_Value = other.m_Value;
			return *this;
		}
		SafePtr& operator=( const OwningPtr<tType>& other ) {
			m_Value = other.Ptr( );
			return *this;
		}


		template<cNonPointer tOther>
			requires(std::is_convertible_v<tOther*, tType*>)
		SafePtr& operator=( const OwningPtr<tOther>& other ) {
			m_Value = other.Ptr( );
			return *this;
		}

		template<cNonPointer tOther>
			requires(std::is_convertible_v<tOther*, tType*>)
		SafePtr& operator=( tOther* other ) {
			m_Value = other;
			return *this;
		}

		template<cNonPointer tOther>
			requires(std::is_convertible_v<tOther*, tType*>)
		SafePtr& operator=( const SafePtr<tOther>& other ) {
			m_Value = other.m_Value;
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

		/* @brief Returns a reference to the referenced object.
		* @return Reference to the referenced object.
		* Asserts at runtime if the SafePtr is empty.
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

		/* @brief Clears the referenced pointer.
		* After calling Clear(), the SafePtr no longer references an object.
		*/
		void Clear( ) noexcept {
			m_Value = nullptr;
		}

		/* @brief Logs the underlying pointer for debugging purposes. */
		void PrintDebug( ) const noexcept {
			LUM_LOG_DEBUG( "{}", m_Value );
		}

	private:

		tType* m_Value = nullptr; // < Non-owning pointer to the referenced object.

	};

} // namespace lum