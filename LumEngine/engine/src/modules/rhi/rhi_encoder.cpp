//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// TODO USELESS, REMOVE LATER
//
//=============================================================================//
#include "rhi/rhi_encoder.hpp"

namespace lum::rhi {

	void REncoder::Initialize(RDevice* device, usize maxCommands) {
		LUM_ASSERT(device, "Nullptr given in initialization");
		mRenderDevice = device;
		mMaxCommands = maxCommands;
		init();
	}

	void REncoder::SetViewport(int32 x, int32 y, int32 width, int32 height) {
		setup_command([x, y, width, height](RDevice* device) {
			device->SetViewport(x, y, width, height);
			});
	}
	void REncoder::SetViewportX(int32 x) {
		setup_command([x](RDevice* device) {
			device->SetViewportX(x);
			});
	}
	void REncoder::SetViewportY(int32 y) {
		setup_command([y](RDevice* device) {
			device->SetViewportY(y);
			});
	}
	void REncoder::SetViewportWidth(int32 width) {
		setup_command([width](RDevice* device) {
			device->SetViewportWidth(width);
			});
	}
	void REncoder::SetViewportHeight(int32 height) {
		setup_command([height](RDevice* device) {
			device->SetViewportHeight(height);
			});
	}

	void REncoder::ToggleScissors(bool toggle) {
		setup_command([toggle](RDevice* device) {
			device->ToggleScissors(toggle);
			});
	}
	void REncoder::SetScissors(int32 x, int32 y, int32 width, int32 height) {
		setup_command([x, y, width, height](RDevice* device) {
			device->SetScissors(x, y, width, height);
			});
	}
	void REncoder::SetScissorX(int32 x) {
		setup_command([x](RDevice* device) {
			device->SetScissorX(x);
			});
	}
	void REncoder::SetScissorY(int32 y) {
		setup_command([y](RDevice* device) {
			device->SetScissorY(y);
			});
	}
	void REncoder::SetScissorWidth(int32 width) {
		setup_command([width](RDevice* device) {
			device->SetScissorWidth(width);
			});
	}
	void REncoder::SetScissorHeight(int32 height) {
		setup_command([height](RDevice* device) {
			device->SetScissorHeight(height);
			});
	}

	void REncoder::ToggleCull(bool toggle) {
		setup_command([toggle](RDevice* device) {
			device->ToggleCull(toggle);
			});
	}
	void REncoder::SetCullFace(Face face) {
		setup_command([face](RDevice* device) {
			device->SetCullFace(face);
			});
	}
	void REncoder::SetFrontFace(WindingOrder order) {
		setup_command([order](RDevice* device) {
			device->SetFrontFace(order);
			});
	}

	void REncoder::ToggleBlend(bool toggle) {
		setup_command([toggle](RDevice* device) {
			device->ToggleBlend(toggle);
			});
	}
	void REncoder::SetBlendConstantColor(ChannelRGBA rgba) {
		setup_command([rgba](RDevice* device) {
			device->SetBlendConstantColor(rgba);
			});
	}
	void REncoder::SetBlendFactors(BlendFactor srcColor, BlendFactor dstColor, BlendFactor srcAlpha, BlendFactor dstAlpha) {
		setup_command([srcColor, dstColor, srcAlpha, dstAlpha](RDevice* device) {
			device->SetBlendFactors(srcColor, dstColor, srcAlpha, dstAlpha);
			});
	}
	void REncoder::SetBlendColorFactors(BlendFactor srcColor, BlendFactor dstColor) {
		setup_command([srcColor, dstColor](RDevice* device) {
			device->SetBlendColorFactors(srcColor, dstColor);
			});
	}
	void REncoder::SetBlendAlphaFactors(BlendFactor srcAlpha, BlendFactor dstAlpha) {
		setup_command([srcAlpha, dstAlpha](RDevice* device) {
			device->SetBlendAlphaFactors(srcAlpha, dstAlpha);
			});
	}
	void REncoder::SetBlendSrcColorFactor(BlendFactor factor) {
		setup_command([factor](RDevice* device) {
			device->SetBlendSrcColorFactor(factor);
			});
	}
	void REncoder::SetBlendDstColorFactor(BlendFactor factor) {
		setup_command([factor](RDevice* device) {
			device->SetBlendDstColorFactor(factor);
			});
	}
	void REncoder::SetBlendSrcAlphaFactor(BlendFactor factor) {
		setup_command([factor](RDevice* device) {
			device->SetBlendSrcAlphaFactor(factor);
			});
	}
	void REncoder::SetBlendDstAlphaFactor(BlendFactor factor) {
		setup_command([factor](RDevice* device) {
			device->SetBlendDstAlphaFactor(factor);
			});
	}
	void REncoder::SetBlendOp(BlendOp colorOp, BlendOp alphaOp) {
		setup_command([colorOp, alphaOp](RDevice* device) {
			device->SetBlendOp(colorOp, alphaOp);
			});
	}
	void REncoder::SetBlendColorOp(BlendOp op) {
		setup_command([op](RDevice* device) {
			device->SetBlendColorOp(op);
			});
	}
	void REncoder::SetBlendAlphaOp(BlendOp op) {
		setup_command([op](RDevice* device) {
			device->SetBlendAlphaOp(op);
			});
	}
	void REncoder::SetBlendFactorsForTarget(uint8 target) {
		setup_command([target](RDevice* device) {
			device->SetBlendFactorsForTarget(target);
			});
	}
	void REncoder::ToggleBlendForTarget(uint8 target, bool enable) {
		setup_command([target, enable](RDevice* device) {
			device->ToggleBlendForTarget(target, enable);
			});
	}

	void REncoder::ToggleDepthWrite(bool toggle) {
		setup_command([toggle](RDevice* device) {
			device->ToggleDepthWrite(toggle);
			});
	}
	void REncoder::ToggleDepthTest(bool enable) {
		setup_command([enable](RDevice* device) {
			device->ToggleDepthTest(enable);
			});
	}
	void REncoder::SetDepthFunc(CompareFlag func) {
		setup_command([func](RDevice* device) {
			device->SetDepthFunc(func);
			});
	}

	void REncoder::ToggleStencilTest(bool toggle) {
		setup_command([toggle](RDevice* device) {
			device->ToggleStencilTest(toggle);
			});
	}
	void REncoder::SetStencilReference(int32 ref, Face face) {
		setup_command([ref, face](RDevice* device) {
			device->SetStencilReference(ref, face);
			});
	}
	void REncoder::SetStencilOp(StencilOp stencilFailOp, StencilOp depthFailOp, StencilOp passOp, Face face) {
		setup_command([stencilFailOp, depthFailOp, passOp, face](RDevice* device) {
			device->SetStencilOp(stencilFailOp, depthFailOp, passOp, face);
			});
	}
	void REncoder::SetStencilOpOnStencilFail(StencilOp op, Face face) {
		setup_command([op, face](RDevice* device) {
			device->SetStencilOpOnStencilFail(op, face);
			});
	}
	void REncoder::SetStencilOpOnDepthFail(StencilOp op, Face face) {
		setup_command([op, face](RDevice* device) {
			device->SetStencilOpOnDepthFail(op, face);
			});
	}
	void REncoder::SetStencilOpOnDepthPass(StencilOp op, Face face) {
		setup_command([op, face](RDevice* device) {
			device->SetStencilOpOnDepthPass(op, face);
			});
	}

	void REncoder::ToggleDepthBias(bool toggle) {
		setup_command([toggle](RDevice* device) {
			device->ToggleDepthBias(toggle);
			});
	}
	void REncoder::SetDepthBiasFactors(float32 slope, float32 constant) {
		setup_command([slope, constant](RDevice* device) {
			device->SetDepthBiasFactors(slope, constant);
			});
	}
	void REncoder::SetDepthBiasClamp(float32 clamp) {
		setup_command([clamp](RDevice* device) {
			device->SetDepthBiasClamp(clamp);
			});
	}
	void REncoder::SetDepthBiasSlope(float32 slopeFactor) {
		setup_command([slopeFactor](RDevice* device) {
			device->SetDepthBiasSlope(slopeFactor);
			});
	}
	void REncoder::SetDepthBiasConstant(float32 constantBias) {
		setup_command([constantBias](RDevice* device) {
			device->SetDepthBiasConstant(constantBias);
			});
	}
	void REncoder::SetTopology(TopologyMode mode, Face face) {
		setup_command([mode, face](RDevice* device) {
			device->SetTopology(mode, face);
			});
	}
	void REncoder::SetPointSize(float32 size) {
		setup_command([size](RDevice* device) {
			device->SetPointSize(size);
			});
	}
	void REncoder::SetLineWidth(float32 width) {
		setup_command([width](RDevice* device) {
			device->SetLineWidth(width);
			});
	}
	void REncoder::ToggleMultisample(bool toggle) {
		setup_command([toggle](RDevice* device) {
			device->ToggleMultisample(toggle);
			});
	}
	void REncoder::ToggleSampleCoverage(bool toggle) {
		setup_command([toggle](RDevice* device) {
			device->ToggleSampleCoverage(toggle);
			});
	}
	void REncoder::ToggleSampleAlphaToCoverage(bool toggle) {
		setup_command([toggle](RDevice* device) {
			device->ToggleSampleAlphaToCoverage(toggle);
			});
	}
	void REncoder::ToggleSampleAlphaToOne(bool toggle) {
		setup_command([toggle](RDevice* device) {
			device->ToggleSampleAlphaToOne(toggle);
			});
	}
	void REncoder::SetSampleCoverage(float32 value, bool invert) {
		setup_command([value, invert](RDevice* device) {
			device->SetSampleCoverage(value, invert);
			});
	}

	void REncoder::SetColorMask(bool r, bool g, bool b, bool a) {
		setup_command([r, g, b, a](RDevice* device) {
			device->SetColorMask(r, g, b, a);
			});
	}
	void REncoder::SetColorMask(FColorMask rgba) {
		setup_command([rgba](RDevice* device) {
			device->SetColorMask(rgba);
			});
	}
	void REncoder::SetClearColor(ChannelRGBA color) {
		setup_command([color](RDevice* device) {
			device->SetClearColor(color);
			});
	}
	void REncoder::ClearColor() {
		setup_command([](RDevice* device) {
			device->ClearColor();
			});
	}
	void REncoder::ClearColor(ChannelRGBA color) {
		setup_command([color](RDevice* device) {
			device->ClearColor(color);
			});
	}
	void REncoder::ClearDepth() {
		setup_command([](RDevice* device) {
			device->ClearDepth();
			});
	}
	void REncoder::ClearStencil() {
		setup_command([](RDevice* device) {
			device->ClearStencil();
			});
	}
	void REncoder::Clear(Flags<ClearFlag> flags) {
		setup_command([flags](RDevice* device) {
			device->Clear(flags);
			});
	}

	void REncoder::Draw(const RVertexLayoutHandle& vao, uint32 vertexCount) {
		setup_command([vao, vertexCount](RDevice* device) {
			device->Draw(vao, vertexCount);
			});
	}
	void REncoder::DrawInstanced(const RVertexLayoutHandle& vao, uint32 vertexCount, uint32 instanceCount) {
		setup_command([vao, vertexCount, instanceCount](RDevice* device) {
			device->DrawInstanced(vao, vertexCount, instanceCount);
			});
	}
	void REncoder::DrawInstancedBase(const RVertexLayoutHandle& vao, uint32 vertexCount, uint32 instanceCount, uint32 baseInstance) {
		setup_command([vao, vertexCount, instanceCount, baseInstance](RDevice* device) {
			device->DrawInstancedBase(vao, vertexCount, instanceCount, baseInstance);
			});
	}
	void REncoder::DrawElements(const RVertexLayoutHandle& vao, uint32 indicesCount) {
		setup_command([vao, indicesCount](RDevice* device) {
			device->DrawElements(vao, indicesCount);
			});
	}
	void REncoder::DrawElementsInstanced(const RVertexLayoutHandle& vao, uint32 indicesCount, uint32 instanceCount) {
		setup_command([vao, indicesCount, instanceCount](RDevice* device) {
			device->DrawElementsInstanced(vao, indicesCount, instanceCount);
			});
	}
	void REncoder::DrawElementsInstancedBase(const RVertexLayoutHandle& vao, uint32 indicesCount, uint32 instanceCount, uint32 baseInstance) {
		setup_command([vao, indicesCount, instanceCount, baseInstance](RDevice* device) {
			device->DrawElementsInstancedBase(vao, indicesCount, instanceCount, baseInstance);
			});
	}

	void REncoder::BeginPass() {
		mRenderDevice->NewFrame();
	}

	void REncoder::EndPass() {
		


		mRenderDevice->SwapBuffers();
	}

	void REncoder::init() {
		mCommandBuffer.reserve(mMaxCommands);
	}

} // namespace lum::rhi