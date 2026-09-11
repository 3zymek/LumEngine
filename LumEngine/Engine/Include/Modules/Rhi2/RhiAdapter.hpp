#pragma once

#include "Rhi2/RhiCommon.hpp"
#include "Core/Utils/Ranged.hpp"

namespace lum::rhi {

	enum class AdapterType : uint8 {
		Discrete,
		Integrated,
		Virtual,
		Cpu,
		Unknown
	};

	struct AdapterDescription {
		String m_AdapterName{};
		AdapterType m_AdapterType = AdapterType::Unknown;
		uint64 m_DedicatedVideoMemory = 0;
		uint64 m_SharedSystemMemory = 0;
		uint32 m_VendorId = 0;
		uint32 m_AdapterId = 0;
	};


	enum class AdapterFeature {

		Anisotropy,
		GeometryShader,
		TessellationShader,
		MultiViewport,
		IndependentBlend,
		SampleRateShading,
		WideLines,

	};

	using AdapterScore = Ranged<uint32, 0, 100>;

	class AdapterRequirements {
	public:

		void AddRequiredFeature( AdapterFeature feature ) {
			m_RequiredFeatures.emplace( feature );
		}

		void AddFeature( AdapterFeature feature, AdapterScore weight ) {
			m_FeatureWeights.try_emplace( feature, weight );
		}

		const std::unordered_set<AdapterFeature>& GetRequiredFeatures( ) const noexcept { return m_RequiredFeatures; }
		const std::unordered_map<AdapterFeature, uint32>& GetFeatures( ) const noexcept { return m_FeatureWeights; }

	private:

		std::unordered_set<AdapterFeature> m_RequiredFeatures{};
		std::unordered_map<AdapterFeature, uint32> m_FeatureWeights{};

	};

} // namespace lum::rhi