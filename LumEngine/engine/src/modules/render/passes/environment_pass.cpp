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
		//mContext.mRenderDevice->BindTexture( mCubemap.mTexture, LUM_TEX_CUBEMAP );
		mContext.mRenderDevice->BindTexture( mIrradianceMap, LUM_TEX_CUBEMAP );
		mContext.mRenderDevice->BindSampler( mSampler, LUM_TEX_CUBEMAP );
		mContext.mRenderDevice->DrawElements( mCubemap.mVao, mCubemap.mNumIndices );

	}





	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void EnvironmentPass::generate_irradiance_map( ) {

		{
			rhi::FTextureDescriptor desc;
			desc.mTextureType = rhi::TextureType::Cubemap;
			desc.mImageLayout = rhi::ImageLayout::RGB16F;
			desc.mImageFormat = rhi::ImageFormat::RGB;
			desc.mDataType = rhi::TextureDataType::Float;
			desc.mWidth = 32;
			desc.mHeight = 32;
			mIrradianceMap = mContext.mRenderDevice->CreateTexture( desc );

		}

		{
			rhi::FBufferDescriptor desc;
			desc.mBufferType = rhi::BufferType::Uniform;
			desc.mBufferUsage = rhi::BufferUsage::Dynamic;
			desc.mMapFlags = rhi::MapFlag::Write;
			desc.mSize = sizeof( glm::mat4 ) * 2;
			mIrradianceUBO = mContext.mRenderDevice->CreateBuffer( desc );
			mContext.mRenderDevice->SetUniformBufferBinding( mIrradianceUBO, LUM_UBO_IRRADIANCE );
		}

		mIrradianceShader = mContext.mShaderMgr->LoadShader( "shaders/irradiance.vert", "shaders/irradiance.frag", RootID::Internal );
		
		rhi::RFramebufferHandle captureFBO;
		{

			captureFBO = mContext.mRenderDevice->CreateFramebuffer( {} );

		}

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
		mContext.mRenderDevice->BindShader( mIrradianceShader );
		mContext.mRenderDevice->BindTexture( mCubemap.mTexture, LUM_TEX_CUBEMAP );
		mContext.mRenderDevice->BindSampler( mSampler, LUM_TEX_CUBEMAP );
		mContext.mRenderDevice->BindPipeline( mCubemap.mPipeline );

		for (int32 i = 0; i < 6; i++) {

			glm::mat4 matrices[ ] = { captureProjection, captureViews[ i ] };
			mContext.mRenderDevice->UpdateBuffer( mIrradianceUBO, matrices );
			mContext.mRenderDevice->AttachCubemapFace( captureFBO, mIrradianceMap, i );
			mContext.mRenderDevice->DrawElements( mCubemap.mVao, mCubemap.mNumIndices );

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

		{ // Environment VBO
			rhi::FBufferDescriptor desc;
			desc.mBufferUsage = rhi::BufferUsage::Static;
			desc.mMapFlags = rhi::MapFlag::None;
			desc.mSize = ByteSize( vertices );
			desc.mData = vertices;
			desc.mBufferType = rhi::BufferType::Vertex;
			mCubemap.mVbo = mContext.mRenderDevice->CreateBuffer( desc );
		}
		{ // Environment EBO
			rhi::FBufferDescriptor desc;
			desc.mBufferUsage = rhi::BufferUsage::Static;
			desc.mMapFlags = rhi::MapFlag::None;
			desc.mSize = ByteSize( indices );
			desc.mData = indices;
			desc.mBufferType = rhi::BufferType::Element;
			mCubemap.mEbo = mContext.mRenderDevice->CreateBuffer( desc );
		}
		{ // Environment VAO
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
		{ // Environment sampler

			rhi::FSamplerDescriptor desc;
			desc.mMinFilter = rhi::SamplerMinFilter::LinearMipmapLinear;
			desc.mMagFilter = rhi::SamplerMagFilter::Linear;
			desc.mWrapR = rhi::SamplerWrap::ClampEdge;
			desc.mWrapS = rhi::SamplerWrap::ClampEdge;
			desc.mWrapT = rhi::SamplerWrap::ClampEdge;
			mSampler = mContext.mRenderDevice->CreateSampler( desc );

		}
		{ // Environment Pipeline
			rhi::FPipelineDescriptor desc;
			desc.mDepthStencil.mDepth.bEnabled = true;
			desc.mDepthStencil.mDepth.bWriteToZBuffer = false;
			desc.mDepthStencil.mDepth.mCompare = rhi::CompareFlag::LessEqual;
			desc.mCull.bEnabled = false;
			desc.mCull.mFace = rhi::Face::Back;
			mCubemap.mPipeline = mContext.mRenderDevice->CreatePipeline( desc );
		}

		mCubemap.mShader = mContext.mShaderMgr->LoadShader( "shaders/skybox_pass.vert", "shaders/skybox_pass.frag", RootID::Internal );

	}


} // namespace lum::render