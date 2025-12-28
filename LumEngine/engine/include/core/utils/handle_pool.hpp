#pragma once
#include "core/core_pch.hpp"
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

		HandleType CreateHandle(const DenseType& obj);

		void DeleteHandle(const HandleType& handle);

		void Reserve(Hsize_t new_max_size);

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
	#include "handle_pool.ipp"
}