//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Full-chain audio effect presets for AHI.
//          Each EffectPreset configures the entire effect chain at once —
//          reverb, filters, echo, distortion, chorus, flange, and compression.
//          Use EffectPreset enum with AudioDevice::CreateEffect(preset).
//
//=============================================================================//
#pragma once

#include "Ahi/Core/AhiEffect.hpp"

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

		inline constexpr AudioEffectCreateInfo k_EffectPresetLookup[ ] = {

			// -------------------------------------------------------
			// Off
			// -------------------------------------------------------
			{},

			// -------------------------------------------------------
			// Underwater
			// -------------------------------------------------------
			{
				.m_FreqPass = {
					.m_Low = { true,  800.0f, 0.6f },
					.m_High = { false, 0.0f,   0.0f },
				},
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 1500.0f,
					.m_EarlyDelay = 7.0f,
					.m_LateDelay = 11.0f,
					.m_ReferenceFreq = 1000.0f,
					.m_Diffusion = 1.0f,
					.m_WetLevel = -4.0f,
					.m_DryLevel = -6.0f,
					.m_Density = 0.5f,
				},
				.m_Chorus = {
					.m_Enabled = true,
					.m_Rate = 0.3f,
					.m_Depth = 4.0f,
					.m_Mix = 25.0f,
				},
				.m_Compressor = {
					.m_Enabled = true,
					.m_Threshold = -18.0f,
					.m_Ratio = 3.0f,
					.m_Attack = 20.0f,
					.m_Release = 200.0f,
					.m_GainMakeup = 2.0f,
				},
				.m_ParamEQ = {
					.m_Enabled = true,
					.m_Center = 60.0f,    // Deep sub-bass depth
					.m_Bandwidth = 1.2f,
					.m_Gain = 5.0f,
				},
			},

			// -------------------------------------------------------
			// Underground
			// -------------------------------------------------------
			{
				.m_FreqPass = {
					.m_Low = { true, 120.0f, 1.8f },
					.m_High = { false, 0.0f,  0.0f },
				},
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 10000.0f,
					.m_EarlyDelay = 20.0f,
					.m_LateDelay = 40.0f,
					.m_ReferenceFreq = 500.0f,
					.m_Diffusion = 0.4f,
					.m_WetLevel = -2.0f,
					.m_DryLevel = -3.0f,
					.m_Density = 0.4f,
				},
				.m_Compressor = {
					.m_Enabled = true,
					.m_Threshold = -24.0f,
					.m_Ratio = 6.0f,
					.m_Attack = 5.0f,
					.m_Release = 300.0f,
					.m_GainMakeup = 4.0f,
				},
				.m_ParamEQ = {
					.m_Enabled = true,
					.m_Center = 70.0f,    // Heavy low-end rumble
					.m_Bandwidth = 1.0f,
					.m_Gain = 7.0f,
				},
			},

			// -------------------------------------------------------
			// Cave
			// -------------------------------------------------------
			{
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 3000.0f,
					.m_EarlyDelay = 15.0f,
					.m_LateDelay = 22.0f,
					.m_ReferenceFreq = 4000.0f,
					.m_Diffusion = 0.5f,
					.m_WetLevel = -6.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 1.0f,
				},
				.m_ParamEQ = {
					.m_Enabled = true,
					.m_Center = 100.0f,   // Booming low end
					.m_Bandwidth = 1.0f,
					.m_Gain = 5.0f,
				},
			},

			// -------------------------------------------------------
			// Forest
			// -------------------------------------------------------
			{
				.m_FreqPass = {
					.m_Low = { true, 6000.0f, 0.4f },
					.m_High = { false, 0.0f,   0.0f },
				},
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 1500.0f,
					.m_EarlyDelay = 20.0f,
					.m_LateDelay = 30.0f,
					.m_ReferenceFreq = 3000.0f,
					.m_Diffusion = 0.3f,
					.m_WetLevel = -14.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 0.7f,
				},
			},

			// -------------------------------------------------------
			// Mountains
			// -------------------------------------------------------
			{
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 8000.0f,
					.m_EarlyDelay = 40.0f,
					.m_LateDelay = 80.0f,
					.m_ReferenceFreq = 2500.0f,
					.m_Diffusion = 0.2f,
					.m_WetLevel = -8.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 0.5f,
				},
			},

			// -------------------------------------------------------
			// Plain
			// -------------------------------------------------------
			{
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 4000.0f,
					.m_EarlyDelay = 30.0f,
					.m_LateDelay = 60.0f,
					.m_ReferenceFreq = 3000.0f,
					.m_Diffusion = 0.2f,
					.m_WetLevel = -10.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 0.6f,
				},
			},

			// -------------------------------------------------------
			// City
			// -------------------------------------------------------
			{
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 1500.0f,
					.m_EarlyDelay = 7.0f,
					.m_LateDelay = 11.0f,
					.m_ReferenceFreq = 5000.0f,
					.m_Diffusion = 0.3f,
					.m_WetLevel = -10.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 1.0f,
				},
			},

			// -------------------------------------------------------
			// Alley
			// -------------------------------------------------------
			{
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 1500.0f,
					.m_EarlyDelay = 7.0f,
					.m_LateDelay = 11.0f,
					.m_ReferenceFreq = 5000.0f,
					.m_Diffusion = 0.3f,
					.m_WetLevel = -9.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 1.0f,
				},
			},

			// -------------------------------------------------------
			// ParkingLot
			// -------------------------------------------------------
			{
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 1700.0f,
					.m_EarlyDelay = 8.0f,
					.m_LateDelay = 12.0f,
					.m_ReferenceFreq = 5000.0f,
					.m_Diffusion = 0.5f,
					.m_WetLevel = -8.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 1.0f,
				},
			},

			// -------------------------------------------------------
			// SewerPipe
			// -------------------------------------------------------
			{
				.m_FreqPass = {
					.m_Low = { false, 0.0f,    0.0f },
					.m_High = { true,  120.0f,  2.0f },
				},
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 2800.0f,
					.m_EarlyDelay = 14.0f,
					.m_LateDelay = 21.0f,
					.m_ReferenceFreq = 9000.0f,
					.m_Diffusion = 0.9f,
					.m_WetLevel = -3.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 0.8f,
				},
				.m_ParamEQ = {
					.m_Enabled = true,
					.m_Center = 150.0f,
					.m_Bandwidth = 0.8f,
					.m_Gain = 4.0f,
				},
			},

			// -------------------------------------------------------
			// SmallRoom
			// -------------------------------------------------------
			{
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 400.0f,
					.m_EarlyDelay = 2.0f,
					.m_LateDelay = 3.0f,
					.m_ReferenceFreq = 5000.0f,
					.m_Diffusion = 0.5f,
					.m_WetLevel = -10.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 1.0f,
				},
			},

			// -------------------------------------------------------
			// Bathroom
			// -------------------------------------------------------
			{
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 800.0f,
					.m_EarlyDelay = 3.0f,
					.m_LateDelay = 5.0f,
					.m_ReferenceFreq = 8000.0f,
					.m_Diffusion = 1.0f,
					.m_WetLevel = -4.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 1.0f,
				},
			},

			// -------------------------------------------------------
			// Livingroom
			// -------------------------------------------------------
			{
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 500.0f,
					.m_EarlyDelay = 3.0f,
					.m_LateDelay = 4.0f,
					.m_ReferenceFreq = 5000.0f,
					.m_Diffusion = 0.4f,
					.m_WetLevel = -11.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 1.0f,
				},
			},

			// -------------------------------------------------------
			// Stoneroom
			// -------------------------------------------------------
			{
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 2300.0f,
					.m_EarlyDelay = 12.0f,
					.m_LateDelay = 17.0f,
					.m_ReferenceFreq = 5000.0f,
					.m_Diffusion = 0.6f,
					.m_WetLevel = -6.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 1.0f,
				},
				.m_ParamEQ = {
					.m_Enabled = true,
					.m_Center = 120.0f,
					.m_Bandwidth = 0.8f,
					.m_Gain = 3.0f,
				},
			},

			// -------------------------------------------------------
			// StoneCorridor
			// -------------------------------------------------------
			{
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 2700.0f,
					.m_EarlyDelay = 13.0f,
					.m_LateDelay = 20.0f,
					.m_ReferenceFreq = 5000.0f,
					.m_Diffusion = 0.8f,
					.m_WetLevel = -6.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 1.0f,
				},
				.m_ParamEQ = {
					.m_Enabled = true,
					.m_Center = 120.0f,
					.m_Bandwidth = 0.8f,
					.m_Gain = 3.0f,
				},
			},

			// -------------------------------------------------------
			// Auditorium
			// -------------------------------------------------------
			{
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 4300.0f,
					.m_EarlyDelay = 20.0f,
					.m_LateDelay = 30.0f,
					.m_ReferenceFreq = 5000.0f,
					.m_Diffusion = 0.8f,
					.m_WetLevel = -6.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 1.0f,
				},
			},

			// -------------------------------------------------------
			// ConcertHall
			// -------------------------------------------------------
			{
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 2800.0f,
					.m_EarlyDelay = 15.0f,
					.m_LateDelay = 35.0f,
					.m_ReferenceFreq = 5000.0f,
					.m_Diffusion = 0.9f,
					.m_WetLevel = -5.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 1.0f,
				},
			},

			// -------------------------------------------------------
			// Arena
			// -------------------------------------------------------
			{
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 7200.0f,
					.m_EarlyDelay = 20.0f,
					.m_LateDelay = 30.0f,
					.m_ReferenceFreq = 5000.0f,
					.m_Diffusion = 0.8f,
					.m_WetLevel = -4.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 1.0f,
				},
				.m_ParamEQ = {
					.m_Enabled = true,
					.m_Center = 80.0f,
					.m_Bandwidth = 1.0f,
					.m_Gain = 3.0f,
				},
			},

			// -------------------------------------------------------
			// Hangar
			// -------------------------------------------------------
			{
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 10000.0f,
					.m_EarlyDelay = 20.0f,
					.m_LateDelay = 30.0f,
					.m_ReferenceFreq = 5000.0f,
					.m_Diffusion = 0.5f,
					.m_WetLevel = -3.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 1.0f,
				},
				.m_ParamEQ = {
					.m_Enabled = true,
					.m_Center = 90.0f,
					.m_Bandwidth = 1.2f,
					.m_Gain = 4.0f,
				},
			},

			// -------------------------------------------------------
			// Radio  (walkie-talkie / military comms)
			// -------------------------------------------------------
			{
				.m_FreqPass = {
					.m_Low = { true,  3000.0f, 2.5f },
					.m_High = { true,  800.0f,  2.5f },
				},
				.m_Distortion = {
					.m_Enabled = true,
					.m_Level = 0.25f,
				},
				.m_Compressor = {
					.m_Enabled = true,
					.m_Threshold = -12.0f,
					.m_Ratio = 8.0f,
					.m_Attack = 2.0f,
					.m_Release = 80.0f,
					.m_GainMakeup = 3.0f,
				},
			},

			// -------------------------------------------------------
			// Phone
			// -------------------------------------------------------
			{
				.m_FreqPass = {
					.m_Low = { true,  3400.0f, 3.0f },
					.m_High = { true,  300.0f,  3.0f },
				},
				.m_Distortion = {
					.m_Enabled = true,
					.m_Level = 0.35f,
				},
				.m_Compressor = {
					.m_Enabled = true,
					.m_Threshold = -10.0f,
					.m_Ratio = 10.0f,
					.m_Attack = 1.0f,
					.m_Release = 60.0f,
					.m_GainMakeup = 2.0f,
				},
			},

			// -------------------------------------------------------
			// MegaphoneMuffled
			// -------------------------------------------------------
			{
				.m_FreqPass = {
					.m_Low = { true,  2000.0f, 1.5f },
					.m_High = { true,  500.0f,  1.0f },
				},
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 300.0f,
					.m_EarlyDelay = 5.0f,
					.m_LateDelay = 8.0f,
					.m_ReferenceFreq = 2000.0f,
					.m_Diffusion = 0.6f,
					.m_WetLevel = -8.0f,
					.m_DryLevel = 0.0f,
					.m_Density = 0.8f,
				},
				.m_Distortion = {
					.m_Enabled = true,
					.m_Level = 0.5f,
				},
			},

			// -------------------------------------------------------
			// InHelmet
			// -------------------------------------------------------
			{
				.m_FreqPass = {
					.m_Low = { true,  3500.0f, 0.5f },
					.m_High = { false, 0.0f,    0.0f },
				},
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 150.0f,
					.m_EarlyDelay = 1.0f,
					.m_LateDelay = 2.0f,
					.m_ReferenceFreq = 3000.0f,
					.m_Diffusion = 0.9f,
					.m_WetLevel = -8.0f,
					.m_DryLevel = -2.0f,
					.m_Density = 1.0f,
				},
				.m_Compressor = {
					.m_Enabled = true,
					.m_Threshold = -15.0f,
					.m_Ratio = 3.0f,
					.m_Attack = 5.0f,
					.m_Release = 100.0f,
					.m_GainMakeup = 1.0f,
				},
				.m_ParamEQ = {
					.m_Enabled = true,
					.m_Center = 200.0f,
					.m_Bandwidth = 0.8f,
					.m_Gain = 4.0f,
				},
			},

			// -------------------------------------------------------
			// Explosion  (post-explosion ear ringing)
			// -------------------------------------------------------
			{
				.m_FreqPass = {
					.m_Low = { true,  2000.0f, 0.3f },
					.m_High = { true,  200.0f,  0.5f },
				},
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 800.0f,
					.m_EarlyDelay = 5.0f,
					.m_LateDelay = 15.0f,
					.m_ReferenceFreq = 1500.0f,
					.m_Diffusion = 0.7f,
					.m_WetLevel = -5.0f,
					.m_DryLevel = -8.0f,
					.m_Density = 0.8f,
				},
				.m_Compressor = {
					.m_Enabled = true,
					.m_Threshold = -6.0f,
					.m_Ratio = 20.0f,
					.m_Attack = 0.5f,
					.m_Release = 500.0f,
					.m_GainMakeup = 0.0f,
				},
				.m_ParamEQ = {
					.m_Enabled = true,
					.m_Center = 80.0f,
					.m_Bandwidth = 1.5f,
					.m_Gain = 8.0f,
				},
			},

			// -------------------------------------------------------
			// Horror
			// -------------------------------------------------------
			{
				.m_FreqPass = {
					.m_Low = { true,  5000.0f, 0.3f },
					.m_High = { false, 0.0f,    0.0f },
				},
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 6000.0f,
					.m_EarlyDelay = 25.0f,
					.m_LateDelay = 50.0f,
					.m_ReferenceFreq = 2000.0f,
					.m_Diffusion = 0.6f,
					.m_WetLevel = -5.0f,
					.m_DryLevel = -2.0f,
					.m_Density = 0.6f,
				},
				.m_Compressor = {
					.m_Enabled = true,
					.m_Threshold = -20.0f,
					.m_Ratio = 4.0f,
					.m_Attack = 30.0f,
					.m_Release = 400.0f,
					.m_GainMakeup = 1.0f,
				},
				.m_ParamEQ = {
					.m_Enabled = true,
					.m_Center = 60.0f,
					.m_Bandwidth = 1.5f,
					.m_Gain = 6.0f,
				},
			},

			// -------------------------------------------------------
			// Dream
			// -------------------------------------------------------
			{
				.m_FreqPass = {
					.m_Low = { true,  7000.0f, 0.4f },
					.m_High = { false, 0.0f,    0.0f },
				},
				.m_Reverb = {
					.m_Enabled = true,
					.m_DecayTime = 5000.0f,
					.m_EarlyDelay = 20.0f,
					.m_LateDelay = 40.0f,
					.m_ReferenceFreq = 4000.0f,
					.m_Diffusion = 1.0f,
					.m_WetLevel = -3.0f,
					.m_DryLevel = -4.0f,
					.m_Density = 1.0f,
				},
				.m_Chorus = {
					.m_Enabled = true,
					.m_Rate = 0.2f,
					.m_Depth = 6.0f,
					.m_Mix = 40.0f,
				},
				.m_Flange = {
					.m_Enabled = true,
					.m_Rate = 0.15f,
					.m_Depth = 0.4f,
					.m_Mix = 30.0f,
				},
			},

		}; // kEffectPresetLookup

	} // namespace detail

	/* @brief Returns the AudioEffectCreateInfo descriptor for a given preset.
	*  @param preset Named preset to look up.
	*  @return Fully configured effect descriptor ready for AudioDevice::CreateEffect().
	*/
	inline ahi::AudioEffectCreateInfo GetPreset( EffectPreset preset ) {
		return detail::k_EffectPresetLookup[ ToUnderlyingEnum( preset ) ];
	}

} // namespace lum::ahi