// SPDX-License-Identifier: MIT
// Copyright (C) 2025 3zymek
//
// sparse_set<T>
// Dense buffer + sparse index map. O(1) insert/erase without holes.
// Ideal for ECS component storage (fast random access by entity ID).
#pragma once
#include "core/core_pch.hpp"
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

	template<typename val, typename sparse_val>
	class sparse_set {

		using sparse_t = sparse_val;

		using iterator			= typename std::vector<val>::iterator;
		using const_iterator	= typename std::vector<val>::const_iterator;

	public:

		constexpr sparse_set() = default;
		explicit constexpr sparse_set(const sparse_t& _size) : m_size(_size) { reserve(_size); fill_sparse(_size); }
		~sparse_set() {}

		iterator begin() { return m_dense.begin(); }
		iterator end() { return m_dense.end(); }

		const_iterator begin() const { return m_dense.begin(); }
		const_iterator end() const { return m_dense.end(); }

		val& operator[](const sparse_t&);

		val& get_at(const sparse_t&);

		template<typename T>
		void emplace(T&&, const sparse_t&);

		void reserve(const sparse_t&) noexcept;

		inline constexpr sparse_t size()		{ return m_size; }
		inline constexpr sparse_t dense_size() { return m_dense.size(); }
		inline constexpr bool empty()			{ return m_dense.empty(); }
		inline constexpr void clear() {
			m_dense.clear();
			m_sparse.clear();
			m_dense_to_sparse.clear();
		}

		void remove(const sparse_t&);

		constexpr bool contains(const sparse_t& _idx) {
			return _idx < m_size and m_sparse[_idx] != NULL_SPARSE;
		}

		constexpr void fill_sparse(const size_t& _size) {
			m_sparse.assign(_size, NULL_SPARSE);
			m_size = _size;
		}

	private:

		static constexpr sparse_t NULL_SPARSE = std::numeric_limits<sparse_t>::max();
		size_t m_size{ 0 };

		std::vector<val>		m_dense;
		std::vector<sparse_t>	m_sparse;
		std::vector<size_t>	m_dense_to_sparse;

	};
	#include "sparse_set.ipp"

}