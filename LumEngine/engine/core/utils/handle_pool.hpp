#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include "core_defines.hpp"
namespace cstd {
	template<typename DenseType, typename HandleType>
	class handle_pool {

		using Generation = uint32_t;
		using Slot = uint32_t;
		using SparseType = uint32_t;
		using Hsize_t = size_t;

		Hsize_t NULL_HANDLE = -1;

	public:

		handle_pool(Hsize_t new_max_size) : MAX_SIZE(new_max_size) { Reserve(MAX_SIZE); }

		DenseType& operator[](const Hsize_t id) {
			return m_dense[m_sparse[id]];
		}
		DenseType& operator[](const HandleType& handle) {
			return m_dense[m_sparse[handle.id]];
		}

		__forceinline HandleType CreateHandle(const DenseType& obj) {
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

		__forceinline void DeleteHandle(const HandleType& handle) {
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

		__forceinline void Reserve(Hsize_t new_max_size) {
			m_dense.reserve(new_max_size);
			m_sparse.resize(new_max_size, NULL_HANDLE);
			m_dense_to_sparse.resize(new_max_size, NULL_HANDLE);
			m_generations.resize(new_max_size, 0);
			m_free_slots.reserve(new_max_size);
		}

		__forceinline bool Use(const HandleType& handle) const noexcept {
			return Exists(handle);
		}

		__forceinline bool Exists(const HandleType& handle) const noexcept {
			return handle.id < m_generations.size() && handle.generation == m_generations[handle.id];
		}

		__forceinline DenseType* Get(const HandleType& handle) {
			if (Exists(handle))
				return &m_dense[m_sparse[handle.id]];
			else
				return nullptr;

		}

	private:

		Hsize_t MAX_SIZE = 0;

		Hsize_t nextSlot = 0;

		std::vector<DenseType>	m_dense;
		std::vector<SparseType>	m_sparse;
		std::vector<SparseType> m_dense_to_sparse;
		std::vector<Generation> m_generations;
		std::vector<Slot>		m_free_slots;

	};
}