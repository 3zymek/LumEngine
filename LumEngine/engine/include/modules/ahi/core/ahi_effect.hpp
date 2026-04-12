//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Audio effect descriptors and effect handle for AHI.
//          FAudioEffectDescriptor defines parameters for all supported
//          effect types. FAudioEffect holds backend DSP handles.
//
//=============================================================================//
#pragma once
#include "ahi/ahi_common.hpp"

namespace lum::ahi {

	/* @brief Descriptor used to configure an audio effect chain.
	*
	* Each nested struct represents one effect type with its parameters.
	* Only effects with bEnabled = true are created by AudioDevice::CreateEffect().
	* Multiple effects can be enabled simultaneously and will be chained in order.
	*/
	struct FAudioEffectDescriptor {

		/* @brief Low and high frequency pass filters (lowpass / highpass). */
		struct FFrequencyPass {

			/* @brief Parameters for a single frequency pass filter. */
			struct FPass {

				bool bEnabled = false;

				float32 mCutoff = 10.0f;  // Cutoff frequency in Hz
				float32 mResonance = 5.0f;   // Resonance (Q factor) at the cutoff point
			};

			FPass mLow;   // Lowpass  — attenuates frequencies above cutoff
			FPass mHigh;  // Highpass — attenuates frequencies below cutoff

		} mFreqPass;

		/* @brief Reverb effect simulating acoustic environments. */
		struct FReverb {

			bool bEnabled = false;

			float32 mDecayTime = 0.5f;  // Reverberation decay time in ms
			float32 mEarlyDelay = 0.1f;  // Delay before early reflections in ms
			float32 mLateDelay = 0.1f;  // Delay before late reverberation in ms
			float32 mReferenceFreq = 1.0f; // Reference frequency for HF decay in Hz
			float32 mDiffusion = 0.0f;  // Echo density — 0 = discrete echoes, 1 = smooth tail
			float32 mWetLevel = 0.5f;  // Reverb signal level in dB
			float32 mDryLevel = 1.0f;  // Original signal level in dB
			float32 mDensity = 1.0f;  // Modal density of the reverb tail

		} mReverb;

		/* @brief Echo effect with configurable delay and feedback. */
		struct FEcho {

			bool bEnabled = false;

			float32 mDelay = 500.0f; // Delay between repetitions in ms
			float32 mFeedback = 50.0f;  // Amount of echo fed back into input (0–100%). Higher = more repetitions
			float32 mWetLevel = 0.5f;   // Echo signal level in dB
			float32 mDryLevel = 1.0f;   // Original signal level in dB

		} mEcho;

		/* @brief Distortion effect — clips the signal to add harmonic content. */
		struct FDistortion {

			bool bEnabled = false;
			
			float32 mLevel = 0.0f; // Distortion amount (0.0 = clean, 1.0 = heavy clipping)

		} mDistortion;

		/* @brief Chorus effect — thickens sound by layering modulated copies of the signal. */
		struct FChorus {

			bool bEnabled = false;

			float32 mRate = 0.5f;  // Modulation rate in Hz
			float32 mDepth = 3.0f;  // Modulation depth in ms
			float32 mMix = 50.0f; // Wet/dry mix (0–100%)

		} mChorus;

		/* @brief Flanger effect — metallic sweeping sound via short modulated delay with feedback. */
		struct FFlange {

			bool bEnabled = false;

			float32 mRate = 0.5f;	// Modulation rate in Hz
			float32 mDepth = 3.0f;  // Modulation depth (0–1)
			float32 mMix = 50.0f;	// Wet/dry mix (0–100%)

		} mFlange;

		/* @brief Dynamic range compressor — reduces volume peaks and evens out loudness. */
		struct FCompressor {

			bool bEnabled = false;

			float32 mThreshold = -20.0f; // Level in dB above which compression is applied
			float32 mRatio = 4.0f;   // Compression ratio (e.g. 4:1 — 4dB in becomes 1dB out)
			float32 mAttack = 10.0f;  // Time in ms for compression to engage after threshold is crossed
			float32 mRelease = 100.0f; // Time in ms for compression to disengage after signal drops below threshold
			float32 mGainMakeup = 0.0f;   // Post-compression gain boost in dB to restore perceived loudness

		} mCompressor;

		/* @brief Parametric equalizer — boosts or cuts a specific frequency band. */
		struct FParamEQ {

			bool bEnabled = false;

			float32 mCenter		= 1000.0f;	// Center frequency of the band in Hz
			float32 mBandwidth	= 1.0f;		// Bandwidth in octaves around the center frequency
			float32 mGain		= 0.0f;		// Gain in dB — positive = boost, negative = cut

		} mParamEQ;

	};

	/* @brief Backend-side effect handle storing DSP node pointers.
	* Owned by the AudioDevice. Do not access DSPs directly from outside the backend.
	*/
	struct FAudioEffect {

		std::vector<vptr> mDSPs;

	};

	namespace detail {
		/* @brief Internal tag used to distinguish lowpass from highpass during effect creation. */
		enum class FrequnecyType : byte {
			Low,
			High
		};

	}

} // namespace lum::ahi