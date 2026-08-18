//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Environment render pass — renders the HDR skybox cubemap
//          and precomputes IBL (Image-Based Lighting) maps:
//          irradiance map and prefiltered environment map.
//
//=============================================================================//
#pragma once

#include "Render/Passes/EnvironmentPass.hpp"
#include "Render/RenderCommon.hpp"
#include "Rhi/Core/RhiDevice.hpp"
#include "Render/Renderer.hpp"
#include "Render/ShaderManager.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void EnvironmentPass::Initialize( const RendererContext& ctx ) {

		ctx.Validate( );

		mCtx = ctx;

		init( );

	}

	void EnvironmentPass::SetCubemapTexture( rhi::TextureHandle tex ) {
		mCubemap.mTexture = tex;
		generate_irradiance_map( );
		generate_prefiltered_map( );
	}

	void EnvironmentPass::Execute( detail::DeferredBuffer& gbuffer, const detail::ScreenQuad& quad ) {

		mCtx.mRenderDev->BindFramebuffer( quad.mSceneFbo ); // Render skybox to screenquad
		mCtx.mRenderDev->BindPipeline( mCubemap.mPipeline );
		mCtx.mRenderDev->BindShader( mCubemap.mShader );
		mCtx.mRenderDev->BindTexture( mCubemap.mTexture, LUM_TEX_CUBEMAP );
		mCtx.mRenderDev->BindSampler( mSampler, LUM_TEX_CUBEMAP );

		mCtx.mRenderDev->DrawElements( mCubemap.mVao, mCubemap.mNumIndices );

	}

	rhi::TextureHandle EnvironmentPass::GetTexture( detail::IBLTexture tex ) const noexcept {
		switch (tex) {
		case detail::IBLTexture::IrradianceMap: return mIBL.mIrradiance.mTexture;
		case detail::IBLTexture::PrefilteredMap: return mIBL.mPrefiltered.mTexture;
		default: return { };
		}
		return {};
	}



	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void EnvironmentPass::generate_irradiance_map( ) {

		rhi::BufferHandle captureUBO;

		// Capture UBO (IBL)s
		{

			rhi::BufferCreateInfo desc;
			desc.mBufferType = rhi::BufferType::Uniform;
			desc.mBufferUsage = rhi::BufferUsage::Dynamic;
			desc.mMapFlags = rhi::MapFlag::Write;
			desc.mSize = sizeof( Matrix4 ) * 2;
			captureUBO = mCtx.mRenderDev->CreateBuffer( desc );
			mCtx.mRenderDev->SetUniformBufferBinding( captureUBO, LUM_UBO_IRRADIANCE );

		}

		rhi::FramebufferHandle		captureFBO = mCtx.mRenderDev->CreateFramebuffer( { } );
		Matrix4						captureProjection = IBL::GetCaptureProjection( );
		std::array<Matrix4, 6>		captureViews = IBL::GetCaptureViews( );
		rhi::ViewportState			viewport = mCtx.mRenderDev->GetViewport( );

		mCtx.mRenderDev->SetViewport( 0, 0, 32, 32 );
		mCtx.mRenderDev->BindFramebuffer( captureFBO );
		mCtx.mRenderDev->BindTexture( mCubemap.mTexture, LUM_TEX_CUBEMAP );
		mCtx.mRenderDev->BindSampler( mSampler, LUM_TEX_CUBEMAP );
		mCtx.mRenderDev->BindPipeline( mCubemap.mPipeline );
		mCtx.mRenderDev->BindShader( mIBL.mIrradiance.mShader );

		for (int32 i = 0; i < 6; i++) {

			Matrix4 matrices[ ] = { captureProjection, captureViews[ i ] };
			mCtx.mRenderDev->UpdateBuffer( captureUBO, matrices );

			rhi::TextureLayerAttachment attach;
			attach.mAttachment = rhi::BufferBit::Color;
			attach.mMip = 1;
			attach.mLayer = i;
			attach.mSlot = 0;
			mCtx.mRenderDev->AttachTextureLayer( captureFBO, mIBL.mIrradiance.mTexture, attach );
			mCtx.mRenderDev->DrawElements( mCubemap.mVao, mCubemap.mNumIndices );

		}

		mCtx.mRenderDev->Delete( captureFBO );
		mCtx.mRenderDev->Delete( captureUBO );

		mCtx.mRenderDev->SetViewport( 0, 0, viewport.mWidth, viewport.mHeight );

	}

	void EnvironmentPass::generate_prefiltered_map( ) {

		rhi::BufferHandle captureUBO;

		// Capture UBO (IBL)
		{

			rhi::BufferCreateInfo desc;
			desc.mBufferType = rhi::BufferType::Uniform;
			desc.mBufferUsage = rhi::BufferUsage::Dynamic;
			desc.mMapFlags = rhi::MapFlag::Write;
			desc.mSize = sizeof( Matrix4 ) * 2 + sizeof( float32 );
			captureUBO = mCtx.mRenderDev->CreateBuffer( desc );
			mCtx.mRenderDev->SetUniformBufferBinding( captureUBO, LUM_UBO_PREFILTERED_MAP );

		}

		rhi::FramebufferHandle		captureFBO = mCtx.mRenderDev->CreateFramebuffer( { } );
		Matrix4						captureProjection = IBL::GetCaptureProjection( );
		std::array<Matrix4, 6>		captureViews = IBL::GetCaptureViews( );
		rhi::ViewportState			viewport = mCtx.mRenderDev->GetViewport( );

		mCtx.mRenderDev->BindFramebuffer( captureFBO );
		mCtx.mRenderDev->BindTexture( mCubemap.mTexture, LUM_TEX_CUBEMAP );
		mCtx.mRenderDev->BindSampler( mSampler, LUM_TEX_CUBEMAP );
		mCtx.mRenderDev->BindPipeline( mCubemap.mPipeline );
		mCtx.mRenderDev->BindShader( mIBL.mPrefiltered.mShader );

		struct LUM_UBO_ALIGNMENT UniformData {
			Matrix4 mProjection = Matrix4( 1.0f );
			Matrix4 mView = Matrix4( 1.0f );
			float32 mRoughness{};
			float32 _pad[ 3 ]{};
		};

		for (uint32 mip = 0; mip < mIBL.mPrefiltered.skMipmapLevels; mip++) {

			float32 roughness = ToFloat32( mip ) / ToFloat32( (mIBL.mPrefiltered.skMipmapLevels - 1) );

			uint32 mipSize = 128 >> mip;
			mCtx.mRenderDev->SetViewport( 0, 0, mipSize, mipSize );

			for (int32 i = 0; i < 6; i++) {

				UniformData data{};
				data.mProjection = captureProjection;
				data.mView = captureViews[ i ];
				data.mRoughness = roughness;

				mCtx.mRenderDev->UpdateBuffer( captureUBO, &data );

				rhi::TextureLayerAttachment attach;
				attach.mAttachment = rhi::BufferBit::Color;
				attach.mMip = mip;
				attach.mLayer = i;
				attach.mSlot = 0;
				mCtx.mRenderDev->AttachTextureLayer( captureFBO, mIBL.mPrefiltered.mTexture, attach );
				mCtx.mRenderDev->DrawElements( mCubemap.mVao, mCubemap.mNumIndices );

			}

		}

		mCtx.mRenderDev->Delete( captureFBO );
		mCtx.mRenderDev->Delete( captureUBO );

		mCtx.mRenderDev->SetViewport( 0, 0, viewport.mWidth, viewport.mHeight );

	}

	void EnvironmentPass::init( ) {

		float32 cubemapVertices[ ] = {

			-1, -1, -1,  1, -1, -1,  1,  1, -1, -1,  1, -1,
			-1, -1,  1,  1, -1,  1,  1,  1,  1, -1,  1,  1,

		};
		uint32 cubemapIndices[ ] = {

			0,1,2, 2,3,0, // front
			5,4,7, 7,6,5, // back
			4,0,3, 3,7,4, // left
			1,5,6, 6,2,1, // right
			3,2,6, 6,7,3, // top
			4,5,1, 1,0,4  // bottom

		};
		mCubemap.mNumIndices = ArraySize( cubemapIndices );


		// Cubemap VBO
		if (!mCtx.mRenderDev->IsValid( mCubemap.mVbo )) {

			rhi::BufferCreateInfo desc;
			desc.mBufferUsage = rhi::BufferUsage::Static;
			desc.mMapFlags = rhi::MapFlag::None;
			desc.mSize = ComputeByteSize( cubemapVertices );
			desc.mData = cubemapVertices;
			desc.mBufferType = rhi::BufferType::Vertex;
			mCubemap.mVbo = mCtx.mRenderDev->CreateBuffer( desc );

		}

		// Cubemap EBO
		if (!mCtx.mRenderDev->IsValid( mCubemap.mEbo )) {

			rhi::BufferCreateInfo desc;
			desc.mBufferUsage = rhi::BufferUsage::Static;
			desc.mMapFlags = rhi::MapFlag::None;
			desc.mSize = ComputeByteSize( cubemapIndices );
			desc.mData = cubemapIndices;
			desc.mBufferType = rhi::BufferType::Element;
			mCubemap.mEbo = mCtx.mRenderDev->CreateBuffer( desc );

		}

		// Cubemap VAO
		if (!mCtx.mRenderDev->IsValid( mCubemap.mVao )) {

			rhi::VertexAttribute attrs[ ]{
				{
					.mFormat = rhi::DataFormat::Vec3,
					.mRelativeOffset = 0,
					.mShaderLocation = LUM_LAYOUT_POSITION
				}
			};
			rhi::VertexLayoutCreateInfo desc;
			desc.mStride = 3 * sizeof( float32 );
			desc.mAttributes = attrs;
			mCubemap.mVao = mCtx.mRenderDev->CreateVertexLayout( desc, mCubemap.mVbo );
			mCtx.mRenderDev->AttachElementBufferToLayout( mCubemap.mEbo, mCubemap.mVao );

		}

		// Cubemap sampler
		if (!mCtx.mRenderDev->IsValid( mSampler )) {

			rhi::SamplerCreateInfo desc;
			desc.mMinFilter = rhi::SamplerMinFilter::LinearMipmapLinear;
			desc.mMagFilter = rhi::SamplerMagFilter::Linear;
			desc.mWrapR = rhi::SamplerWrap::ClampEdge;
			desc.mWrapS = rhi::SamplerWrap::ClampEdge;
			desc.mWrapT = rhi::SamplerWrap::ClampEdge;
			mSampler = mCtx.mRenderDev->CreateSampler( desc );

		}

		// Cubemap Pipeline
		if (!mCtx.mRenderDev->IsValid( mCubemap.mPipeline )) {

			rhi::PipelineCreateInfo desc;
			desc.mDepthStencil.mDepth.bEnabled = true;
			desc.mDepthStencil.mDepth.bWriteToZBuffer = false;
			desc.mDepthStencil.mDepth.mCompare = rhi::CompareFlag::LessEqual;
			desc.mCull.bEnabled = false;
			desc.mCull.mFace = rhi::Face::Back;
			mCubemap.mPipeline = mCtx.mRenderDev->CreatePipeline( desc );

		}

		// Irradiance map (IBL)
		if (!mCtx.mRenderDev->IsValid( mIBL.mIrradiance.mTexture )) {

			rhi::TextureCreateInfo desc;
			desc.mTextureType = rhi::TextureType::Cubemap;
			desc.mInternalFormat = rhi::TextureInternalFormat::RGB16F;
			desc.mPixelFormat = rhi::TexturePixelFormat::RGB;
			desc.mDataType = rhi::TextureDataType::Float;
			desc.mWidth = 32;
			desc.mHeight = 32;
			mIBL.mIrradiance.mTexture = mCtx.mRenderDev->CreateTexture( desc );

		}

		// Prefiltered environment map (IBL)
		if (!mCtx.mRenderDev->IsValid( mIBL.mPrefiltered.mTexture )) {

			rhi::TextureCreateInfo desc;
			desc.mTextureType = rhi::TextureType::Cubemap;
			desc.mPixelFormat = rhi::TexturePixelFormat::RGB;
			desc.mInternalFormat = rhi::TextureInternalFormat::RGB16F;
			desc.mWidth = 128;
			desc.mHeight = 128;
			desc.mMipmapLevels = 5;
			mIBL.mPrefiltered.mTexture = mCtx.mRenderDev->CreateTexture( desc );

		}
		{ // Shaders

			mCubemap.mShader = mCtx.mShaderMgr->LoadShader( "shaders/skybox_pass.vert", "shaders/skybox_pass.frag", ResourceRoot::Internal );

			mIBL.mIrradiance.mShader = mCtx.mShaderMgr->LoadShader( "shaders/irradiance.vert", "shaders/irradiance.frag", ResourceRoot::Internal );
			mIBL.mPrefiltered.mShader = mCtx.mShaderMgr->LoadShader( "shaders/prefiltered_env.vert", "shaders/prefiltered_env.frag", ResourceRoot::Internal );

		}

	}


} // namespace lum::render