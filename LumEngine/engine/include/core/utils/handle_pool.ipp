#pragma once
#include "handle_pool.hpp"

template<typename DenseType, typename HandleType>
inline HandleType cstd::handle_pool<DenseType, HandleType>::CreateHandle(const DenseType& obj) {
	if (m_dense.size() == MAX_SIZE)
		throw std::runtime_error("Handle pool full");

	Slot slot;

	if (!m_free_slots.empty()) {
		slot = m_free_slots.back();
		m_free_slots.pop_back();
	}
	else
		slot = nextSlot++;

	m_dense.push_back(obj);

	Hsize_t lastDense = m_dense.size() - 1;

	m_sparse[slot] = lastDense;
	m_dense_to_sparse[lastDense] = slot;

	HandleType handle;
	handle.id = slot;
	handle.generation = m_generations[slot];

	return handle;

}
template<typename DenseType, typename HandleType>
inline void cstd::handle_pool<DenseType, HandleType>::DeleteHandle(const HandleType& handle) {
	Slot slot = handle.id;
	if (slot >= m_sparse.size()) return;
	if (m_generations[slot] != handle.generation) return;

	Hsize_t denseIndex = m_sparse[slot];
	Hsize_t lastIndex = m_dense.size() - 1;

	if (denseIndex != lastIndex) {

		std::swap(m_dense[denseIndex], m_dense[lastIndex]);

		Slot movedSlot = m_dense_to_sparse[lastIndex];
		m_sparse[movedSlot] = denseIndex;
		m_dense_to_sparse[denseIndex] = movedSlot;


	}

	m_dense.pop_back();
	m_dense_to_sparse.pop_back();

	m_generations[slot]++;
	m_free_slots.push_back(slot);



}

template<typename DenseType, typename HandleType>
inline void cstd::handle_pool<DenseType, HandleType>::Reserve(Hsize_t new_max_size) {
	m_dense.reserve(new_max_size);
	m_sparse.resize(new_max_size, NULL_HANDLE);
	m_dense_to_sparse.resize(new_max_size, NULL_HANDLE);
	m_generations.resize(new_max_size, 0);
	m_free_slots.reserve(new_max_size);
}