//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Very simple performance profiler for Render Hardware Interface
// 
//=============================================================================//
#pragma once
#include "Rhi/RhiCommon.hpp"

namespace lum::rhi {

	/* @brief RHI performance monitoring utilities. */
	namespace performance {

		/* @brief Lightweight profiler for tracking RHI performance metrics.
		* Collects draw call counts and texture cache hit/miss statistics
		* between StartRecording and EndRecording calls.
		*/
		class Profiler {
		public:

			/* @brief Begins a new profiling session and resets all counters. */
			void StartRecording() {
				m_Recording = true;
				m_CacheHits = 0;
				m_CacheMisses = 0;
			}

			/* @brief Ends the current profiling session. */
			void EndRecording() {
				m_Recording = false;
			}

			/* @brief Increments the draw call counter by one. */
			void RegisterDrawCall() {
				m_DrawCalls++;
			}

			/* @brief Increments the cache miss counter by one. */
			void RegisterCacheMiss() {
				m_CacheMisses++;
			}

			/* @brief Increments the cache hit counter by one. */
			void RegisterCacheHit() {
				m_CacheHits++;
			}

			/* @brief Computes the cache hit rate for the current session.
			* @return Ratio of cache hits to total cache accesses (0.0 - 1.0).
			*         Returns 0.0 if no cache accesses have been recorded.
			*/
			float32 GetCacheHitRate() const {
				uint32 total = m_CacheHits + m_CacheMisses;
				return total > 0 ? (float32)m_CacheHits / total : 0.0f;
			}

		private:

			/* @brief Whether the profiler is currently recording. */
			bool m_Recording = false;

			/* @brief Total number of draw calls recorded in the current session. */
			uint32 m_DrawCalls = 0;

			/* @brief Number of cache hits recorded in the current session. */
			uint32 m_CacheHits = 0;

			/* @brief Number of cache misses recorded in the current session. */
			uint32 m_CacheMisses = 0;

		};

	} // namespace lum::rhi::performance

} // namespace lum::rhi