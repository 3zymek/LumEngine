#pragma once
#include "rhi/rhi_common.hpp"

namespace lum::rhi {
	
	enum class CompareFlag : byte {
		less,		// Draws only pixels with Z < bufferZ
		less_equal,	// Draws only pixels with Z <= bufferZ
		greater,	// Draws only pixels with Z > bufferZ
		greater_equal,
		not_equal,
		always,		// Always draws a pixel
		never,
	};

	enum class StencilFlag : byte {
		keep,
		zero,
		replace
	};

	// +
	struct RasterizerState {

		PolygonMode polygonMode = PolygonMode::fill;
		Face		polygonModeFaces = Face::front_back;

		bool	bEnableDepthBias = false;
		float32 depthBiasFactor = 0.f;
		float32 depthBiasUnits = 0.f;

	};

	// +
	struct DepthState {

		bool		bEnabled = false;
		bool		bWriteToZBuffer = false;
		CompareFlag	compare_flag = CompareFlag::always;

		constexpr bool operator==(const DepthState& other) const noexcept {
			return bWriteToZBuffer == other.bWriteToZBuffer && compare_flag == other.compare_flag;
		}
		constexpr bool operator!=(const DepthState& other) const noexcept {
			return !(*this == other);
		}


	};

	struct CullState {
		
		bool bEnabled = false;
		Face face = Face::back;
		WindingOrder windingOrder = WindingOrder::counter_clockwise;

		constexpr bool operator==(const CullState& other) const noexcept {
			return face == other.face && windingOrder != other.windingOrder;
		}
		constexpr bool operator!=(const CullState& other) const noexcept {
			return !(*this == other);
		}

	};

	// -
	struct StencilState {

		bool	bEnabled = false;
		float32 value = 1.0f;

	};

	// +
	struct ScissorState {

		bool	bEnabled = false;
		int32	x = 0;
		int32	y = 0;
		int32	width = 0;
		int32	height = 0;

		constexpr bool operator==(const ScissorState& other) const noexcept {
			return x == other.x && y == other.y && width == other.width && height == other.height;
		}
		constexpr bool operator!=(const ScissorState& other) const noexcept {
			return !(*this == other);
		}


	};

}