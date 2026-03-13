#pragma once

#include "ahi/ahi_common.hpp"

namespace lum::ahi {

	enum class EffectType : byte {

		Reverb,
		LowPass,
		HighPass,
		Echo,
		Distortion,
		Chorus,
		Flanger,
		Compressor,

	};

	enum class ReverbPreset : byte {

		Off,
		Generic,
		PaddedCell,
		Room,
		Bathroom,
		Livingroom,
		Stoneroom,
		Auditorium,
		ConcertHall,
		Cave,
		Arena,
		Hangar,
		StoneCorridor,
		Alley,
		Forest,
		City,
		Mountains,
		Quarry,
		Plain,
		ParkingLot,
		SewerPipe,
		UnderWater,

	};

	struct FAudioEffectDescriptor {

		struct FFrequencyPass {

			struct FPass {

				bool bEnabled = false;
				float32 mCutoff = 10.0f;
				float32 mResonance = 5.0f;

			};

			FPass mLow;
			FPass mHigh;

		} mFreqPass;

		struct FReverb {

			bool bEnabled = false;

			ReverbPreset mPreset = ReverbPreset::Off;

			float32 mDecayTime = 0.5f;
			float32 mEarlyDelay = 0.1f;
			float32 mLateDelay = 0.1f;
			float32 mReferenceFreq = 1.0f;
			float32 mDiffusion = 0.f;
			float32 mWetLevel = 1.0f;
			float32 mDryLevel = 1.0f;

		} mReverb;
		
	};

	struct FAudioEffect {

		std::vector<vptr> mDSPs;

	};

} // namespace lum::ahi