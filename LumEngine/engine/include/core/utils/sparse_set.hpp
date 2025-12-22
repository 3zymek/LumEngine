// SPDX-License-Identifier: MIT
// Copyright (C) 2025 3zymek
//
// sparse_set<T>
// Dense buffer + sparse index map. O(1) insert/erase without holes.
// Ideal for ECS component storage (fast random access by entity ID).
#pragma once
#include <vector>
#include <exception>
#include <string>
namespace cstd {
	namespace detail {

		class NullValue : public std::exception {
			std::string msg;
		public:
			NullValue(const std::string& _msg) : msg(_msg) {}
			const char* what() const noexcept override {
				return msg.c_str();
			}
		};

		class OutOfRange : public std::exception {
			std::string msg;
		public:
			OutOfRange(const std::string& _msg) : msg(_msg) {}
			const char* what() const noexcept override {
				return msg.c_str();
			}
		};

	}

	template<typename _Val>
	class sparse_set {

		using sparse_t = size_t;

		using iterator			= typename std::vector<_Val>::iterator;
		using const_iterator	= typename std::vector<_Val>::const_iterator;

	public:

		constexpr sparse_set() = default;
		explicit constexpr sparse_set(const sparse_t& _size) : m_size(_size) { reserve(_size); fill_sparse(_size); }
		~sparse_set() {}

		iterator begin() { return m_dense.begin(); }
		iterator end() { return m_dense.end(); }

		const_iterator begin() const { return m_dense.begin(); }
		const_iterator end() const { return m_dense.end(); }

		_Val& operator[](const sparse_t&);

		_Val& get_at(const sparse_t&);

		template<typename T>
		void emplace(T&&, const sparse_t&);

		void reserve(const sparse_t&) noexcept;

		constexpr sparse_t size()		{ return m_size; }
		constexpr sparse_t dense_size() { return m_dense.size(); }
		constexpr bool empty()			{ return m_dense.empty(); }
		constexpr void clear() {
			m_dense.clear();
			m_sparse.clear();
			m_dense_to_sparse.clear();
		}

		void remove(const sparse_t&);

		constexpr bool contains(const sparse_t& _idx) {
			return _idx < m_size and m_sparse[_idx] != NULL_SPARSE;
		}

		constexpr void fill_sparse(const sparse_t& _size) {
			m_sparse.assign(_size, NULL_SPARSE);
			m_size = _size;
		}

	private:

		static constexpr size_t NULL_SPARSE = std::numeric_limits<size_t>::max();
		sparse_t m_size{ 0 };

		std::vector<_Val>		m_dense;
		std::vector<sparse_t>	m_sparse;
		std::vector<sparse_t>	m_dense_to_sparse;

	};
	#include "sparse_set.ipp"

}