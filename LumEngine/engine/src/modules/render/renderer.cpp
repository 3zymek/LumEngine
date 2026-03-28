//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Bridge between engine data and the RHI backend.
//
//=============================================================================//

#include "core/core_common.hpp"

#include "render/renderer.hpp"
#include "render/shader_manager.hpp"

#include "event/event_bus.hpp"
#include "event/events/window_events.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void Renderer::Initialize( const FRendererContext& ctx ) {

		mContext = ctx;

		mGeometryPass.Initialize( mContext );
		mLightPass.Initialize( mContext );
		mEnvironmentPass.Initialize( mContext );
		mShadowPass.Initialize( mContext );
		mGBuffer.Initialize( mContext, 500, 500 );

		init( );

	}

	void Renderer::UpdateCamera( const FRenderCamera& camera ) {

		mCameraUBOData.mPosition = glm::vec4( camera.mPosition, 0.0 );
		mCameraUBOData.mProjection = camera.mProjection;
		mCameraUBOData.mView = camera.mView;
		mCameraUBOData.mInvViewProj = glm::inverse( camera.mProjection * camera.mView );

		upload_camera_uniform( );

	}

	void Renderer::BeginFrame( ) {

		mLightPass.ClearLights( );
		mContext.mRenderDevice->BindFramebuffer( rhi::gDefaultFramebuffer );
		mContext.mRenderDevice->Clear(
			rhi::BufferBit::Color |
			rhi::BufferBit::Depth |
			rhi::BufferBit::Stencil
		);

	}

	void Renderer::EndFrame( ) {

		mContext.mRenderDevice->BindFramebuffer( mScreenQuad.mFbo );
		mContext.mRenderDevice->Clear(
			rhi::BufferBit::Color |
			rhi::BufferBit::Depth |
			rhi::BufferBit::Stencil
		);

		mShadowPass.Execute( mGeometryPass, mLightPass );

		mGeometryPass.Execute( mGBuffer );

		mLightPassExecutables.mIrradianceMap = mEnvironmentPass.GetTexture( detail::IBLTexture::IrradianceMap );
		mLightPassExecutables.mPrefilteredEnvMap = mEnvironmentPass.GetTexture( detail::IBLTexture::PrefilteredMap );
		mLightPassExecutables.mShadowMap = mShadowPass.GetShadowMap( );

		mLightPass.Execute( mGBuffer, mScreenQuad, mLightPassExecutables );

		mEnvironmentPass.Execute( mGBuffer, mScreenQuad );

		mContext.mRenderDevice->BindFramebuffer( rhi::gDefaultFramebuffer );
		mContext.mRenderDevice->BindShader( mPostprocessShader );
		mContext.mRenderDevice->BindTexture( mScreenQuad.mTexture, 0 );
		mContext.mRenderDevice->DrawElements( mScreenQuad.mVao, 6 );

		mContext.mRenderDevice->SwapBuffers( );

	}





	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void Renderer::init( ) {

		mContext.mEventBus->SubscribePermanently<EWindowResized>(
			[&]( const EWindowResized& e ) {
				mContext.mRenderDevice->SetViewport( 0, 0, e.mWidth, e.mHeight );
				create_screenquad_texture( e.mWidth, e.mHeight );
				create_screenquad_fbo( );
			}
		);

		// Camera Uniform
		if (!mContext.mRenderDevice->IsValid( mCameraUBO )) {
			rhi::FBufferDescriptor desc;
			desc.mBufferUsage = rhi::BufferUsage::Dynamic;
			desc.mMapFlags = rhi::MapFlag::Write;
			desc.mSize = sizeof( detail::FCameraUBOData );
			desc.mBufferType = rhi::BufferType::Uniform;
			mCameraUBO = mContext.mRenderDevice->CreateBuffer( desc );
			mContext.mRenderDevice->SetUniformBufferBinding( mCameraUBO, LUM_UBO_CAMERA_BINDING );
		}

		// Screen quad VBO
		if (!mContext.mRenderDevice->IsValid( mScreenQuad.mVbo )) {

			std::vector<FVertex> vertices = {
				{ {-1.f, -1.f, 0.f}, {}, {0.f, 0.f}, {}, {} },
				{ { 1.f, -1.f, 0.f}, {}, {1.f, 0.f}, {}, {} },
				{ { 1.f,  1.f, 0.f}, {}, {1.f, 1.f}, {}, {} },
				{ {-1.f,  1.f, 0.f}, {}, {0.f, 1.f}, {}, {} },
			};

			rhi::FBufferDescriptor desc;
			desc.mBufferUsage = rhi::BufferUsage::Static;
			desc.mSize = ByteSize( vertices );
			desc.mBufferType = rhi::BufferType::Vertex;
			desc.mData = vertices.data( );
			mScreenQuad.mVbo = mContext.mRenderDevice->CreateBuffer( desc );
		}

		// Screen quad EBO
		if (!mContext.mRenderDevice->IsValid( mScreenQuad.mEbo )) {

			std::vector<uint32> indices = {
				0, 1, 2,
				2, 3, 0
			};

			rhi::FBufferDescriptor desc;
			desc.mBufferUsage = rhi::BufferUsage::Static;
			desc.mSize = ByteSize( indices );
			desc.mBufferType = rhi::BufferType::Element;
			desc.mData = indices.data( );
			mScreenQuad.mEbo = mContext.mRenderDevice->CreateBuffer( desc );

		}

		// Screen quad VAO
		if (!mContext.mRenderDevice->IsValid( mScreenQuad.mVao )) {

			std::vector<rhi::FVertexAttribute> attrs( 2 );
			attrs[ 0 ].mFormat = rhi::DataFormat::Vec3;
			attrs[ 0 ].mRelativeOffset = offsetof( FVertex, mPosition );
			attrs[ 0 ].mShaderLocation = LUM_LAYOUT_POSITION;

			attrs[ 1 ].mFormat = rhi::DataFormat::Vec2;
			attrs[ 1 ].mRelativeOffset = offsetof( FVertex, mUv );
			attrs[ 1 ].mShaderLocation = LUM_LAYOUT_UV;

			rhi::FVertexLayoutDescriptor desc;
			desc.mAttributes = attrs;
			desc.mStride = sizeof( FVertex );
			mScreenQuad.mVao = mContext.mRenderDevice->CreateVertexLayout( desc, mScreenQuad.mVbo );

			mContext.mRenderDevice->AttachElementBufferToLayout( mScreenQuad.mEbo, mScreenQuad.mVao );

		}
		{
			mPostprocessShader = mContext.mShaderMgr->LoadShader( "shaders/postprocess_pass.vert", "shaders/postprocess_pass.frag", RootID::Internal );
		}

		create_screenquad_texture( 500, 500 );
		create_screenquad_fbo( );
	}

	void Renderer::create_screenquad_fbo( ) {

		if (mContext.mRenderDevice->IsValid( mScreenQuad.mFbo ))
			mContext.mRenderDevice->DeleteFramebuffer( mScreenQuad.mFbo );

		rhi::FFramebufferDescriptor desc;
		desc.mColorTex.push_back( { 0, mScreenQuad.mTexture } );
		desc.mDepthTex = mGBuffer.GetTexture( detail::GBufferTexture::Depth );
		mScreenQuad.mFbo = mContext.mRenderDevice->CreateFramebuffer( desc );

	}
	void Renderer::create_screenquad_texture( uint32 w, uint32 h ) {

		if (mContext.mRenderDevice->IsValid( mScreenQuad.mTexture ))
			mContext.mRenderDevice->DeleteTexture( mScreenQuad.mTexture );

		rhi::FTextureDescriptor desc;
		desc.mImageFormat = rhi::ImageFormat::RGBA;
		desc.mImageLayout = rhi::ImageLayout::RGBA16F;
		desc.mTextureType = rhi::TextureType::Texture2D;
		//desc.mSamples = 8;
		desc.mWidth = w;
		desc.mHeight = h;

		mScreenQuad.mTexture = mContext.mRenderDevice->CreateTexture( desc );

	}

	void Renderer::upload_camera_uniform( ) {

		mContext.mRenderDevice->UpdateBuffer( mCameraUBO, &mCameraUBOData );

	}

}