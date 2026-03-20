//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Sound category definitions and flag mappings for AHI.
//
//=============================================================================//
#pragma once

#include "core/core_common.hpp"
#include "ahi/ahi_common.hpp"

namespace lum {

	// High-level sound category used to classify audio assets.
	// Each category maps to a fixed set of AHI sound flags (see gkSoundCategoryFlags).
	enum class SoundCategory : byte {
		SFX,     // Short, non-positional sound effects (gunshots, UI clicks, etc.)
		SFX3D,   // Positional sound effects with inverse distance rolloff
		Music,   // Streamed background music, non-positional
		Voice,   // Streamed dialogue, non-positional, unique (one instance at a time)
		Ambient, // Streamed positional ambience (wind, water, etc.)
	};

	namespace detail {

		// Parses a string into a SoundCategory. Case-insensitive.
		// Falls back to Music and logs a warning on unknown input.
		inline SoundCategory ParseSoundCategory( StringView cat ) {

			if (ToLower( cat ) == "sfx") { return SoundCategory::SFX; }
			if (ToLower( cat ) == "sfx3d") { return SoundCategory::SFX3D; }
			if (ToLower( cat ) == "music") { return SoundCategory::Music; }
			if (ToLower( cat ) == "voice") { return SoundCategory::Voice; }
			if (ToLower( cat ) == "ambient") { return SoundCategory::Ambient; }

			LUM_LOG_WARN( "Sound category %s doesn't exist", cat.data( ) );

			return SoundCategory::Music;

		}

		// Compile-time flag table indexed by SoundCategory.
		// Order must match SoundCategory enum declaration exactly.
		inline constexpr Flags<ahi::SoundFlag> gkSoundCategoryFlags[ ] = {
			// SFX
			ahi::SoundFlag::CreateSample | ahi::SoundFlag::Flat2D,
			// SFX3D
			ahi::SoundFlag::CreateSample | ahi::SoundFlag::Spatial3D | ahi::SoundFlag::InverseRolloff,
			// Music
			ahi::SoundFlag::CreateStream | ahi::SoundFlag::Flat2D,
			// Voice
			ahi::SoundFlag::CreateStream | ahi::SoundFlag::Flat2D | ahi::SoundFlag::Unique,
			// Ambient
			ahi::SoundFlag::CreateStream | ahi::SoundFlag::Spatial3D | ahi::SoundFlag::InverseRolloff,
		};

	} // namespace detail

	// Returns the AHI sound flags associated with the given category.
	inline constexpr Flags<ahi::SoundFlag> GetSoundFlags( SoundCategory category ) {
		return detail::gkSoundCategoryFlags[ static_cast< byte >(category) ];
	}

} // namespace lum