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

	void Renderer::Initialize( const RendererContext& ctx ) {

		ValidateRendererContext( ctx );

		mContext = ctx;

		mGeometryPass.Initialize( mContext );
		mLightPass.Initialize( mContext );
		mEnvironmentPass.Initialize( mContext );
		mShadowSys.Initialize( mContext );
		mPostprocessPass.Initialize( mContext );
		mGBuffer.Initialize( mContext, 500, 500 );

		mContext.mRenderDev->ToggleMultisample( true );

		init( );

	}

	void Renderer::UpdateCamera( const FRenderCamera& camera ) {

		Matrix4 jittered = mTemporalAA.ApplyJitter( camera.mProjection );

		mCameraUBOData.mPosition = Vector4( camera.mPosition, 0.0f );
		mCameraUBOData.mProjection = jittered;
		mCameraUBOData.mView = camera.mView;
		mCameraUBOData.mInvViewProj = Inverse( jittered * camera.mView );

		upload_camera_uniform( );

	}

	void Renderer::BeginFrame( ) {

		mLightPass.ClearLights( );
		mContext.mRenderDev->BindFramebuffer( rhi::kDefaultFramebuffer );
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

		mShadowSys.Execute( mGeometryPass, mLightPass );

		mGeometryPass.Execute( mGBuffer );

		mLightPassExecutables.mIrradianceMap = mEnvironmentPass.GetTexture( detail::IBLTexture::IrradianceMap );
		mLightPassExecutables.mPrefilteredEnvMap = mEnvironmentPass.GetTexture( detail::IBLTexture::PrefilteredMap );
		mLightPassExecutables.mShadowMap = mShadowSys.mDirectionalLight.GetShadowMap( );

		mLightPass.Execute( mGBuffer, mScreenQuad, mLightPassExecutables );

		mEnvironmentPass.Execute( mGBuffer, mScreenQuad );

		{
			PostprocessPassExecute desc;
			desc.bTAAEnabled = false;
			desc.mPreviousFrameTex = mTemporalAA.GetPreviousFrameTex( );
			mPostprocessPass.Execute( mScreenQuad, desc );
		}

	}





	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void Renderer::init( ) {

		mTemporalAA.Initialize( mContext.mRenderDev );

		mContext.mEvBus->SubscribePermanently<EWindowResized>(
			[&]( const EWindowResized& e ) {
				if (e.mWidth <= 0 || e.mHeight <= 0) return;
				mContext.mRenderDev->SetViewport( 0, 0, e.mWidth, e.mHeight );
				ensure_screenquad_texture( e.mWidth, e.mHeight );
				ensure_screenquad_fbo( );
			}
		);

		// Camera Uniform
		if (!mContext.mRenderDev->IsValid( mCameraUBO )) {

			rhi::BufferCreateInfo desc;
			desc.mBufferUsage = rhi::BufferUsage::Dynamic;
			desc.mMapFlags = rhi::MapFlag::Write;
			desc.mSize = sizeof( detail::FCameraUBOData );
			desc.mBufferType = rhi::BufferType::Uniform;
			mCameraUBO = mContext.mRenderDev->CreateBuffer( desc );
			mContext.mRenderDev->SetUniformBufferBinding( mCameraUBO, LUM_UBO_CAMERA_BINDING );

		}

		// Screen quad VBO
		if (!mContext.mRenderDev->IsValid( mScreenQuad.mVbo )) {

			std::vector<Vertex> vertices = {
				{ {-1.f, -1.f, 0.f}, {}, {0.f, 0.f}, {}, {} },
				{ { 1.f, -1.f, 0.f}, {}, {1.f, 0.f}, {}, {} },
				{ { 1.f,  1.f, 0.f}, {}, {1.f, 1.f}, {}, {} },
				{ {-1.f,  1.f, 0.f}, {}, {0.f, 1.f}, {}, {} },
			};

			rhi::BufferCreateInfo desc;
			desc.mBufferUsage = rhi::BufferUsage::Static;
			desc.mSize = ComputeByteSize( vertices );
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

			rhi::BufferCreateInfo desc;
			desc.mBufferUsage = rhi::BufferUsage::Static;
			desc.mSize = ComputeByteSize( indices );
			desc.mBufferType = rhi::BufferType::Element;
			desc.mData = indices.data( );
			mScreenQuad.mEbo = mContext.mRenderDev->CreateBuffer( desc );

		}

		// Screen quad VAO
		if (!mContext.mRenderDev->IsValid( mScreenQuad.mVao )) {

			std::vector<rhi::VertexAttribute> attrs( 2 );
			attrs[ 0 ].mFormat = rhi::DataFormat::Vec3;
			attrs[ 0 ].mRelativeOffset = offsetof( Vertex, mPosition );
			attrs[ 0 ].mShaderLocation = LUM_LAYOUT_POSITION;

			attrs[ 1 ].mFormat = rhi::DataFormat::Vec2;
			attrs[ 1 ].mRelativeOffset = offsetof( Vertex, mUv );
			attrs[ 1 ].mShaderLocation = LUM_LAYOUT_UV;

			rhi::VertexLayoutCreateInfo desc;
			desc.mAttributes = attrs;
			desc.mStride = sizeof( Vertex );
			mScreenQuad.mVao = mContext.mRenderDev->CreateVertexLayout( desc, mScreenQuad.mVbo );

			mContext.mRenderDev->AttachElementBufferToLayout( mScreenQuad.mEbo, mScreenQuad.mVao );

		}

		ensure_screenquad_texture( 500, 500 );
		ensure_screenquad_fbo( );
	}

	void Renderer::ensure_screenquad_fbo( ) {

		mContext.mRenderDev->Delete( mScreenQuad.mSceneFbo );
		mContext.mRenderDev->Delete( mScreenQuad.mPostprocessFbo );

		{
			rhi::FramebufferCreateInfo desc;
			desc.mColorTex.push_back( { 0, mScreenQuad.mSceneTex } );
			desc.mDepthTex = mGBuffer.GetAttachment( detail::DeferredBufferAttachment::Depth );
			mScreenQuad.mSceneFbo = mContext.mRenderDev->CreateFramebuffer( desc );
		}
		{
			rhi::FramebufferCreateInfo desc;
			desc.mColorTex.push_back( { 0, mScreenQuad.mPostprocessTex } );
			desc.mDepthTex = mGBuffer.GetAttachment( detail::DeferredBufferAttachment::Depth );
			mScreenQuad.mPostprocessFbo = mContext.mRenderDev->CreateFramebuffer( desc );
		}

	}
	void Renderer::ensure_screenquad_texture( uint32 w, uint32 h ) {

		mContext.mRenderDev->Delete( mScreenQuad.mSceneTex );
		mContext.mRenderDev->Delete( mScreenQuad.mPostprocessTex );

		{
			rhi::TextureCreateInfo desc;
			desc.mPixelFormat = rhi::TexturePixelFormat::RGBA;
			desc.mInternalFormat = rhi::TextureInternalFormat::RGBA16F;
			desc.mTextureType = rhi::TextureType::Texture2D;
			desc.mWidth = w;
			desc.mHeight = h;
			mScreenQuad.mSceneTex = mContext.mRenderDev->CreateTexture( desc );
			mTemporalAA.EnsureFrameTex( desc );
		}
		{
			rhi::TextureCreateInfo desc;
			desc.mPixelFormat = rhi::TexturePixelFormat::RGBA;
			desc.mInternalFormat = rhi::TextureInternalFormat::RGBA16F;
			desc.mTextureType = rhi::TextureType::Texture2D;
			desc.mWidth = w;
			desc.mHeight = h;
			mScreenQuad.mPostprocessTex = mContext.mRenderDev->CreateTexture( desc );
		}

	}

	void Renderer::upload_camera_uniform( ) {

		mContext.mRenderDev->UpdateBuffer( mCameraUBO, &mCameraUBOData );

	}

}