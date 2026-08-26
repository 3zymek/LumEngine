//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Exception for deserialization process.
//
//=============================================================================//
#pragma once

#include "Core/CoreCommon.hpp"

namespace lum::fmt {

	/* @brief Exception thrown when scene deserialization fails. */
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

			m_Message = buffer;
		}

		explicit DeserializeException( const char* message ) : m_Message( message ) {}

		const char* what( ) const noexcept override {
			return m_Message.c_str( );
		}

	private:

		String m_Message{};

	};

}