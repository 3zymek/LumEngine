#pragma once
#include "rhi/rhi_common.hpp"
namespace lum::rhi {

	// Specifies the comparison function used in LumEngine RHI.
	enum class CompareFlag : bitfield {
		Equal,			// Passes if the incoming depth value is equal to the stored depth value.
		NotEqual,		// Passes if the incoming depth value is not equal to the stored depth value.
		LessEqual,		// Passes if the incoming depth value is less than or equal to the stored depth value.
		GreaterEqual,	// Passes if the incoming depth value is greater than or equal to the stored depth value.
		Less,			// Passes if the incoming depth value is less than the stored depth value.
		Greater,		// Passes if the incoming depth value is greater than the stored depth value.
		Always,			// Always passes.
		Never			// Never passes.
	};

	// Represents the rasterization state of the pipeline.
	// Includes polygon mode, face culling topology, and depth bias settings.
	struct RasterizerState {

		TopologyMode	topologyMode = TopologyMode::Fill;
		Face			topologyModeFaces = Face::FrontBack;

		struct DepthBiasState {

			// Specifies whether the depth bias is enabled.
			// If flag is false, depth bias is disabled. Otherwise, it is enabled. 
			// The initial value is false.
			bool bEnable = false;

			// Specifies a scale factor that is used to create a variable depth offset for each polygon. The initial value is 0.
			float32 slopeFactor = 0.f;

			// Is multiplied by an implementation-specific value to create a constant depth offset. The initial value is 0. 
			float32 units = 0.f;

			// Specifies the maximum (or minimum if negative) depth offset value.
			// If clamp is positive, the calculated offset is clamped to at most this value.
			// If clamp is negative, the calculated offset is clamped to at least this value.
			// If clamp is zero, no clamping is applied. The initial value is 0.
			float32 clamp = 0.f;

		} depthBias;

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

	// Represents the stencil state for a single face (front or back) in the pipeline.
	// Includes reference value, read/write masks, comparison function, and stencil operations.
	struct StencilFaceState {

		// Specifies the reference value for the stencil test. 
		// ref is clamped to the range [0,2n−1], where n is the number of bitplanes in the stencil buffer. 
		// The initial value is 0. 
		int32 reference = 0;

		// Specifies a mask that is ANDed with both the reference value
		// and the stored stencil value when the test is done.
		// The initial value is all 1's ( 0xFFFFFFFF ).
		uint32 readMask = 0xFFFFFFFF;

		// Specifies a bit mask to enable and disable writing of individual bits in the stencil planes.
		// Initially, the mask is all 1's ( 0xFFFFFFFF ).
		uint32 writeMask = 0xFFFFFFFF;

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

	};

	// Represents the depth and stencil state of the pipeline.
	// Includes depth test enable/write, comparison function, and stencil face operations.
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

	// Represents the face culling state of the pipeline.
	// Includes cull enable, which face to cull, and front face winding order.
	struct CullState {

		// Defines if cull is enabled.
		// The initial value is false.
		bool bEnabled = false;
		
		// Specifies whether front- or back-facing facets are candidates for culling. 
		// The initial value is Face::Back. 
		Face face = Face::Back;
		
		// Specifies the orientation of front-facing polygons. 
		// The initial value is WindingOrder::CounterClockwise. 
		WindingOrder windingOrder = WindingOrder::CounterClockwise;

	};

	// Represents the scissor test state of the pipeline.
	// Includes enable flag and scissor rectangle dimensions.
	struct ScissorState {

		// Defines if scissor test is enabled.
		// The initial value is false.
		bool bEnabled = false;
		
		// Specify the lower left corner of the scissor box on x axis.
		// Initially 0. 
		int32 x = 0;
		
		// Specify the lower left corner of the scissor box on y axis.
		// Initially 0. 
		int32 y = 0;

		// Specify the width of the scissor box.
		int32 width = 0;
		
		// Specify the height of the scissor box.
		int32 height = 0;

	};

	// Specifies the blending factor used for source or destination color in blending operations.
	enum class BlendFactor : bitfield {

		Zero,					// Multiply by 0, effectively ignoring this component.
		One,					// Multiply by 1, use full value of this component.

		SrcColor,				// Multiply by source color.
		OneMinusSrcColor,		// Multiply by ( 1 - source color ).
		SrcAlpha,				// Multiply by source alpha.
		OneMinusSrcAlpha,		// Multiply by ( 1 - source alpha ).

		DstColor,				// Multiply by destination color.
		OneMinusDstColor,		// Multiply by ( 1 - destination color ).
		DstAlpha,				// Multiply by destination alpha.
		OneMinusDstAlpha,		// Multiply by ( 1 - destination alpha ).

		ConstantColor,			// Multiply by a constant color set via glBlendColor.
		OneMinusConstantColor,	// Multiply by ( 1 - constant color ).
		ConstantAlpha,			// Multiply by constant alpha value.
		OneMinusConstantAlpha,	// Multiply by ( 1 - constant alpha value ).

		SrcAlphaSaturate,		// Multiply by min( source alpha, 1 - destination alpha ), typically for antialiasing.

		Src1Color,				// Multiply by secondary source color ( multi-source blending ).
		OneMinusSrc1Color,		// Multiply by ( 1 - secondary source color ).
		Src1Alpha,				// Multiply by secondary source alpha.
		OneMinusSrc1Alpha,		// Multiply by ( 1 - secondary source alpha ).

		Default,				// Default placeholder; uses value that's currently enabled ( no changes ).

	};

	// Specifies the operation used to combine source and destination colors in blending.
	enum class BlendOp : bitfield {

		// Add the weighted source and destination colors.
		//( C_out = C_src * F_src + C_dst * F_dst )
		Add,
		// Subtract the weighted destination from the source. 
		// ( C_out = C_src * F_src - C_dst * F_dst )
		Substract,
		// Subtract the weighted source from the destination. 
		// ( C_out = C_dst * F_dst - C_src * F_src )
		ReverseSubstract,
		// Take the component-wise minimum of the weighted source and destination colors.
		Min,
		// Take the component-wise maximum of the weighted source and destination colors.
		Max,

		Default // Default placeholder; uses value that's currently enabled ( no changes ).
	};

	// Represents the blending state of the pipeline.
	// Includes enable flag, source/destination factors, and blend operations for color and alpha.
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
	
	struct ViewportState {

		int32 x{};
		int32 y{};
		int32 width{};
		int32 height{};

	};

	struct ClearState {

	};

}