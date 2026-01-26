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
		never
	};

	enum class StencilFlag : byte {
		zero,
		one,
		keep,
		replace,
		increment,
		increment_wrap,
		decrement,
		decrement_wrap,
		invert
	};

	// +
	struct RasterizerState {

		PolygonMode topologyMode = PolygonMode::fill;
		Face		topologyModeFaces = Face::front_back;

		// IMPLEMENT
		bool	bEnableDepthBias = false;
		float32 depthBiasFactor = 0.f;
		float32 depthBiasUnits = 0.f;

		constexpr bool operator==(const RasterizerState& other) const noexcept {
			return  
				topologyMode == other.topologyMode && 
				topologyModeFaces == other.topologyModeFaces && 
				bEnableDepthBias == other.bEnableDepthBias && 
				depthBiasFactor == other.depthBiasFactor && 
				depthBiasUnits == other.depthBiasUnits;
		}
		constexpr bool operator!=(const RasterizerState& other) const noexcept {
			return !(*this == other);
		}

	};

	// +
	struct DepthStencilState {

		struct Depth {

			bool		bEnabled = false;
			bool		bWriteToZBuffer = false;
			CompareFlag	compareFlag = CompareFlag::always;

			constexpr bool operator==(const Depth& other) const noexcept {
				return bWriteToZBuffer == other.bWriteToZBuffer && compareFlag == other.compareFlag;
			}
			constexpr bool operator!=(const Depth& other) const noexcept {
				return !(*this == other);
			}
		} depth;

		struct Stencil {

			// Drawing condition

			bool		bEnabled = false;
			int32		value = 0;
			CompareFlag compareFlag = CompareFlag::always;

			// Optional operation



		} stencil;

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