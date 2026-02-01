#pragma once
#include <iostream>
#include <vector>
namespace cstd {
	template<typename DenseType, typename HandleType>
	class handle_pool {

		using Generation = uint32_t;
		using Slot = uint32_t;
		using SparseType = uint32_t;
		using Hsize_t = size_t;

		using iterator = typename std::vector<DenseType>::iterator;
		using const_iterator = typename std::vector<DenseType>::const_iterator;

		static constexpr SparseType NULL_HANDLE = std::numeric_limits<SparseType>::max();

	public:

		handle_pool(Hsize_t new_max_size) : MAX_SIZE(new_max_size) { reserve(MAX_SIZE); }

		iterator begin() { return m_dense.begin(); }
		iterator end() { return m_dense.end(); }

		const_iterator begin() const { return m_dense.begin(); }
		const_iterator end() const { return m_dense.end(); }

		DenseType& operator[](const Hsize_t id) {
			return m_dense[m_sparse[id]];
		}
		DenseType& operator[](const HandleType& handle) {
			return m_dense[m_sparse[handle.id]];
		}

		HandleType create_handle(const DenseType& obj) {
			if (m_dense.size() == MAX_SIZE)
				throw std::runtime_error("Handle pool full");

			Slot slot;

			if (!m_free_slots.empty()) {
				slot = m_free_slots.back();
				m_free_slots.pop_back();
			}
			else
				slot = static_cast<Slot>(nextSlot++);

			m_dense.push_back(obj);

			Hsize_t lastDense = m_dense.size() - 1;

			m_sparse[slot] = static_cast<SparseType>(lastDense);
			m_dense_to_sparse[lastDense] = slot;

			HandleType handle;
			handle.id = slot;
			handle.generation = m_generations[slot];

			return handle;

		}

		void delete_handle(const HandleType& handle) {
			Slot slot = static_cast<Slot>(handle.id);
			if (slot >= m_sparse.size()) return;
			if (m_generations[slot] != handle.generation) return;

			Hsize_t denseIndex = m_sparse[slot];
			Hsize_t lastIndex = m_dense.size() - 1;

			if (denseIndex != lastIndex) {

				std::swap(m_dense[denseIndex], m_dense[lastIndex]);

				Slot movedSlot = m_dense_to_sparse[lastIndex];
				m_sparse[movedSlot] = static_cast<SparseType>(denseIndex);
				m_dense_to_sparse[denseIndex] = static_cast<SparseType>(movedSlot);


			}

			m_dense.pop_back();
			m_dense_to_sparse.pop_back();

			m_generations[slot]++;
			m_free_slots.push_back(slot);

			m_sparse[slot] = NULL_HANDLE;

		}

		void reserve(Hsize_t new_max_size) {
			m_dense.reserve(new_max_size);
			m_sparse.resize(new_max_size, NULL_HANDLE);
			m_dense_to_sparse.resize(new_max_size, NULL_HANDLE);
			m_generations.resize(new_max_size, 0);
			m_free_slots.reserve(new_max_size);
		}

		inline Hsize_t dense_size() const { return m_dense.size(); }

		inline bool exist(const HandleType& handle) const noexcept {
			return handle.id < m_generations.size() && handle.generation == m_generations[handle.id];
		}

		inline DenseType* get(const HandleType& handle) {
			if (exist(handle))
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