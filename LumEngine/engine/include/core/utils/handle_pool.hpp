#pragma once
#include "core/core_pch.hpp"
namespace cstd {
	template<typename DenseType, typename HandleType>
	class handle_pool {

		using Generation = uint32_t;
		using Slot = uint32_t;
		using SparseType = uint32_t;
		using Hsize_t = size_t;

		using iterator = typename std::vector<DenseType>::iterator;
		using const_iterator = typename std::vector<DenseType>::const_iterator;

		Hsize_t NULL_HANDLE = -1;

	public:

		handle_pool(Hsize_t new_max_size) : MAX_SIZE(new_max_size) { Reserve(MAX_SIZE); }

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

		HandleType CreateHandle(const DenseType& obj);

		void DeleteHandle(const HandleType& handle);

		void Reserve(Hsize_t new_max_size);

		inline Hsize_t DenseSize() const { return m_dense.size(); }

		inline bool Exists(const HandleType& handle) const noexcept {
			return handle.id < m_generations.size() && handle.generation == m_generations[handle.id];
		}

		inline DenseType* Get(const HandleType& handle) {
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
	#include "handle_pool.ipp"
}