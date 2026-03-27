#pragma once

#include "render/passes/environment_pass.hpp"
#include "render/common.hpp"
#include "rhi/core/rhi_device.hpp"
#include "render/renderer.hpp"
#include "render/shader_manager.hpp"
#include "render/mesh_manager.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void EnvironmentPass::Initialize( const FRendererContext& ctx ) {

		mContext = ctx;

		init( );

	}

	void EnvironmentPass::Execute( detail::GBuffer& gbuffer, const detail::FScreenQuad quad ) {

		mContext.mRenderDevice->BindFramebuffer( quad.mFbo );

		mContext.mRenderDevice->BindPipeline( mCubemap.mPipeline );

		mContext.mRenderDevice->BindShader( mCubemap.mShader );
		mContext.mRenderDevice->BindTexture( mCubemap.mTexture, LUM_TEX_CUBEMAP );
		mContext.mRenderDevice->BindSampler( mSampler, LUM_TEX_CUBEMAP );
		mContext.mRenderDevice->DrawElements( mCubemap.mVao, mCubemap.mNumIndices );

	}

	rhi::RTextureHandle EnvironmentPass::GetTexture( detail::IBLTexture tex ) {
		switch (tex) {
		case detail::IBLTexture::IrradianceMap: return mIrradianceMap;
		case detail::IBLTexture::PrefilteredMap: return mPrefilteredMap;
		default: return { };
		}
		return {};
	}



	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void EnvironmentPass::generate_irradiance_map( ) {

		rhi::RBufferHandle captureUBO;
		{ // Capture UBO (IBL)

			rhi::FBufferDescriptor desc;
			desc.mBufferType = rhi::BufferType::Uniform;
			desc.mBufferUsage = rhi::BufferUsage::Dynamic;
			desc.mMapFlags = rhi::MapFlag::Write;
			desc.mSize = sizeof( glm::mat4 ) * 2;
			captureUBO = mContext.mRenderDevice->CreateBuffer( desc );
			mContext.mRenderDevice->SetUniformBufferBinding( captureUBO, LUM_UBO_IRRADIANCE );

		}

		rhi::RFramebufferHandle captureFBO = mContext.mRenderDevice->CreateFramebuffer( { } );

		glm::mat4 captureProjection = glm::perspective( glm::radians( 90.0f ), 1.0f, 0.1f, 10.0f );
		glm::mat4 captureViews[ ] = {
			glm::lookAt( glm::vec3( 0 ), glm::vec3( 1, 0, 0 ), glm::vec3( 0,-1, 0 ) ),
			glm::lookAt( glm::vec3( 0 ), glm::vec3( -1, 0, 0 ), glm::vec3( 0,-1, 0 ) ),
			glm::lookAt( glm::vec3( 0 ), glm::vec3( 0, 1, 0 ), glm::vec3( 0, 0, 1 ) ),
			glm::lookAt( glm::vec3( 0 ), glm::vec3( 0,-1, 0 ), glm::vec3( 0, 0,-1 ) ),
			glm::lookAt( glm::vec3( 0 ), glm::vec3( 0, 0, 1 ), glm::vec3( 0,-1, 0 ) ),
			glm::lookAt( glm::vec3( 0 ), glm::vec3( 0, 0,-1 ), glm::vec3( 0,-1, 0 ) ),
		};

		rhi::FViewportState viewport = mContext.mRenderDevice->GetViewport( );

		mContext.mRenderDevice->SetViewport( 0, 0, 32, 32 );
		mContext.mRenderDevice->BindFramebuffer( captureFBO );
		mContext.mRenderDevice->BindTexture( mCubemap.mTexture, LUM_TEX_CUBEMAP );
		mContext.mRenderDevice->BindSampler( mSampler, LUM_TEX_CUBEMAP );
		mContext.mRenderDevice->BindPipeline( mCubemap.mPipeline );
		mContext.mRenderDevice->BindShader( mIrradianceShader );

		for (int32 i = 0; i < 6; i++) {

			glm::mat4 matrices[ ] = { captureProjection, captureViews[ i ] };
			mContext.mRenderDevice->UpdateBuffer( captureUBO, matrices );
			mContext.mRenderDevice->AttachCubemapFace( captureFBO, mIrradianceMap, i, 0 );
			mContext.mRenderDevice->DrawElements( mCubemap.mVao, mCubemap.mNumIndices );

		}

		mContext.mRenderDevice->DeleteFramebuffer( captureFBO );

		mContext.mRenderDevice->SetViewport( 0, 0, viewport.mWidth, viewport.mHeight );

	}

	void EnvironmentPass::generate_prefiltered_map( ) {

		rhi::RBufferHandle captureUBO;
		{ // Capture UBO (IBL)

			rhi::FBufferDescriptor desc;
			desc.mBufferType = rhi::BufferType::Uniform;
			desc.mBufferUsage = rhi::BufferUsage::Dynamic;
			desc.mMapFlags = rhi::MapFlag::Write;
			desc.mSize = sizeof( glm::mat4 ) * 2 + sizeof( float32 );
			captureUBO = mContext.mRenderDevice->CreateBuffer( desc );
			mContext.mRenderDevice->SetUniformBufferBinding( captureUBO, LUM_UBO_PREFILTERED_MAP );

		}

		rhi::RFramebufferHandle captureFBO = mContext.mRenderDevice->CreateFramebuffer( { } );

		glm::mat4 captureProjection = glm::perspective( glm::radians( 90.0f ), 1.0f, 0.1f, 10.0f );
		glm::mat4 captureViews[ ] = {
			glm::lookAt( glm::vec3( 0 ), glm::vec3( 1, 0, 0 ), glm::vec3( 0,-1, 0 ) ),
			glm::lookAt( glm::vec3( 0 ), glm::vec3( -1, 0, 0 ), glm::vec3( 0,-1, 0 ) ),
			glm::lookAt( glm::vec3( 0 ), glm::vec3( 0, 1, 0 ), glm::vec3( 0, 0, 1 ) ),
			glm::lookAt( glm::vec3( 0 ), glm::vec3( 0,-1, 0 ), glm::vec3( 0, 0,-1 ) ),
			glm::lookAt( glm::vec3( 0 ), glm::vec3( 0, 0, 1 ), glm::vec3( 0,-1, 0 ) ),
			glm::lookAt( glm::vec3( 0 ), glm::vec3( 0, 0,-1 ), glm::vec3( 0,-1, 0 ) ),
		};

		rhi::FViewportState viewport = mContext.mRenderDevice->GetViewport( );

		mContext.mRenderDevice->BindFramebuffer( captureFBO );
		mContext.mRenderDevice->BindTexture( mCubemap.mTexture, LUM_TEX_CUBEMAP );
		mContext.mRenderDevice->BindSampler( mSampler, LUM_TEX_CUBEMAP );
		mContext.mRenderDevice->BindPipeline( mCubemap.mPipeline );
		mContext.mRenderDevice->BindShader( mPrefilteredShader );

		struct UniformData {
			glm::mat4 mProjection;
			glm::mat4 mView;
			float32 mRoughness;
			float32 _pad[ 3 ];
		};

		for (uint32 mip = 0; mip < skPrefilteredMipmaps; mip++) {

			float32 roughness = ( float32 ) mip / ( float32 ) (skPrefilteredMipmaps - 1);

			uint32 mipSize = 128 >> mip;
			mContext.mRenderDevice->SetViewport( 0, 0, mipSize, mipSize );

			for (int32 i = 0; i < 6; i++) {

				UniformData data;
				data.mProjection = captureProjection;
				data.mView = captureViews[ i ];
				data.mRoughness = roughness;

				mContext.mRenderDevice->UpdateBuffer( captureUBO, &data );
				mContext.mRenderDevice->AttachCubemapFace( captureFBO, mPrefilteredMap, i, mip );
				mContext.mRenderDevice->DrawElements( mCubemap.mVao, mCubemap.mNumIndices );

			}

		}

		mContext.mRenderDevice->DeleteFramebuffer( captureFBO );

		mContext.mRenderDevice->SetViewport( 0, 0, viewport.mWidth, viewport.mHeight );
	}

	void EnvironmentPass::init( ) {

		float32 vertices[ ] = {
			-1, -1, -1,  1, -1, -1,  1,  1, -1, -1,  1, -1,
			-1, -1,  1,  1, -1,  1,  1,  1,  1, -1,  1,  1,
		};
		uint32 indices[ ] = {
			0,1,2, 2,3,0, // front
			5,4,7, 7,6,5, // back
			4,0,3, 3,7,4, // left
			1,5,6, 6,2,1, // right
			3,2,6, 6,7,3, // top
			4,5,1, 1,0,4  // bottom
		};
		mCubemap.mNumIndices = ArraySize( indices );

		if (!mContext.mRenderDevice->IsValid( mCubemap.mVbo )) { // Environment VBO

			rhi::FBufferDescriptor desc;
			desc.mBufferUsage = rhi::BufferUsage::Static;
			desc.mMapFlags = rhi::MapFlag::None;
			desc.mSize = ByteSize( vertices );
			desc.mData = vertices;
			desc.mBufferType = rhi::BufferType::Vertex;
			mCubemap.mVbo = mContext.mRenderDevice->CreateBuffer( desc );

		}
		if (!mContext.mRenderDevice->IsValid( mCubemap.mEbo )) { // Environment EBO

			rhi::FBufferDescriptor desc;
			desc.mBufferUsage = rhi::BufferUsage::Static;
			desc.mMapFlags = rhi::MapFlag::None;
			desc.mSize = ByteSize( indices );
			desc.mData = indices;
			desc.mBufferType = rhi::BufferType::Element;
			mCubemap.mEbo = mContext.mRenderDevice->CreateBuffer( desc );

		}
		if (!mContext.mRenderDevice->IsValid( mCubemap.mVao )) { // Environment VAO
			rhi::FVertexAttribute attrs[ ]{
				{
					.mFormat = rhi::DataFormat::Vec3,
					.mRelativeOffset = 0,
					.mShaderLocation = LUM_LAYOUT_POSITION
				}
			};
			rhi::FVertexLayoutDescriptor desc;
			desc.mStride = 3 * sizeof( float32 );
			desc.mAttributes = attrs;
			mCubemap.mVao = mContext.mRenderDevice->CreateVertexLayout( desc, mCubemap.mVbo );
			mContext.mRenderDevice->AttachElementBufferToLayout( mCubemap.mEbo, mCubemap.mVao );
		}
		if (!mContext.mRenderDevice->IsValid( mSampler )) { // Environment sampler

			rhi::FSamplerDescriptor desc;
			desc.mMinFilter = rhi::SamplerMinFilter::LinearMipmapLinear;
			desc.mMagFilter = rhi::SamplerMagFilter::Linear;
			desc.mWrapR = rhi::SamplerWrap::ClampEdge;
			desc.mWrapS = rhi::SamplerWrap::ClampEdge;
			desc.mWrapT = rhi::SamplerWrap::ClampEdge;
			mSampler = mContext.mRenderDevice->CreateSampler( desc );

		}
		if (!mContext.mRenderDevice->IsValid( mCubemap.mPipeline )) { // Environment Pipeline
			rhi::FPipelineDescriptor desc;
			desc.mDepthStencil.mDepth.bEnabled = true;
			desc.mDepthStencil.mDepth.bWriteToZBuffer = false;
			desc.mDepthStencil.mDepth.mCompare = rhi::CompareFlag::LessEqual;
			desc.mCull.bEnabled = false;
			desc.mCull.mFace = rhi::Face::Back;
			mCubemap.mPipeline = mContext.mRenderDevice->CreatePipeline( desc );
		}
		if (!mContext.mRenderDevice->IsValid( mIrradianceMap )) { // Irradiance map (IBL)

			rhi::FTextureDescriptor desc;
			desc.mTextureType = rhi::TextureType::Cubemap;
			desc.mImageLayout = rhi::ImageLayout::RGB16F;
			desc.mImageFormat = rhi::ImageFormat::RGB;
			desc.mDataType = rhi::TextureDataType::Float;
			desc.mWidth = 32;
			desc.mHeight = 32;
			mIrradianceMap = mContext.mRenderDevice->CreateTexture( desc );

		}
		if (!mContext.mRenderDevice->IsValid( mPrefilteredMap )) { // Prefiltered environment map (IBL)

			rhi::FTextureDescriptor desc;
			desc.mTextureType = rhi::TextureType::Cubemap;
			desc.mImageFormat = rhi::ImageFormat::RGB;
			desc.mImageLayout = rhi::ImageLayout::RGB16F;
			desc.mWidth = 128;
			desc.mHeight = 128;
			desc.mMipmapLevels = 5;
			mPrefilteredMap = mContext.mRenderDevice->CreateTexture( desc );

		}
		{ // Shaders

			mCubemap.mShader = mContext.mShaderMgr->LoadShader( "shaders/skybox_pass.vert", "shaders/skybox_pass.frag", RootID::Internal );
			mIrradianceShader = mContext.mShaderMgr->LoadShader( "shaders/irradiance.vert", "shaders/irradiance.frag", RootID::Internal );
			mPrefilteredShader = mContext.mShaderMgr->LoadShader( "shaders/prefiltered_env.vert", "shaders/prefiltered_env.frag", RootID::Internal );

		}

	}


} // namespace lum::render