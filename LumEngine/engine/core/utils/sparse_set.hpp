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

		constexpr __forceinline _Val& operator[](const sparse_t& _idx) {
			return m_dense[m_sparse[_idx]];
		}

		constexpr __forceinline _Val& get_at(const sparse_t& _idx) {
			if (_idx > m_size) throw detail::OutOfRange("index out of range");
			else if (m_sparse[_idx] == NULL_SPARSE) throw detail::NullValue("null value at index");
			return m_dense[m_sparse[_idx]];
		}

		template<typename T>
		constexpr __forceinline void append(T&& _value, const sparse_t& _idx) {
			if ((_idx >= m_size) or (m_sparse[_idx] != NULL_SPARSE)) return;

			m_dense.emplace_back(std::forward<T>(_value));
			m_sparse[_idx] = m_dense.size() - 1;
			m_dense_to_sparse.push_back(_idx);
		}

		constexpr __forceinline void reserve(const sparse_t& _size) noexcept {
			m_dense.reserve(_size);
			m_sparse.reserve(_size);
			m_dense_to_sparse.reserve(_size);
			m_size = _size;
		}

		constexpr __forceinline sparse_t size() { return m_size; }
		constexpr __forceinline bool empty() { return m_dense.empty(); }
		constexpr __forceinline void clear() {
			m_dense.clear();
			m_sparse.clear();
			m_dense_to_sparse.clear();
		}

		constexpr __forceinline void remove(const sparse_t& _idx) {
			if (_idx > m_size) return;
			if (m_sparse[_idx] == NULL_SPARSE) return;
			sparse_t to_delete = m_sparse[_idx];
			sparse_t last = m_dense.size() - 1;
			if (to_delete != last) {

				m_dense[to_delete] = m_dense[last];
				sparse_t moved = m_dense_to_sparse[last];
				m_sparse[moved] = to_delete;
				m_dense_to_sparse[to_delete] = moved;

			}
			m_dense.pop_back();
			m_dense_to_sparse.pop_back();
			m_sparse[_idx] = NULL_SPARSE;
		}

		constexpr __forceinline bool contains(const sparse_t& _idx) {
			return _idx < m_size and m_sparse[_idx] != NULL_SPARSE;
		}

		constexpr __forceinline void fill_sparse(const sparse_t& _size) {
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

}