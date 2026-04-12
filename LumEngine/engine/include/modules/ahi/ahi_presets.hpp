//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Full-chain audio effect presets for AHI.
//          Each FAudioPreset configures the entire effect chain at once —
//          reverb, filters, echo, distortion, chorus, flange, and compression.
//          Use AudioPreset enum with AudioDevice::CreateEffect(preset).
//
//=============================================================================//
#pragma once

#include "ahi/core/ahi_effect.hpp"

namespace lum::ahi {

	/* @brief Named presets for full-chain audio effect configuration. */
	enum class EffectPreset : byte {

		// Off
		Off,

		// Environmental
		Underwater,
		Underground,
		Cave,
		Forest,
		Mountains,
		Plain,
		City,
		Alley,
		ParkingLot,
		SewerPipe,

		// Indoor rooms
		SmallRoom,
		Bathroom,
		Livingroom,
		Stoneroom,
		StoneCorridor,
		Auditorium,
		ConcertHall,
		Arena,
		Hangar,

		// Gameplay
		Radio,             // Walkie-talkie / military comms
		Phone,             // Telephone call
		MegaphoneMuffled,  // Distant PA / bullhorn
		InHelmet,          // Inside a closed helmet
		Explosion,         // Post-explosion ear ringing
		Horror,            // Low, oppressive, unsettling
		Dream,             // Washed out, distant, floaty

	};

	namespace detail {

		inline constexpr FAudioEffectDescriptor gkEffectPresetLookup[ ] = {

			// -------------------------------------------------------
			// Off
			// -------------------------------------------------------
			{},

			// -------------------------------------------------------
			// Underwater
			// -------------------------------------------------------
			{
				.mFreqPass = {
					.mLow = { true,  800.0f, 0.6f },
					.mHigh = { false, 0.0f,   0.0f },
				},
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 1500.0f,
					.mEarlyDelay = 7.0f,
					.mLateDelay = 11.0f,
					.mReferenceFreq = 1000.0f,
					.mDiffusion = 1.0f,
					.mWetLevel = -4.0f,
					.mDryLevel = -6.0f,
					.mDensity = 0.5f,
				},
				.mChorus = {
					.bEnabled = true,
					.mRate = 0.3f,
					.mDepth = 4.0f,
					.mMix = 25.0f,
				},
				.mCompressor = {
					.bEnabled = true,
					.mThreshold = -18.0f,
					.mRatio = 3.0f,
					.mAttack = 20.0f,
					.mRelease = 200.0f,
					.mGainMakeup = 2.0f,
				},
				.mParamEQ = {
					.bEnabled = true,
					.mCenter = 60.0f,    // Deep sub-bass depth
					.mBandwidth = 1.2f,
					.mGain = 5.0f,
				},
			},

			// -------------------------------------------------------
			// Underground
			// -------------------------------------------------------
			{
				.mFreqPass = {
					.mLow = { true, 120.0f, 1.8f },
					.mHigh = { false, 0.0f,  0.0f },
				},
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 10000.0f,
					.mEarlyDelay = 20.0f,
					.mLateDelay = 40.0f,
					.mReferenceFreq = 500.0f,
					.mDiffusion = 0.4f,
					.mWetLevel = -2.0f,
					.mDryLevel = -3.0f,
					.mDensity = 0.4f,
				},
				.mCompressor = {
					.bEnabled = true,
					.mThreshold = -24.0f,
					.mRatio = 6.0f,
					.mAttack = 5.0f,
					.mRelease = 300.0f,
					.mGainMakeup = 4.0f,
				},
				.mParamEQ = {
					.bEnabled = true,
					.mCenter = 70.0f,    // Heavy low-end rumble
					.mBandwidth = 1.0f,
					.mGain = 7.0f,
				},
			},

			// -------------------------------------------------------
			// Cave
			// -------------------------------------------------------
			{
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 3000.0f,
					.mEarlyDelay = 15.0f,
					.mLateDelay = 22.0f,
					.mReferenceFreq = 4000.0f,
					.mDiffusion = 0.5f,
					.mWetLevel = -6.0f,
					.mDryLevel = 0.0f,
					.mDensity = 1.0f,
				},
				.mParamEQ = {
					.bEnabled = true,
					.mCenter = 100.0f,   // Booming low end
					.mBandwidth = 1.0f,
					.mGain = 5.0f,
				},
			},

			// -------------------------------------------------------
			// Forest
			// -------------------------------------------------------
			{
				.mFreqPass = {
					.mLow = { true, 6000.0f, 0.4f },
					.mHigh = { false, 0.0f,   0.0f },
				},
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 1500.0f,
					.mEarlyDelay = 20.0f,
					.mLateDelay = 30.0f,
					.mReferenceFreq = 3000.0f,
					.mDiffusion = 0.3f,
					.mWetLevel = -14.0f,
					.mDryLevel = 0.0f,
					.mDensity = 0.7f,
				},
			},

			// -------------------------------------------------------
			// Mountains
			// -------------------------------------------------------
			{
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 8000.0f,
					.mEarlyDelay = 40.0f,
					.mLateDelay = 80.0f,
					.mReferenceFreq = 2500.0f,
					.mDiffusion = 0.2f,
					.mWetLevel = -8.0f,
					.mDryLevel = 0.0f,
					.mDensity = 0.5f,
				},
			},

			// -------------------------------------------------------
			// Plain
			// -------------------------------------------------------
			{
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 4000.0f,
					.mEarlyDelay = 30.0f,
					.mLateDelay = 60.0f,
					.mReferenceFreq = 3000.0f,
					.mDiffusion = 0.2f,
					.mWetLevel = -10.0f,
					.mDryLevel = 0.0f,
					.mDensity = 0.6f,
				},
			},

			// -------------------------------------------------------
			// City
			// -------------------------------------------------------
			{
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 1500.0f,
					.mEarlyDelay = 7.0f,
					.mLateDelay = 11.0f,
					.mReferenceFreq = 5000.0f,
					.mDiffusion = 0.3f,
					.mWetLevel = -10.0f,
					.mDryLevel = 0.0f,
					.mDensity = 1.0f,
				},
			},

			// -------------------------------------------------------
			// Alley
			// -------------------------------------------------------
			{
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 1500.0f,
					.mEarlyDelay = 7.0f,
					.mLateDelay = 11.0f,
					.mReferenceFreq = 5000.0f,
					.mDiffusion = 0.3f,
					.mWetLevel = -9.0f,
					.mDryLevel = 0.0f,
					.mDensity = 1.0f,
				},
			},

			// -------------------------------------------------------
			// ParkingLot
			// -------------------------------------------------------
			{
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 1700.0f,
					.mEarlyDelay = 8.0f,
					.mLateDelay = 12.0f,
					.mReferenceFreq = 5000.0f,
					.mDiffusion = 0.5f,
					.mWetLevel = -8.0f,
					.mDryLevel = 0.0f,
					.mDensity = 1.0f,
				},
			},

			// -------------------------------------------------------
			// SewerPipe
			// -------------------------------------------------------
			{
				.mFreqPass = {
					.mLow = { false, 0.0f,    0.0f },
					.mHigh = { true,  120.0f,  2.0f },
				},
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 2800.0f,
					.mEarlyDelay = 14.0f,
					.mLateDelay = 21.0f,
					.mReferenceFreq = 9000.0f,
					.mDiffusion = 0.9f,
					.mWetLevel = -3.0f,
					.mDryLevel = 0.0f,
					.mDensity = 0.8f,
				},
				.mParamEQ = {
					.bEnabled = true,
					.mCenter = 150.0f,   // Pipe resonance
					.mBandwidth = 0.8f,
					.mGain = 4.0f,
				},
			},

			// -------------------------------------------------------
			// SmallRoom
			// -------------------------------------------------------
			{
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 400.0f,
					.mEarlyDelay = 2.0f,
					.mLateDelay = 3.0f,
					.mReferenceFreq = 5000.0f,
					.mDiffusion = 0.5f,
					.mWetLevel = -10.0f,
					.mDryLevel = 0.0f,
					.mDensity = 1.0f,
				},
			},

			// -------------------------------------------------------
			// Bathroom
			// -------------------------------------------------------
			{
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 800.0f,
					.mEarlyDelay = 3.0f,
					.mLateDelay = 5.0f,
					.mReferenceFreq = 8000.0f,
					.mDiffusion = 1.0f,
					.mWetLevel = -4.0f,
					.mDryLevel = 0.0f,
					.mDensity = 1.0f,
				},
			},

			// -------------------------------------------------------
			// Livingroom
			// -------------------------------------------------------
			{
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 500.0f,
					.mEarlyDelay = 3.0f,
					.mLateDelay = 4.0f,
					.mReferenceFreq = 5000.0f,
					.mDiffusion = 0.4f,
					.mWetLevel = -11.0f,
					.mDryLevel = 0.0f,
					.mDensity = 1.0f,
				},
			},

			// -------------------------------------------------------
			// Stoneroom
			// -------------------------------------------------------
			{
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 2300.0f,
					.mEarlyDelay = 12.0f,
					.mLateDelay = 17.0f,
					.mReferenceFreq = 5000.0f,
					.mDiffusion = 0.6f,
					.mWetLevel = -6.0f,
					.mDryLevel = 0.0f,
					.mDensity = 1.0f,
				},
				.mParamEQ = {
					.bEnabled = true,
					.mCenter = 120.0f,   // Stone resonance
					.mBandwidth = 0.8f,
					.mGain = 3.0f,
				},
			},

			// -------------------------------------------------------
			// StoneCorridor
			// -------------------------------------------------------
			{
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 2700.0f,
					.mEarlyDelay = 13.0f,
					.mLateDelay = 20.0f,
					.mReferenceFreq = 5000.0f,
					.mDiffusion = 0.8f,
					.mWetLevel = -6.0f,
					.mDryLevel = 0.0f,
					.mDensity = 1.0f,
				},
				.mParamEQ = {
					.bEnabled = true,
					.mCenter = 120.0f,   // Stone resonance
					.mBandwidth = 0.8f,
					.mGain = 3.0f,
				},
			},

			// -------------------------------------------------------
			// Auditorium
			// -------------------------------------------------------
			{
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 4300.0f,
					.mEarlyDelay = 20.0f,
					.mLateDelay = 30.0f,
					.mReferenceFreq = 5000.0f,
					.mDiffusion = 0.8f,
					.mWetLevel = -6.0f,
					.mDryLevel = 0.0f,
					.mDensity = 1.0f,
				},
			},

			// -------------------------------------------------------
			// ConcertHall
			// -------------------------------------------------------
			{
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 2800.0f,
					.mEarlyDelay = 15.0f,
					.mLateDelay = 35.0f,
					.mReferenceFreq = 5000.0f,
					.mDiffusion = 0.9f,
					.mWetLevel = -5.0f,
					.mDryLevel = 0.0f,
					.mDensity = 1.0f,
				},
			},

			// -------------------------------------------------------
			// Arena
			// -------------------------------------------------------
			{
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 7200.0f,
					.mEarlyDelay = 20.0f,
					.mLateDelay = 30.0f,
					.mReferenceFreq = 5000.0f,
					.mDiffusion = 0.8f,
					.mWetLevel = -4.0f,
					.mDryLevel = 0.0f,
					.mDensity = 1.0f,
				},
				.mParamEQ = {
					.bEnabled = true,
					.mCenter = 80.0f,    // Crowd low-end weight
					.mBandwidth = 1.0f,
					.mGain = 3.0f,
				},
			},

			// -------------------------------------------------------
			// Hangar
			// -------------------------------------------------------
			{
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 10000.0f,
					.mEarlyDelay = 20.0f,
					.mLateDelay = 30.0f,
					.mReferenceFreq = 5000.0f,
					.mDiffusion = 0.5f,
					.mWetLevel = -3.0f,
					.mDryLevel = 0.0f,
					.mDensity = 1.0f,
				},
				.mParamEQ = {
					.bEnabled = true,
					.mCenter = 90.0f,    // Metal structure resonance
					.mBandwidth = 1.2f,
					.mGain = 4.0f,
				},
			},

			// -------------------------------------------------------
			// Radio  (walkie-talkie / military comms)
			// -------------------------------------------------------
			{
				.mFreqPass = {
					.mLow = { true,  3000.0f, 2.5f },
					.mHigh = { true,  800.0f,  2.5f },
				},
				.mDistortion = {
					.bEnabled = true,
					.mLevel = 0.25f,
				},
				.mCompressor = {
					.bEnabled = true,
					.mThreshold = -12.0f,
					.mRatio = 8.0f,
					.mAttack = 2.0f,
					.mRelease = 80.0f,
					.mGainMakeup = 3.0f,
				},
			},

			// -------------------------------------------------------
			// Phone
			// -------------------------------------------------------
			{
				.mFreqPass = {
					.mLow = { true,  3400.0f, 3.0f },
					.mHigh = { true,  300.0f,  3.0f },
				},
				.mDistortion = {
					.bEnabled = true,
					.mLevel = 0.35f,
				},
				.mCompressor = {
					.bEnabled = true,
					.mThreshold = -10.0f,
					.mRatio = 10.0f,
					.mAttack = 1.0f,
					.mRelease = 60.0f,
					.mGainMakeup = 2.0f,
				},
			},

			// -------------------------------------------------------
			// MegaphoneMuffled
			// -------------------------------------------------------
			{
				.mFreqPass = {
					.mLow = { true,  2000.0f, 1.5f },
					.mHigh = { true,  500.0f,  1.0f },
				},
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 300.0f,
					.mEarlyDelay = 5.0f,
					.mLateDelay = 8.0f,
					.mReferenceFreq = 2000.0f,
					.mDiffusion = 0.6f,
					.mWetLevel = -8.0f,
					.mDryLevel = 0.0f,
					.mDensity = 0.8f,
				},
				.mDistortion = {
					.bEnabled = true,
					.mLevel = 0.5f,
				},
			},

			// -------------------------------------------------------
			// InHelmet
			// -------------------------------------------------------
			{
				.mFreqPass = {
					.mLow = { true,  3500.0f, 0.5f },
					.mHigh = { false, 0.0f,    0.0f },
				},
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 150.0f,
					.mEarlyDelay = 1.0f,
					.mLateDelay = 2.0f,
					.mReferenceFreq = 3000.0f,
					.mDiffusion = 0.9f,
					.mWetLevel = -8.0f,
					.mDryLevel = -2.0f,
					.mDensity = 1.0f,
				},
				.mCompressor = {
					.bEnabled = true,
					.mThreshold = -15.0f,
					.mRatio = 3.0f,
					.mAttack = 5.0f,
					.mRelease = 100.0f,
					.mGainMakeup = 1.0f,
				},
				.mParamEQ = {
					.bEnabled = true,
					.mCenter = 200.0f,   // Helmet resonance / muffled thump
					.mBandwidth = 0.8f,
					.mGain = 4.0f,
				},
			},

			// -------------------------------------------------------
			// Explosion  (post-explosion ear ringing)
			// -------------------------------------------------------
			{
				.mFreqPass = {
					.mLow = { true,  2000.0f, 0.3f },
					.mHigh = { true,  200.0f,  0.5f },
				},
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 800.0f,
					.mEarlyDelay = 5.0f,
					.mLateDelay = 15.0f,
					.mReferenceFreq = 1500.0f,
					.mDiffusion = 0.7f,
					.mWetLevel = -5.0f,
					.mDryLevel = -8.0f,
					.mDensity = 0.8f,
				},
				.mCompressor = {
					.bEnabled = true,
					.mThreshold = -6.0f,
					.mRatio = 20.0f,
					.mAttack = 0.5f,
					.mRelease = 500.0f,
					.mGainMakeup = 0.0f,
				},
				.mParamEQ = {
					.bEnabled = true,
					.mCenter = 80.0f,    // Concussive low-end thump
					.mBandwidth = 1.5f,
					.mGain = 8.0f,
				},
			},

			// -------------------------------------------------------
			// Horror
			// -------------------------------------------------------
			{
				.mFreqPass = {
					.mLow = { true,  5000.0f, 0.3f },
					.mHigh = { false, 0.0f,    0.0f },
				},
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 6000.0f,
					.mEarlyDelay = 25.0f,
					.mLateDelay = 50.0f,
					.mReferenceFreq = 2000.0f,
					.mDiffusion = 0.6f,
					.mWetLevel = -5.0f,
					.mDryLevel = -2.0f,
					.mDensity = 0.6f,
				},
				.mCompressor = {
					.bEnabled = true,
					.mThreshold = -20.0f,
					.mRatio = 4.0f,
					.mAttack = 30.0f,
					.mRelease = 400.0f,
					.mGainMakeup = 1.0f,
				},
				.mParamEQ = {
					.bEnabled = true,
					.mCenter = 60.0f,    // Oppressive sub-bass dread
					.mBandwidth = 1.5f,
					.mGain = 6.0f,
				},
			},

			// -------------------------------------------------------
			// Dream
			// -------------------------------------------------------
			{
				.mFreqPass = {
					.mLow = { true,  7000.0f, 0.4f },
					.mHigh = { false, 0.0f,    0.0f },
				},
				.mReverb = {
					.bEnabled = true,
					.mDecayTime = 5000.0f,
					.mEarlyDelay = 20.0f,
					.mLateDelay = 40.0f,
					.mReferenceFreq = 4000.0f,
					.mDiffusion = 1.0f,
					.mWetLevel = -3.0f,
					.mDryLevel = -4.0f,
					.mDensity = 1.0f,
				},
				.mChorus = {
					.bEnabled = true,
					.mRate = 0.2f,
					.mDepth = 6.0f,
					.mMix = 40.0f,
				},
				.mFlange = {
					.bEnabled = true,
					.mRate = 0.15f,
					.mDepth = 0.4f,
					.mMix = 30.0f,
				},
			},

		}; // gkEffectPresetLookup

	} // namespace detail

	inline ahi::FAudioEffectDescriptor GetPreset( EffectPreset preset ) {
		return detail::gkEffectPresetLookup[ ToUnderlyingEnum( preset ) ];
	}

} // namespace lum::ahi