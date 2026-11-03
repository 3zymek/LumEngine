//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Provides directory traversal utilities.
//
//=============================================================================//
#pragma once

#include "Platform/FileSystem/Path.hpp"

namespace lum {

	namespace detail {

		/* @brief Iterator over the entries of a directory. */
		class DirectoryIterator {
		public:

			using Iterator = std::filesystem::directory_iterator;

			/* @brief Creates an iterator starting at the given directory. */
			DirectoryIterator( const Path& path ) : m_Iterator( path.ToString( ) ) {}

			/* @brief Creates an end iterator. */
			DirectoryIterator( ) {}

			/* @brief Advances the iterator to the next directory entry. */
			DirectoryIterator& operator++( ) {
				++m_Iterator;
				return *this;
			}

			/* @brief Checks whether two iterators point to different entries. */
			bool operator!=( DirectoryIterator& other ) {
				return m_Iterator != other.m_Iterator;
			}

			/* @brief Returns the path of the current directory entry. */
			Path operator*( ) {
				return Path( m_Iterator->path( ) );
			}

		private:

			Iterator m_Iterator{};

		};

	}

	/* @brief Provides access to the entries of a directory. */
	class Directory {
	public:

		/* @brief Creates a directory wrapper for the given path. */
		Directory( const Path& path ) : m_Path( path ) {}

		/* @brief Returns an iterator to the first directory entry. */
		detail::DirectoryIterator begin( ) {
			return detail::DirectoryIterator( m_Path );
		}

		/* @brief Returns an iterator representing the end of the directory. */
		detail::DirectoryIterator end( ) {
			return detail::DirectoryIterator( );
		}

		/* @brief Returns the path represented by this directory. */
		Path& GetPath( ) {
			return m_Path;
		}

	private:

		Path m_Path{};

	};

}