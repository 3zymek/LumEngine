#pragma once

#include "core/core_common.hpp"
#include "ahi/ahi_common.hpp"

namespace lum {

	enum class SoundCategory : byte {
		SFX,
		SFX3D,
		Music,
		Voice,
		Ambient,
	};

	namespace detail {

		inline SoundCategory ParseSoundCategory( StringView cat ) {

			if (ToLower( cat ) == "sfx") { return SoundCategory::SFX; }
			if (ToLower( cat ) == "sfx3d") { return SoundCategory::SFX3D; }
			if (ToLower( cat ) == "music") { return SoundCategory::Music; }
			if (ToLower( cat ) == "voice") { return SoundCategory::Voice; }
			if (ToLower( cat ) == "ambient") { return SoundCategory::Ambient; }

			LUM_LOG_WARN( "Sound category %s doesn't exist", cat.data() );

			return SoundCategory::Music;

		}

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

	inline constexpr Flags<ahi::SoundFlag> GetSoundFlags( SoundCategory category ) {
		return detail::gkSoundCategoryFlags[ static_cast< byte >(category) ];
	}

}