#pragma once
#include "sparse_set.hpp"

using sparse_t = size_t;

template<typename _Val>
_Val& cstd::sparse_set<_Val>::operator[](const sparse_t& _idx) {
	return m_dense[m_sparse[_idx]];
}
template<typename _Val>
_Val& cstd::sparse_set<_Val>::get_at(const sparse_t& _idx) {
	if (_idx > m_size) throw detail::OutOfRange("index out of range");
	else if (m_sparse[_idx] == NULL_SPARSE) throw detail::NullValue("null value at index");
	return m_dense[m_sparse[_idx]];
}

template<typename _Val>
template<typename T>
void cstd::sparse_set<_Val>::emplace(T&& _value, const sparse_t& _idx) {
	if ((_idx >= m_size) or (m_sparse[_idx] != NULL_SPARSE)) return;

	m_dense.emplace_back(std::forward<T>(_value));
	m_sparse[_idx] = m_dense.size() - 1;
	m_dense_to_sparse.push_back(_idx);
}

template<typename _Val>
void cstd::sparse_set<_Val>::reserve(const sparse_t& _size) noexcept {
	m_dense.reserve(_size);
	m_sparse.reserve(_size);
	m_dense_to_sparse.reserve(_size);
	m_size = _size;
}

template<typename _Val>
void cstd::sparse_set<_Val>::remove(const sparse_t& _idx) {
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
