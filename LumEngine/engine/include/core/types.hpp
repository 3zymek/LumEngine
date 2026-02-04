////////////////////////////////
/// Types for LumEngine
////////////////////////////////
#pragma once
namespace lum {

	using float32 = float;
	using float64 = double;

	using int64 = int64_t;
	using int32 = int32_t;
	using int16 = int16_t;
	using int8	= int8_t;

	using uint64	= uint64_t;
	using uint32	= uint32_t;
	using uint16	= uint16_t;
	using uint8		= uint8_t;

	using char8		= char8_t;
	using char16	= char16_t;
	using char32	= char32_t;

	using ucharptr	= unsigned char*;
	using ccharptr	= const char*;
	using charptr	= char*;

	using usize = size_t;
	using isize = ptrdiff_t;
	using byte	= char;

	using bitfield	= uint32_t;

	using vptr	= void*;
	using cvptr = const void*;

	using string = std::string;
	using string_view = std::string_view;

}