//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Audio effect descriptors and effect handle for AHI.
//          FAudioEffectDescriptor defines parameters for all supported
//          effect types. AudioEffect holds backend DSP handles.
//
//=============================================================================//
#pragma once
#include "Ahi/AhiCommon.hpp"

namespace lum::ahi {

	/* @brief Descriptor used to configure an audio effect chain.
	*
	* Each nested struct represents one effect type with its parameters.
	* Only effects with bEnabled = true are created by AudioDevice::CreateEffect().
	* Multiple effects can be enabled simultaneously and will be chained in order.
	*/
	struct AudioEffectCreateInfo {

		/* @brief Low and high frequency pass filters (lowpass / highpass). */
		struct FrequencyPass {

			/* @brief Parameters for a single frequency pass filter. */
			struct Pass {

				bool m_Enabled = false;

				float32 m_Cutoff = 10.0f;  // Cutoff frequency in Hz
				float32 m_Resonance = 5.0f;   // Resonance (Q factor) at the cutoff point
			};

			Pass m_Low;   // Lowpass  — attenuates frequencies above cutoff
			Pass m_High;  // Highpass — attenuates frequencies below cutoff

		} m_FreqPass;

		/* @brief Reverb effect simulating acoustic environments. */
		struct Reverb {

			bool m_Enabled = false;

			float32 m_DecayTime = 0.5f;  // Reverberation decay time in ms
			float32 m_EarlyDelay = 0.1f;  // Delay before early reflections in ms
			float32 m_LateDelay = 0.1f;  // Delay before late reverberation in ms
			float32 m_ReferenceFreq = 1.0f; // Reference frequency for HF decay in Hz
			float32 m_Diffusion = 0.0f;  // Echo density — 0 = discrete echoes, 1 = smooth tail
			float32 m_WetLevel = 0.5f;  // Reverb signal level in dB
			float32 m_DryLevel = 1.0f;  // Original signal level in dB
			float32 m_Density = 1.0f;  // Modal density of the reverb tail

		} m_Reverb;

		/* @brief Echo effect with configurable delay and feedback. */
		struct Echo {

			bool m_Enabled = false;

			float32 m_Delay = 500.0f; // Delay between repetitions in ms
			float32 m_Feedback = 50.0f;  // Amount of echo fed back into input (0–100%). Higher = more repetitions
			float32 m_WetLevel = 0.5f;   // Echo signal level in dB
			float32 m_DryLevel = 1.0f;   // Original signal level in dB

		} m_Echo;

		/* @brief Distortion effect — clips the signal to add harmonic content. */
		struct Distortion {

			bool m_Enabled = false;

			float32 m_Level = 0.0f; // Distortion amount (0.0 = clean, 1.0 = heavy clipping)

		} m_Distortion;

		/* @brief Chorus effect — thickens sound by layering modulated copies of the signal. */
		struct Chorus {

			bool m_Enabled = false;

			float32 m_Rate = 0.5f;  // Modulation rate in Hz
			float32 m_Depth = 3.0f;  // Modulation depth in ms
			float32 m_Mix = 50.0f; // Wet/dry mix (0–100%)

		} m_Chorus;

		/* @brief Flanger effect — metallic sweeping sound via short modulated delay with feedback. */
		struct Flange {

			bool m_Enabled = false;

			float32 m_Rate = 0.5f;	// Modulation rate in Hz
			float32 m_Depth = 3.0f;  // Modulation depth (0–1)
			float32 m_Mix = 50.0f;	// Wet/dry mix (0–100%)

		} m_Flange;

		/* @brief Dynamic range compressor — reduces volume peaks and evens out loudness. */
		struct Compressor {

			bool m_Enabled = false;

			float32 m_Threshold = -20.0f; // Level in dB above which compression is applied
			float32 m_Ratio = 4.0f;   // Compression ratio (e.g. 4:1 — 4dB in becomes 1dB out)
			float32 m_Attack = 10.0f;  // Time in ms for compression to engage after threshold is crossed
			float32 m_Release = 100.0f; // Time in ms for compression to disengage after signal drops below threshold
			float32 m_GainMakeup = 0.0f;   // Post-compression gain boost in dB to restore perceived loudness

		} m_Compressor;

		/* @brief Parametric equalizer — boosts or cuts a specific frequency band. */
		struct ParamEQ {

			bool m_Enabled = false;

			float32 m_Center = 1000.0f;	// Center frequency of the band in Hz
			float32 m_Bandwidth = 1.0f;		// Bandwidth in octaves around the center frequency
			float32 m_Gain = 0.0f;		// Gain in dB — positive = boost, negative = cut

		} m_ParamEQ;

	};

	/* @brief Backend-side effect handle storing DSP node pointers.
	* Owned by the AudioDevice. Do not access DSPs directly from outside the backend.
	*/
	struct AudioEffect {

		/* @brief Opaque DSP node handles owned by the audio backend (e.g. FMOD::DSP*).
		*  Cast to the backend-specific type inside the implementation.
		*/
		std::vector<void*> m_Dsps{};

	};

	namespace detail {
		/* @brief Internal tag used to distinguish lowpass from highpass during effect creation. */
		enum class FrequencyType : byte {
			Low,
			High
		};

	}

} // namespace lum::ahi