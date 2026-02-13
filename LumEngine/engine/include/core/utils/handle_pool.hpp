#pragma once
#include "core/core_pch.hpp"
#include "core/core_common.hpp"

namespace cstd {
	struct alignas(8) BaseHandle {
		lum::uint32 id = lum::MaxVal<lum::uint32>();
		lum::uint32 generation = 0;

		bool operator==(const BaseHandle& other) const noexcept {
			return id == other.id && generation == other.generation;
		}
	};


	template<typename DenseType, typename HandleType>
		requires std::is_base_of_v<BaseHandle, HandleType>
	class handle_pool {

		using Generation	= lum::uint32;
		using Slot			= lum::uint32;
		using SparseType	= lum::uint32;
		using Hsize			= lum::usize;

		using iterator = typename std::vector<DenseType>::iterator;
		using const_iterator = typename std::vector<DenseType>::const_iterator;
		
		static constexpr SparseType NULL_HANDLE = lum::MaxVal<SparseType>();

	public:

		handle_pool(Hsize new_max_size) : MAX_SIZE(new_max_size) { reserve(MAX_SIZE); }

		iterator begin() { return mDense.begin(); }
		iterator end() { return mDense.end(); }

		const_iterator begin() const { return mDense.begin(); }
		const_iterator end() const { return mDense.end(); }

		DenseType& operator[](const Hsize id) {
			return mDense[mSparse[id]];
		}
		DenseType& operator[](const HandleType& handle) {
			return mDense[mSparse[handle.id]];
		}

		HandleType create_handle(const DenseType& obj) {
			if (mDense.size() == MAX_SIZE)
				throw std::runtime_error("Handle pool full");

			Slot slot;

			if (!mFreeSlots.empty()) {
				slot = mFreeSlots.back();
				mFreeSlots.pop_back();
			}
			else
				slot = static_cast<Slot>(nextSlot++);

			mDense.push_back(obj);

			Hsize lastDense = mDense.size() - 1;

			mSparse[slot] = static_cast<SparseType>(lastDense);
			mDenseToSparse[lastDense] = slot;

			HandleType handle;
			handle.id = slot;
			handle.generation = mGenerations[slot];

			return handle;

		}

		void delete_handle(const HandleType& handle) {
			Slot slot = static_cast<Slot>(handle.id);
			if (slot >= mSparse.size()) return;
			if (mGenerations[slot] != handle.generation) return;

			Hsize denseIndex = mSparse[slot];
			Hsize lastIndex = mDense.size() - 1;

			if (denseIndex != lastIndex) {

				std::swap(mDense[denseIndex], mDense[lastIndex]);

				Slot movedSlot = mDenseToSparse[lastIndex];
				mSparse[movedSlot] = static_cast<SparseType>(denseIndex);
				mDenseToSparse[denseIndex] = static_cast<SparseType>(movedSlot);


			}

			mDense.pop_back();
			mDenseToSparse.pop_back();

			mGenerations[slot]++;
			mFreeSlots.push_back(slot);

			mSparse[slot] = NULL_HANDLE;

		}

		void reserve(Hsize new_max_size) {
			mDense.reserve(new_max_size);
			mSparse.resize(new_max_size, NULL_HANDLE);
			mDenseToSparse.resize(new_max_size, NULL_HANDLE);
			mGenerations.resize(new_max_size, 0);
			mFreeSlots.reserve(new_max_size);
		}

		inline Hsize dense_size() const { return mDense.size(); }

		inline bool exist(const HandleType& handle) const noexcept {
			return handle.id < mGenerations.size() && handle.generation == mGenerations[handle.id];
		}

		inline DenseType* get(const HandleType& handle) {
			if (exist(handle))
				return &mDense[mSparse[handle.id]];
			else
				return nullptr;
		}

	private:

		Hsize MAX_SIZE = 0;

		Hsize nextSlot = 0;

		std::vector<DenseType>	mDense;
		std::vector<SparseType>	mSparse;
		std::vector<SparseType> mDenseToSparse;
		std::vector<Generation> mGenerations;
		std::vector<Slot>		mFreeSlots;

	};
}