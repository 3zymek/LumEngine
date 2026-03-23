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

	void EnvironmentPass::Execute( detail::GBuffer& gbuffer ) {

		mContext.mRenderDevice->BindFramebuffer( rhi::gDefaultFramebuffer );

		mContext.mRenderDevice->BindPipeline( mCubemap.mPipeline );

		mContext.mRenderDevice->BindShader( mCubemap.mShader );
		mContext.mRenderDevice->BindTexture( mCubemap.mTexture, LUM_TEX_CUBEMAP );
		mContext.mRenderDevice->BindSampler( mSampler, LUM_TEX_CUBEMAP );
		mContext.mRenderDevice->DrawElements( mCubemap.mVao, mCubemap.mNumIndices );

	}





	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

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