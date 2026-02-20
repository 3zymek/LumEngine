//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Custom data structure to storage objects by handles
// 
//=============================================================================//
#pragma once

#include "core/core_pch.hpp"
#include "core/core_common.hpp"

namespace cstd {
	template<typename T>
		requires std::is_arithmetic_v<T>
	struct alignas(8) BaseHandle {
		T mID = lum::MaxVal<T>();
		T mGeneration = 0;

		bool operator==(const BaseHandle& other) const noexcept {
			return mID == other.mID && mGeneration == other.mGeneration;
		}
	};

	template<typename DenseType, typename HandleType, typename T = lum::uint32>
		requires std::is_arithmetic_v<T>
	class handle_pool {

		using Generation	= T;
		using Slot			= T;
		using SparseType	= T;
		using Hsize			= T;

		using iterator = typename std::vector<DenseType>::iterator;
		using const_iterator = typename std::vector<DenseType>::const_iterator;
		
		static constexpr SparseType NULL_HANDLE = lum::MaxVal<SparseType>();

	public:

		handle_pool(Hsize maxSize) : mMaxSize(maxSize) { reserve(mMaxSize); }

		iterator begin() { return mDense.begin(); }
		iterator end() { return mDense.end(); }

		const_iterator begin() const { return mDense.begin(); }
		const_iterator end() const { return mDense.end(); }

		DenseType& operator[](const Hsize id) {
			return mDense[mSparse[id]];
		}
		DenseType& operator[](const HandleType& handle) {
			return mDense[mSparse[handle.mID]];
		}

		HandleType CreateHandle(const DenseType& obj) {
			if (mDense.size() == mMaxSize)
				throw std::runtime_error("Handle pool full");

			Slot slot;

			if (!mFreeSlots.empty()) {
				slot = mFreeSlots.back();
				mFreeSlots.pop_back();
			}
			else
				slot = static_cast<Slot>(mNextSlot++);

			mDense.push_back(obj);

			Hsize lastDense = mDense.size() - 1;

			mSparse[slot] = static_cast<SparseType>(lastDense);
			mDenseToSparse[lastDense] = slot;

			HandleType handle;
			handle.mID = slot;
			handle.mGeneration = mGenerations[slot];

			return handle;

		}

		void DeleteHandle(const HandleType& handle) {
			Slot slot = static_cast<Slot>(handle.mID);
			if (slot >= mSparse.size()) return;
			if (mGenerations[slot] != handle.mGeneration) return;

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

		inline Hsize DenseSize() const { return mDense.size(); }

		inline bool Exist(const HandleType& handle) const noexcept {
			return handle.mID < mGenerations.size() && handle.mGeneration == mGenerations[handle.mID];
		}

		inline DenseType* Get(const HandleType& handle) {
			if (Exist(handle))
				return &mDense[mSparse[handle.mID]];
			else
				return nullptr;
		}

	private:

		Hsize mMaxSize = 0;

		Hsize mNextSlot = 0;

		std::vector<DenseType>	mDense;
		std::vector<SparseType>	mSparse;
		std::vector<SparseType> mDenseToSparse;
		std::vector<Generation> mGenerations;
		std::vector<Slot>		mFreeSlots;

	};
}