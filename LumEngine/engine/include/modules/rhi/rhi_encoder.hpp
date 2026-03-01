//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// TODO USELESS, REMOVE LATER
//
//=============================================================================//
#pragma once

#include "rhi/rhi_common.hpp"
#include "rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi {

	namespace detail {

		using InvokeFn = void(*)(vptr userParams, vptr device);
		using Storage = std::aligned_storage_t<256, alignof(std::max_align_t)>;
		struct FCommand {

			InvokeFn mInvokeFn;
			Storage mData;

		};

	}

	class REncoder {
	public:

		REncoder( ) = default;

		void Initialize( RDevice* device, usize maxCommands );

		///////////////////////////////////////////////////
		/// Viewport
		///////////////////////////////////////////////////

		/* @brief Sets the full viewport rectangle.
		* @param x      Left coordinate in pixels.
		* @param y      Bottom coordinate in pixels.
		* @param width  Width in pixels.
		* @param height Height in pixels.
		*/
		void SetViewport(int32 x, int32 y, int32 width, int32 height);

		/* @brief Sets only the X coordinate of the viewport. */
		void SetViewportX(int32 x);

		/* @brief Sets only the Y coordinate of the viewport. */
		void SetViewportY(int32 y);

		/* @brief Sets only the width of the viewport. */
		void SetViewportWidth(int32 width);

		/* @brief Sets only the height of the viewport. */
		void SetViewportHeight(int32 height);


		///////////////////////////////////////////////////
		/// Scissor
		///////////////////////////////////////////////////

		/* @brief Enables or disables the scissor test.
		* When enabled, fragments outside the scissor rectangle are discarded.
		* Commonly used for UI clipping, split-screen, and render region masking.
		* @param toggle True to enable, false to disable.
		*/
		void ToggleScissors(bool toggle);

		/* @brief Sets the full scissor rectangle.
		* @param x      Left coordinate in pixels.
		* @param y      Bottom coordinate in pixels.
		* @param width  Width in pixels.
		* @param height Height in pixels.
		*/
		void SetScissors(int32 x, int32 y, int32 width, int32 height);

		/* @brief Sets only the X coordinate of the scissor rectangle. */
		void SetScissorX(int32 x);

		/* @brief Sets only the Y coordinate of the scissor rectangle. */
		void SetScissorY(int32 y);

		/* @brief Sets only the width of the scissor rectangle. */
		void SetScissorWidth(int32 width);

		/* @brief Sets only the height of the scissor rectangle. */
		void SetScissorHeight(int32 height);


		///////////////////////////////////////////////////
		/// Culling
		///////////////////////////////////////////////////

		/* @brief Enables or disables face culling.
		* When enabled, back-facing primitives are discarded before rasterization.
		* Reduces overdraw on opaque geometry.
		* @param toggle True to enable, false to disable.
		*/
		void ToggleCull(bool toggle);

		/* @brief Selects which polygon faces are culled.
		* @param face Cull face selection (Front, Back, or FrontBack).
		*/
		void SetCullFace(EFace face);

		/* @brief Sets the front-face winding order.
		* @param order Winding order (Clockwise or CounterClockwise).
		*/
		void SetFrontFace(EWindingOrder order);


		///////////////////////////////////////////////////
		/// Blending
		///////////////////////////////////////////////////

		/* @brief Enables or disables color blending.
		* When enabled, fragment output is blended with the current render target.
		* @param toggle True to enable, false to disable.
		*/
		void ToggleBlend(bool toggle);

		/* @brief Sets the constant RGBA color used in constant-factor blend modes. */
		void SetBlendConstantColor(ChannelRGBA rgba);

		/* @brief Sets blend factors for both color and alpha channels.
		* @param srcColor Source factor for RGB.
		* @param dstColor Destination factor for RGB.
		* @param srcAlpha Source factor for alpha.
		* @param dstAlpha Destination factor for alpha.
		*/
		void SetBlendFactors(EBlendFactor srcColor, EBlendFactor dstColor, EBlendFactor srcAlpha, EBlendFactor dstAlpha);

		/* @brief Sets blend factors for RGB channels only. Alpha factors remain unchanged.
		* @param srcColor Source factor for RGB.
		* @param dstColor Destination factor for RGB.
		*/
		void SetBlendColorFactors(EBlendFactor srcColor, EBlendFactor dstColor);

		/* @brief Sets blend factors for alpha channel only. RGB factors remain unchanged.
		* @param srcAlpha Source factor for alpha.
		* @param dstAlpha Destination factor for alpha.
		*/
		void SetBlendAlphaFactors(EBlendFactor srcAlpha, EBlendFactor dstAlpha);

		/* @brief Sets the source blend factor for RGB channels. */
		void SetBlendSrcColorFactor(EBlendFactor factor);

		/* @brief Sets the destination blend factor for RGB channels. */
		void SetBlendDstColorFactor(EBlendFactor factor);

		/* @brief Sets the source blend factor for alpha channel. */
		void SetBlendSrcAlphaFactor(EBlendFactor factor);

		/* @brief Sets the destination blend factor for alpha channel. */
		void SetBlendDstAlphaFactor(EBlendFactor factor);

		/* @brief Sets the blend operation for both color and alpha channels.
		* @param colorOp Blend operation for RGB (e.g. Add, Subtract, Min, Max).
		* @param alphaOp Blend operation for alpha.
		*/
		void SetBlendOp(EBlendOp colorOp, EBlendOp alphaOp);

		/* @brief Sets the blend operation for RGB channels only. */
		void SetBlendColorOp(EBlendOp op);

		/* @brief Sets the blend operation for alpha channel only. */
		void SetBlendAlphaOp(EBlendOp op);

		/* @brief Sets blend factors for a specific render target index. */
		void SetBlendFactorsForTarget(uint8 target);

		/* @brief Enables or disables blending for a specific render target index. */
		void ToggleBlendForTarget(uint8 target, bool enable);


		///////////////////////////////////////////////////
		/// Depth
		///////////////////////////////////////////////////

		/* @brief Enables or disables depth buffer writes.
		* Depth testing may still occur when writes are disabled.
		* @param toggle True to enable, false to disable.
		*/
		void ToggleDepthWrite(bool toggle);

		/* @brief Enables or disables depth testing.
		* Failing fragments are discarded before blending.
		* @param enable True to enable, false to disable.
		*/
		void ToggleDepthTest(bool enable);

		/* @brief Sets the depth comparison function.
		* @param func Comparison function (e.g. Less, LessOrEqual, Always).
		*/
		void SetDepthFunc(RCompareFlag func);


		///////////////////////////////////////////////////
		/// Stencil
		///////////////////////////////////////////////////

		/* @brief Enables or disables stencil testing.
		* Used for masking, outlining, shadow volumes, and portal rendering.
		* @param toggle True to enable, false to disable.
		*/
		void ToggleStencilTest(bool toggle);

		/* @brief Sets the reference value used in stencil comparisons.
		* @param ref  Reference value (typically 0-255).
		* @param face Polygon faces this applies to (Front, Back, or FrontBack).
		*/
		void SetStencilReference(int32 ref, EFace face = EFace::FrontBack);

		/* @brief Sets stencil buffer operations for all test outcomes.
		* @param stencilFailOp Operation when stencil test fails.
		* @param depthFailOp   Operation when stencil passes but depth fails.
		* @param passOp        Operation when both tests pass.
		* @param face          Polygon faces this applies to.
		*/
		void SetStencilOp(EStencilOp stencilFailOp, EStencilOp depthFailOp, EStencilOp passOp, EFace face = EFace::FrontBack);

		/* @brief Sets the stencil operation applied when the stencil test fails.
		* @param op   Operation to apply.
		* @param face Polygon faces this applies to.
		*/
		void SetStencilOpOnStencilFail(EStencilOp op, EFace face = EFace::FrontBack);

		/* @brief Sets the stencil operation applied when the depth test fails.
		* @param op   Operation to apply.
		* @param face Polygon faces this applies to.
		*/
		void SetStencilOpOnDepthFail(EStencilOp op, EFace face = EFace::FrontBack);

		/* @brief Sets the stencil operation applied when both tests pass.
		* @param op   Operation to apply.
		* @param face Polygon faces this applies to.
		*/
		void SetStencilOpOnDepthPass(EStencilOp op, EFace face = EFace::FrontBack);


		///////////////////////////////////////////////////
		/// Rasterizer
		///////////////////////////////////////////////////

		/* @brief Enables or disables depth bias (polygon offset).
		* Used to prevent z-fighting in shadow mapping and decals.
		* @param toggle True to enable, false to disable.
		*/
		void ToggleDepthBias(bool toggle);

		/* @brief Sets depth bias scale factors. Offset = slope * dZ + constant.
		* @param slope    Scale factor applied to the polygon's depth slope.
		* @param constant Constant offset scaled by an implementation-specific value.
		*/
		void SetDepthBiasFactors(float32 slope, float32 constant);

		/* @brief Sets the maximum depth bias clamp.
		* Positive clamps from above, negative from below. Zero = no clamp.
		* @param clamp Maximum depth offset limit.
		*/
		void SetDepthBiasClamp(float32 clamp);

		/* @brief Sets only the slope scale factor for depth bias. */
		void SetDepthBiasSlope(float32 slopeFactor);

		/* @brief Sets only the constant factor for depth bias. */
		void SetDepthBiasConstant(float32 constantBias);

		/* @brief Sets the polygon rasterization mode.
		* @param mode Rasterization mode (Point, Line, or Fill).
		* @param face Polygon faces this applies to (default FrontBack).
		*/
		void SetTopology(ETopologyMode mode, EFace face = EFace::FrontBack);

		/* @brief Sets the size of rendered points in pixels. */
		void SetPointSize(float32 size);

		/* @brief Sets the width of rendered lines in pixels. */
		void SetLineWidth(float32 width);

		/* @brief Enables or disables multisampling. */
		void ToggleMultisample(bool toggle);

		/* @brief Enables or disables sample coverage. */
		void ToggleSampleCoverage(bool toggle);

		/* @brief Enables or disables alpha-to-coverage sampling. */
		void ToggleSampleAlphaToCoverage(bool toggle);

		/* @brief Enables or disables alpha-to-one sampling. */
		void ToggleSampleAlphaToOne(bool toggle);

		/* @brief Sets the sample coverage value and inversion flag. */
		void SetSampleCoverage(float32 value, bool invert);


		///////////////////////////////////////////////////
		/// Color & Clear
		///////////////////////////////////////////////////

		/* @brief Sets which RGBA channels are written to the framebuffer.
		* @param r, g, b, a Per-channel write enable flags.
		*/
		void SetColorMask(bool r, bool g, bool b, bool a);

		/* @brief Sets which RGBA channels are written to the framebuffer.
		* @param rgba ColorMask structure containing all channel flags.
		*/
		void SetColorMask(FColorMask rgba);

		/* @brief Sets the RGBA color used for subsequent ClearColor() calls. */
		void SetClearColor(ChannelRGBA color);

		/* @brief Clears the color buffer using the current clear color. */
		void ClearColor();

		/* @brief Sets the clear color and immediately clears the color buffer. */
		void ClearColor(ChannelRGBA color);

		/* @brief Clears the depth buffer to its default value (1.0). */
		void ClearDepth();

		/* @brief Clears the stencil buffer to zero. */
		void ClearStencil();

		/* @brief Clears the specified buffers in a single operation.
		* @param flags Combination of EClearFlag values (Color, Depth, Stencil).
		*/
		void Clear(Flags<EClearFlag> flags);


		///////////////////////////////////////////////////
		/// Draw Calls
		///////////////////////////////////////////////////

		/* @brief Draws non-indexed geometry.
		* @param vao         Vertex layout to draw.
		* @param vertexCount Number of vertices.
		*/
		void Draw(const RVertexLayoutHandle& vao, uint32 vertexCount);

		/* @brief Draws non-indexed geometry with instancing.
		* @param vao           Vertex layout to draw.
		* @param vertexCount   Number of vertices per instance.
		* @param instanceCount Number of instances.
		*/
		void DrawInstanced(const RVertexLayoutHandle& vao, uint32 vertexCount, uint32 instanceCount);

		/* @brief Draws non-indexed geometry with instancing and a base instance offset.
		* @param vao           Vertex layout to draw.
		* @param vertexCount   Number of vertices per instance.
		* @param instanceCount Number of instances.
		* @param baseInstance  Starting instance index.
		*/
		void DrawInstancedBase(const RVertexLayoutHandle& vao, uint32 vertexCount, uint32 instanceCount, uint32 baseInstance);

		/* @brief Draws indexed geometry.
		* @param vao          Vertex layout with attached EBO.
		* @param indicesCount Number of indices.
		*/
		void DrawElements(const RVertexLayoutHandle& vao, uint32 indicesCount);

		/* @brief Draws indexed geometry with instancing.
		* @param vao           Vertex layout with attached EBO.
		* @param indicesCount  Number of indices per instance.
		* @param instanceCount Number of instances.
		*/
		void DrawElementsInstanced(const RVertexLayoutHandle& vao, uint32 indicesCount, uint32 instanceCount);

		/* @brief Draws indexed geometry with instancing and a base instance offset.
		* @param vao           Vertex layout with attached EBO.
		* @param indicesCount  Number of indices per instance.
		* @param instanceCount Number of instances.
		* @param baseInstance  Starting instance index.
		*/
		void DrawElementsInstancedBase(const RVertexLayoutHandle& vao, uint32 indicesCount, uint32 instanceCount, uint32 baseInstance);

		void BeginPass();

		void EndPass();

	private:

		RDevice* mRenderDevice = nullptr;
		std::vector<detail::FCommand> mCommandBuffer;
		usize mMaxCommands = 0;

		void init( );

		template<typename tLambda>
		void setup_command( tLambda&& lambda ) {

			LUM_ASSERT(mCommandBuffer.size() < mMaxCommands, "Max commands per flush reached");

			detail::FCommand command;

			LUM_SASSERT(sizeof(detail::Storage) >= sizeof(tLambda) && "Lambda size's too big");
			LUM_SASSERT(alignof(detail::Storage) >= alignof(tLambda) && "Lambda alignment's too big");
			
			new (&command.mData) tLambda(std::forward<tLambda>(lambda));

			command.mInvokeFn = [](vptr lambdaFunc, vptr device) {
				auto* l = reinterpret_cast<tLambda*>(lambdaFunc);
				(*l)(reinterpret_cast<RDevice*>(device));
				};

			mCommandBuffer.push_back(std::move(command));
			
		}

	};

}