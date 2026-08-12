#pragma once

#include "Core/CoreCommon.hpp"

namespace lum::fmt {

	class DeserializeException : public std::exception {
	public:

		template<typename... tArgs>
		explicit DeserializeException( const char* format, tArgs&&... args ) {

			char buffer[ 1024 ]{};

			FormatString(
				buffer,
				format,
				std::forward<tArgs>( args )...
			);

			mMessage = buffer;
		}
		explicit DeserializeException( const char* message ) : mMessage( message ) {}

		const char* what( ) const noexcept override {
			return mMessage.c_str( );
		}

	private:

		String mMessage{};


	};

}