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

		mCtx = ctx;

		mGeometryPass.Initialize( mCtx );
		mLightPass.Initialize( mCtx );
		mEnvironmentPass.Initialize( mCtx );
		mShadowSys.Initialize( mCtx );
		mPostprocessPass.Initialize( mCtx );
		mDefferedBuffer.Initialize( mCtx, 500, 500 );

		init( );

	}

	void Renderer::UpdateCamera( const RenderCamera& camera ) {

		Matrix4 jittered = mTemporalAA.ApplyJitter( camera.mProjection );

		mCameraGpu.mPosition = Vector4( camera.mPosition, 0.0f );
		mCameraGpu.mProjection = jittered;
		mCameraGpu.mView = camera.mView;
		mCameraGpu.mInvViewProj = Inverse( jittered * camera.mView );

		upload_camera_uniform( );

	}

	void Renderer::BeginFrame( ) {

		mLightPass.ClearLights( ); // clear added lights from previous frame
		mCtx.mRenderDev->BindFramebuffer( rhi::kDefaultFramebuffer ); // bind default framebuffer
		mCtx.mRenderDev->Clear( rhi::BufferBit::Color | rhi::BufferBit::Depth | rhi::BufferBit::Stencil ); // clear default framebuffer

	}

	void Renderer::EndFrame( ) {

		mCtx.mRenderDev->BindFramebuffer( mScreenQuad.mSceneFbo );
		mCtx.mRenderDev->Clear( rhi::BufferBit::Color | rhi::BufferBit::Depth | rhi::BufferBit::Stencil ); // clear scene framebuffer

		mShadowSys.Execute( mGeometryPass, mLightPass ); // calculate shadows
		mDefferedBuffer.Clear( );
		mGeometryPass.Execute( mDefferedBuffer ); // draw geometry
		
		mLightPassExecuteCtx.mIrradianceMap = mEnvironmentPass.GetTexture( detail::IBLTexture::IrradianceMap );
		mLightPassExecuteCtx.mPrefilteredEnvMap = mEnvironmentPass.GetTexture( detail::IBLTexture::PrefilteredMap );
		mLightPassExecuteCtx.mShadowMap = mShadowSys.mDirectionalLight.GetShadowMap( );

		mCtx.mRenderDev->BindFramebuffer( mScreenQuad.mSceneFbo );
		mLightPass.Execute( mDefferedBuffer, mScreenQuad, mLightPassExecuteCtx ); // apply lightning
		
		mEnvironmentPass.Execute( mDefferedBuffer, mScreenQuad ); // apply skybox

		{
			PostprocessPassExecute desc{};
			mPostprocessPass.Execute( mScreenQuad, desc ); // apply postprocess effects
		}

	}





	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void Renderer::init( ) {

		mTemporalAA.Initialize( mCtx.mRenderDev );

		mCtx.mEvBus->SubscribePermanently<EWindowResized>(
			[&]( const EWindowResized& e ) {
				if (e.mWidth <= 0 || e.mHeight <= 0) return;
				mCtx.mRenderDev->SetViewport( 0, 0, e.mWidth, e.mHeight );
				ensure_screenquad_texture( e.mWidth, e.mHeight );
				ensure_screenquad_fbo( );
			}
		);

		// Camera Uniform
		if (!mCtx.mRenderDev->IsValid( mCameraUbo )) {

			rhi::BufferCreateInfo desc;
			desc.mBufferUsage = rhi::BufferUsage::Dynamic;
			desc.mMapFlags = rhi::MapFlag::Write;
			desc.mSize = sizeof( detail::CameraGPU );
			desc.mBufferType = rhi::BufferType::Uniform;
			mCameraUbo = mCtx.mRenderDev->CreateBuffer( desc );
			mCtx.mRenderDev->SetUniformBufferBinding( mCameraUbo, LUM_UBO_CAMERA_BINDING );

		}

		// Screen quad VBO
		if (!mCtx.mRenderDev->IsValid( mScreenQuad.mVbo )) {

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
			mScreenQuad.mVbo = mCtx.mRenderDev->CreateBuffer( desc );
		}

		// Screen quad EBO
		if (!mCtx.mRenderDev->IsValid( mScreenQuad.mEbo )) {

			std::vector<uint32> indices = {
				0, 1, 2,
				2, 3, 0
			};

			rhi::BufferCreateInfo desc;
			desc.mBufferUsage = rhi::BufferUsage::Static;
			desc.mSize = ComputeByteSize( indices );
			desc.mBufferType = rhi::BufferType::Element;
			desc.mData = indices.data( );
			mScreenQuad.mEbo = mCtx.mRenderDev->CreateBuffer( desc );

		}

		// Screen quad VAO
		if (!mCtx.mRenderDev->IsValid( mScreenQuad.mVao )) {

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
			mScreenQuad.mVao = mCtx.mRenderDev->CreateVertexLayout( desc, mScreenQuad.mVbo );

			mCtx.mRenderDev->AttachElementBufferToLayout( mScreenQuad.mEbo, mScreenQuad.mVao );

		}

		ensure_screenquad_texture( 500, 500 );
		ensure_screenquad_fbo( );
	}

	void Renderer::ensure_screenquad_fbo( ) {

		mCtx.mRenderDev->Delete( mScreenQuad.mSceneFbo );
		mCtx.mRenderDev->Delete( mScreenQuad.mPostprocessFbo );

		{
			rhi::FramebufferCreateInfo desc;
			desc.mColorTex.push_back( { 0, mScreenQuad.mSceneTex } );
			desc.mDepthTex = mDefferedBuffer.GetAttachment( detail::DeferredBufferAttachment::Depth );
			mScreenQuad.mSceneFbo = mCtx.mRenderDev->CreateFramebuffer( desc );
		}
		{
			rhi::FramebufferCreateInfo desc;
			desc.mColorTex.push_back( { 0, mScreenQuad.mPostprocessTex } );
			desc.mDepthTex = mDefferedBuffer.GetAttachment( detail::DeferredBufferAttachment::Depth );
			mScreenQuad.mPostprocessFbo = mCtx.mRenderDev->CreateFramebuffer( desc );
		}

	}
	void Renderer::ensure_screenquad_texture( uint32 w, uint32 h ) {

		mCtx.mRenderDev->Delete( mScreenQuad.mSceneTex );
		mCtx.mRenderDev->Delete( mScreenQuad.mPostprocessTex );

		{
			rhi::TextureCreateInfo desc;
			desc.mPixelFormat = rhi::TexturePixelFormat::RGBA;
			desc.mInternalFormat = rhi::TextureInternalFormat::RGBA16F;
			desc.mTextureType = rhi::TextureType::Texture2D;
			desc.mWidth = w;
			desc.mHeight = h;
			mScreenQuad.mSceneTex = mCtx.mRenderDev->CreateTexture( desc );
			mTemporalAA.EnsureFrameTex( desc );
		}
		{
			rhi::TextureCreateInfo desc;
			desc.mPixelFormat = rhi::TexturePixelFormat::RGBA;
			desc.mInternalFormat = rhi::TextureInternalFormat::RGBA16F;
			desc.mTextureType = rhi::TextureType::Texture2D;
			desc.mWidth = w;
			desc.mHeight = h;
			mScreenQuad.mPostprocessTex = mCtx.mRenderDev->CreateTexture( desc );
		}

	}

	void Renderer::upload_camera_uniform( ) {

		mCtx.mRenderDev->UpdateBuffer( mCameraUbo, &mCameraGpu );

	}

}