#pragma once

#include "Core/Utils/LumAssert.hpp"
#include "Core/CoreDefines.hpp"
#include "Core/Types.hpp"

namespace lum {

	template<cNonPointer tType>
	class SafePtr {

		template<cNonPointer tOther>
		friend class SafePtr;

	public:

		// =========================================================================
		// Construction & Destruction
		// =========================================================================

		SafePtr( ) : mValue( nullptr ) {}
		SafePtr( tType* val ) : mValue( val ) {}
		SafePtr( tType& val ) : mValue( &val ) {}
		SafePtr( const SafePtr<tType>& val ) : mValue( val.mValue ) {}

		template<cNonPointer tOther>
			requires(std::is_convertible_v<tOther*, tType*>)
		SafePtr( tOther* val ) : mValue( val ) {}

		template<cNonPointer tOther>
			requires(std::is_convertible_v<tOther*, tType*>)
		SafePtr( const SafePtr<tOther>& val ) : mValue( val.mValue ) {}

		~SafePtr( ) = default;

		// =========================================================================
		// Primary Access Operator
		// =========================================================================

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

		template<cNonPointer tOther>
			requires(std::is_convertible_v<tOther*, tType*>)
		SafePtr& operator=( tOther* other ) {
			mValue = other;
			return *this;
		}

		template<cNonPointer tOther>
			requires(std::is_convertible_v<tOther*, tType*>)
		SafePtr& operator=( const SafePtr<tOther>& other ) {
			mValue = other.mValue;
			return *this;
		}

		// =========================================================================
		// Accessors & Utility
		// =========================================================================

		LUM_NODISCARD tType* Ptr( ) const { return mValue; }
		LUM_NODISCARD tType& Ref( ) const { return (*this)(); }

		explicit operator bool( ) const { return mValue != nullptr; }

		void Clear( ) {
			mValue = nullptr;
		}

		void PrintDebug( ) const {
			LUM_LOG_DEBUG( mValue );
		}

	private:

		tType* mValue{ nullptr };

	};

}