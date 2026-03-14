#pragma once

#include "ahi/ahi_common.hpp"

namespace lum::ahi {

	namespace detail {
		enum class FrequnecyType : byte {
			Low,
			High
		};
	}

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
			float32 mWetLevel = 0.5f;
			float32 mDryLevel = 1.0f;
			float32 mDensity = 1.0f;

		} mReverb;

		struct FEcho {
			
			bool bEnabled = false;

			float32 mDelay = 500.0f;
			float32 mFeedback = 50.0f;
			float32 mWetLevel = 0.5f;
			float32 mDryLevel = 1.0f;

			
		} mEcho;

		struct FDistortion {

			bool bEnabled = false;

			float32 mLevel = 0.0f;

		} mDistortion;

		struct FChorus {

			bool bEnabled = false;

			float32 mRate = 0.5f;
			float32 mDepth = 3.0f;
			float32 mMix = 50.0f;

		} mChorus;

		struct FFlange {

			bool bEnabled = false;

			float32 mRate = 0.5f;
			float32 mDepth = 3.0f;
			float32 mMix = 50.0f;

		} mFlange;

		struct FCompressor {

			bool bEnabled = false;

			float32 mThreshold = -20.0f;
			float32 mRatio = 4.0f;
			float32 mAttack = 10.0f;
			float32 mRelease = 100.0f;
			float32 mGainMakeup = 0.0f;

		} mCompressor;
		
	};

	struct FAudioEffect {

		std::vector<vptr> mDSPs;

	};

} // namespace lum::ahi