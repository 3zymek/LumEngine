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
		SafePtr( ) : mValue( nullptr ) {}

		/*
		* @brief Constructs a SafePtr from a raw pointer.
		*
		* @param val Pointer to the object to reference.
		*/
		SafePtr( tType* val ) : mValue( val ) {}

		/*
		* @brief Constructs a SafePtr from an object reference.
		*
		* @param val Object to reference.
		*/
		SafePtr( tType& val ) : mValue( &val ) {}

		/*
		* @brief Constructs a SafePtr by copying another SafePtr.
		* 
		* @param val SafePtr whose referenced object will be copied.
		*/
		SafePtr( const SafePtr<tType>& val ) : mValue( val.mValue ) {}

		/*
		* @brief Constructs a SafePtr from a convertible pointer type.
		*
		* @tparam tOther Source pointer type.
		* @param val Pointer to the object to reference.
		*/
		template<cNonPointer tOther>
			requires(std::is_convertible_v<tOther*, tType*>)
		SafePtr( tOther* val ) : mValue( val ) {}

		/*
		* @brief Constructs a SafePtr from another convertible SafePtr type.
		*
		* @tparam tOther Source SafePtr type.
		* @param val SafePtr whose referenced object will be copied.
		*/
		template<cNonPointer tOther>
			requires(std::is_convertible_v<tOther*, tType*>)
		SafePtr( const SafePtr<tOther>& val ) : mValue( val.mValue ) {}

		~SafePtr( ) = default;

		// =========================================================================
		// Primary Access Operator
		// =========================================================================

		/*
		* @brief Accesses the referenced object.
		*
		* @return Reference to the referenced object.
		*
		* Asserts at runtime if the SafePtr is empty. The source location of the
		* access is included in the assertion message.
		*/
		tType& operator()( std::source_location loc = std::source_location::current( ) ) const {

			LUM_ASSERT(
				mValue != nullptr,
				"Attempted to access unitialized SafePtr at %s: %u (%s)",
				loc.file_name( ),
				loc.line( ),
				loc.function_name( )
			);

			return *mValue;
		}

		// =========================================================================
		// Assignment Operators
		// =========================================================================

		SafePtr& operator=( tType* other ) {

			mValue = other;
			return *this;
		}

		SafePtr& operator=( tType& other ) {

			mValue = &other;
			return *this;
		}

		SafePtr& operator=( const SafePtr<tType>& other ) {

			mValue = other.mValue;
			return *this;
		}

		/*
		* @brief Assigns a pointer of a convertible type to the SafePtr.
		* @tparam tOther Source pointer type.
		* @param other Pointer to the object to reference.
		* @return Reference to this SafePtr.
		*/
		template<cNonPointer tOther>
			requires(std::is_convertible_v<tOther*, tType*>)
		SafePtr& operator=( tOther* other ) {

			mValue = other;
			return *this;
		}

		/*
		* @brief Assigns another convertible SafePtr to this SafePtr.
		* @tparam tOther Source SafePtr type.
		* @param other SafePtr whose referenced object will be copied.
		* @return Reference to this SafePtr.
		*/
		template<cNonPointer tOther>
			requires(std::is_convertible_v<tOther*, tType*>)
		SafePtr& operator=( const SafePtr<tOther>& other ) {

			mValue = other.mValue;
			return *this;
		}

		// =========================================================================
		// Accessors & Utility
		// =========================================================================

		/* 
		* @brief Returns the underlying raw pointer.
		* @return Pointer to the referenced object, or nullptr if empty.
		*/
		LUM_NODISCARD tType* Ptr( ) const {
			return mValue;
		}

		/* 
		* @brief Returns a reference to the referenced object.
		* @return Reference to the referenced object.
		* Asserts at runtime if the SafePtr is empty.
		*/
		LUM_NODISCARD tType& Ref( ) const {
			return (*this)();
		}

		/* @brief Returns if Ptr != nullptr */
		explicit operator bool( ) const {
			return mValue != nullptr;
		}

		/* 
		* @brief Clears the referenced pointer.
		* After calling Clear(), the SafePtr no longer references an object.
		*/
		void Clear( ) {
			mValue = nullptr;
		}

		/* @brief Logs the underlying pointer for debugging purposes. */
		void PrintDebug( ) const {
			LUM_LOG_DEBUG( mValue );
		}

	private:

		tType* mValue{ nullptr }; // < Non-owning pointer to the referenced object.

	};

} // namespace lum