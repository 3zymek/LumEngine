// LumEngine Copyright (C) 2026 3zymek
// All rights reserved.
// Performance profiler for Render Hardware Interface
#pragma once

#include "core/setup.hpp"
#include "rhi/rhi_common.hpp"

namespace lum::rhi::performance {

	class Profiler {
	public:

		void StartRecording() {
			bRecording = true;
			cacheHits = 0;
			cacheMisses = 0;
		}
		void EndRecording() {
			bRecording = false;
		}

		void RegisterDrawCall() {
			drawCalls++;
		}

		void RegisterCacheMiss() {
			cacheMisses++;
		}

		void RegisterCacheHit() {
			cacheHits++;
		}

		float32 GetCacheHitRate() const {
			uint32 total = cacheHits + cacheMisses;
			return total > 0 ? (float32)cacheHits / total : 0.0f;
		}

	private:

		bool bRecording = false;

		uint32 drawCalls = 0;
		uint32 cacheHits = 0;
		uint32 cacheMisses = 0;

	};

}