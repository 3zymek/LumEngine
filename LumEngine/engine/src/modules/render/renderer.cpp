//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Bridge between engine data and the RHI backend.
//
//=============================================================================//

#include "core/core_common.hpp"

#include "render/renderer.hpp"

#include "event/event_bus.hpp"
#include "event/events/window_events.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void Renderer::Initialize( const FRendererContext& ctx ) {

		ValidateRendererContext( ctx );

		mContext = ctx;

		mGeometryPass.Initialize( mContext );
		mLightPass.Initialize( mContext );
		mEnvironmentPass.Initialize( mContext );
		mShadowPass.Initialize( mContext );
		mPostprocessPass.Initialize( mContext );
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
		mContext.mRenderDev->BindFramebuffer( rhi::gDefaultFramebuffer );
		mContext.mRenderDev->Clear(
			rhi::BufferBit::Color |
			rhi::BufferBit::Depth |
			rhi::BufferBit::Stencil
		);

	}

	void Renderer::EndFrame( ) {

		mContext.mRenderDev->BindFramebuffer( mScreenQuad.mSceneFbo );
		mContext.mRenderDev->Clear(
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

		mPostprocessPass.Execute( mScreenQuad );

	}





	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void Renderer::init( ) {

		mContext.mEvBus->SubscribePermanently<EWindowResized>(
			[&]( const EWindowResized& e ) {
				if (e.mWidth <= 0 || e.mHeight <= 0) return;
				mContext.mRenderDev->SetViewport( 0, 0, e.mWidth, e.mHeight );
				create_screenquad_texture( e.mWidth, e.mHeight );
				create_screenquad_fbo( );
			}
		);

		// Camera Uniform
		if (!mContext.mRenderDev->IsValid( mCameraUBO )) {

			rhi::FBufferDescriptor desc;
			desc.mBufferUsage = rhi::BufferUsage::Dynamic;
			desc.mMapFlags = rhi::MapFlag::Write;
			desc.mSize = sizeof( detail::FCameraUBOData );
			desc.mBufferType = rhi::BufferType::Uniform;
			mCameraUBO = mContext.mRenderDev->CreateBuffer( desc );
			mContext.mRenderDev->SetUniformBufferBinding( mCameraUBO, LUM_UBO_CAMERA_BINDING );

		}

		// Screen quad VBO
		if (!mContext.mRenderDev->IsValid( mScreenQuad.mVbo )) {

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
			mScreenQuad.mVbo = mContext.mRenderDev->CreateBuffer( desc );
		}

		// Screen quad EBO
		if (!mContext.mRenderDev->IsValid( mScreenQuad.mEbo )) {

			std::vector<uint32> indices = {
				0, 1, 2,
				2, 3, 0
			};

			rhi::FBufferDescriptor desc;
			desc.mBufferUsage = rhi::BufferUsage::Static;
			desc.mSize = ByteSize( indices );
			desc.mBufferType = rhi::BufferType::Element;
			desc.mData = indices.data( );
			mScreenQuad.mEbo = mContext.mRenderDev->CreateBuffer( desc );

		}

		// Screen quad VAO
		if (!mContext.mRenderDev->IsValid( mScreenQuad.mVao )) {

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
			mScreenQuad.mVao = mContext.mRenderDev->CreateVertexLayout( desc, mScreenQuad.mVbo );

			mContext.mRenderDev->AttachElementBufferToLayout( mScreenQuad.mEbo, mScreenQuad.mVao );

		}

		create_screenquad_texture( 500, 500 );
		create_screenquad_fbo( );
	}

	void Renderer::create_screenquad_fbo( ) {

		if (mContext.mRenderDev->IsValid( mScreenQuad.mSceneFbo ))
			mContext.mRenderDev->DeleteFramebuffer( mScreenQuad.mSceneFbo );

		if (mContext.mRenderDev->IsValid( mScreenQuad.mPostprocessFbo ))
			mContext.mRenderDev->DeleteFramebuffer( mScreenQuad.mPostprocessFbo );

		{
			rhi::FFramebufferDescriptor desc;
			desc.mColorTex.push_back( { 0, mScreenQuad.mSceneTexture } );
			desc.mDepthTex = mGBuffer.GetTexture( detail::GBufferTexture::Depth );
			mScreenQuad.mSceneFbo = mContext.mRenderDev->CreateFramebuffer( desc );
		}
		{
			rhi::FFramebufferDescriptor desc;
			desc.mColorTex.push_back( { 0, mScreenQuad.mPostprocessTexture } );
			desc.mDepthTex = mGBuffer.GetTexture( detail::GBufferTexture::Depth );
			mScreenQuad.mPostprocessFbo = mContext.mRenderDev->CreateFramebuffer( desc );
		}

	}
	void Renderer::create_screenquad_texture( uint32 w, uint32 h ) {

		if (mContext.mRenderDev->IsValid( mScreenQuad.mSceneTexture ))
			mContext.mRenderDev->DeleteTexture( mScreenQuad.mSceneTexture );

		if (mContext.mRenderDev->IsValid( mScreenQuad.mPostprocessTexture ))
			mContext.mRenderDev->DeleteTexture( mScreenQuad.mPostprocessTexture );

		{
			rhi::FTextureDescriptor desc;
			desc.mImageFormat = rhi::ImageFormat::RGBA;
			desc.mImageLayout = rhi::ImageLayout::RGBA16F;
			desc.mTextureType = rhi::TextureType::Texture2D;
			desc.mWidth = w;
			desc.mHeight = h;
			mScreenQuad.mSceneTexture = mContext.mRenderDev->CreateTexture( desc );
		}
		{
			rhi::FTextureDescriptor desc;
			desc.mImageFormat = rhi::ImageFormat::RGBA;
			desc.mImageLayout = rhi::ImageLayout::RGBA16F;
			desc.mTextureType = rhi::TextureType::Texture2D;
			desc.mWidth = w;
			desc.mHeight = h;
			mScreenQuad.mPostprocessTexture = mContext.mRenderDev->CreateTexture( desc );
		}

	}

	void Renderer::upload_camera_uniform( ) {

		mContext.mRenderDev->UpdateBuffer( mCameraUBO, &mCameraUBOData );

	}

}