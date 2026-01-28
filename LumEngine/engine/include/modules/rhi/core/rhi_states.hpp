#pragma once
#include "rhi/rhi_common.hpp"
namespace lum::rhi {

	enum class CompareFlag : bitfield {
		Equal,			// Passes if the incoming depth value is equal to the stored depth value.
		NotEqual,		// Passes if the incoming depth value is not equal to the stored depth value.
		LessEqual,		// Passes if the incoming depth value is less than or equal to the stored depth value.
		GreaterEqual,	// Passes if the incoming depth value is greater than or equal to the stored depth value.
		Less,			// Passes if the incoming depth value is less than the stored depth value.
		Greater,		// Passes if the incoming depth value is greater than the stored depth value.
		Always,			// Always passes.
		Never,			// Never passes.
	};

	// +
	struct RasterizerState {

		PolygonMode topologyMode = PolygonMode::Fill;
		Face		topologyModeFaces = Face::FrontBack;

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

	// Front and back stencil test actions.
	enum class StencilOp : bitfield {
		Zero,			// Sets the stencil buffer value to 0. 
		One,			// Sets the stencil buffer value to 1. 
		Keep,			// Keeps the current value. 
		Replace,		// Sets the stencil buffer value to ref, as specified by SetStencilReferenceValue( x ).
		Increment,		// Increments the current stencil buffer value. Clamps to the maximum representable unsigned value. 
		IncrementWrap,	// Increments the current stencil buffer value. Wraps stencil buffer value to zero when incrementing the maximum representable unsigned value. 
		Decrement,		// Decrements the current stencil buffer value. Clamps to 0. 
		DecrementWrap,	// Decrements the current stencil buffer value. Wraps stencil buffer value to the maximum representable unsigned value when decrementing a stencil buffer value of zero. 
		Invert			// Bitwise inverts the current stencil buffer value.
	};

	struct StencilFaceState {

		// Specifies the reference value for the stencil test. 
		// ref is clamped to the range [0,2n−1], where n is the number of bitplanes in the stencil buffer. 
		// The initial value is 0. 
		int32 reference = 0;

		// Specifies a mask that is ANDed with both the reference value 
		// and the stored stencil value when the test is done. 
		// The initial value is all 1's. 
		uint32 readMask = 1;

		// Specifies a bit mask to enable and disable writing of individual bits in the stencil planes. 
		// Initially, the mask is all 1's. 
		uint32 writeMask = 1;

		// Specifies the test function. 
		// The initial value is CompareFlag::Always. 
		CompareFlag compareFlag = CompareFlag::Always;

		// Specifies the action to take when the stencil test fails. 
		// The initial value is StencilOp::Keep. 
		StencilOp stencilFailOp = StencilOp::Keep;

		// Specifies the stencil action when the stencil test passes, but the depth test fails. dpfail accepts the same symbolic 
		// constants as sfail. 
		// The initial value is StencilOp::Keep.
		StencilOp depthFailOp = StencilOp::Keep;

		// Specifies the stencil action when both the stencil test and the depth test pass, or when the stencil test passes and 
		// either there is no depth buffer or depth testing is not enabled. dppass accepts the same symbolic constants as sfail. 
		// The initial value is StencilOp::Keep. 
		StencilOp dpPass = StencilOp::Keep;


		constexpr bool operator==(const StencilFaceState& a) {
			return reference == a.reference && readMask == a.readMask && writeMask == a.writeMask && compareFlag == a.compareFlag && stencilFailOp == a.stencilFailOp && depthFailOp == a.depthFailOp && dpPass == a.dpPass;
		}

	};

	struct DepthStencilState {

		struct Depth {

			// Specifies whether the depth buffer is enabled for writing. 
			// If flag is false, depth buffer writing is disabled. Otherwise, it is enabled. 
			// The initial value is false.
			bool bEnabled = false;

			// Specifies whether the depth buffer is enabled for writing. 
			// If flag is false, depth buffer writing is disabled. Otherwise, it is enabled. 
			// The initial value is true.
			bool bWriteToZBuffer = true;

			// Specifies the depth comparison function. 
			// The initial value is CompareFlag::Less. 
			CompareFlag	compareFlag = CompareFlag::Less;

			constexpr bool operator==(const Depth& other) const noexcept {
				return bWriteToZBuffer == other.bWriteToZBuffer && compareFlag == other.compareFlag;
			}
			constexpr bool operator!=(const Depth& other) const noexcept {
				return !(*this == other);
			}
		} depth;

		struct Stencil {

			// Defines if stencil test is enabled.
			// The initial value is false.
			bool bEnabled = false;

			// Stencil state for front
			StencilFaceState front{};

			// Stencil state for back
			StencilFaceState back{};

			// Helper for creating same stencil face state for both sides.
			static Stencil CreateBothSides(const StencilFaceState& state) {
				return { .bEnabled = true, .front = state, .back = state };
			}


		} stencil;

	};

	struct CullState {

		// Defines if cull is enabled.
		// The initial value is false.
		bool bEnabled = false;
		
		Face face = Face::Back;
		
		WindingOrder windingOrder = WindingOrder::CounterClockwise;

		constexpr bool operator==(const CullState& other) const noexcept {
			return face == other.face && windingOrder != other.windingOrder;
		}
		constexpr bool operator!=(const CullState& other) const noexcept {
			return !(*this == other);
		}

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

	enum class BlendFactor : bitfield {

		Zero,
		One,

		SrcColor,
		OneMinusSrcColor,
		SrcAlpha,
		OneMinusSrcAlpha,

		DstColor,
		OneMinusDstColor,
		DstAlpha,
		OneMinusDstAlpha,

		ConstantColor,
		OneMinusConstantColor,
		ConstantAlpha,
		OneMinusConstantAlpha,

		SrcAlphaSaturate,

		Src1Color,
		OneMinusSrc1Color,
		Src1Alpha,
		OneMinusSrc1Alpha,

		Default,

	};

	enum class BlendOp : bitfield {
		
		Add,
		Substract,
		ReverseSubstract,
		Min,
		Max,

		Default

	};

	struct BlendState {

		// Defines if blending is enabled.
		// The initial value is false.
		bool bEnabled = false;

		// Specified how the alpha source blending factor is computed.
		// The initial value is BlendFactor::One.
		BlendFactor srcAlphaFactor = BlendFactor::One;

		// Specified how the alpha destination blending factor is computed.
		// The initial value is BlendFactor::Zero.
		BlendFactor dstAlphaFactor = BlendFactor::Zero;

		// specifies the alpha blend equation, how the alpha component of the source and destination colors are combined.
		// The initial value is BlendOp::Add.
		BlendOp alphaOp = BlendOp::Add;

		// Specifies how the red, green, and blue blending factors are computed.
		// The initial value is BlendFactor::One.
		BlendFactor srcColorFactor = BlendFactor::One;

		// Specifies how the red, green, and blue destination blending factors are computed.
		// The initial value is BlendFactor::Zero.
		BlendFactor dstColorFactor = BlendFactor::Zero;

		// Specifies the RGB blend equation, how the red, green, and blue components of the source and destination colors are combined.
		// The initial value is BlendOp::Add.
		BlendOp colorOp = BlendOp::Add;

	};

}