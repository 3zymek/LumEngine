//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Bridge between engine data and the RHI backend.
//
//=============================================================================//

#include "core/core_common.hpp"

#include "render/renderer.hpp"
#include "render/texture_manager.hpp"
#include "render/shader_manager.hpp"

#include "entity/components/transform.hpp"

#include "event/event_bus.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void Renderer::Initialize( const FRendererContext& ctx ) {

		mContext = ctx;

		mGeometryPass.		Initialize(mContext);
		mLightPass.			Initialize(mContext);
		mEnvironmentPass.	Initialize(mContext);
		mShadowPass.		Initialize(mContext);
		mGBuffer.			Initialize(mContext, 500, 500);

		init();

	}

	void Renderer::UpdateCamera( const FRenderCamera& camera ) {

		mCameraStruct.mPosition = glm::vec4(camera.mPosition, 0.0);
		mCameraStruct.mProjection = camera.mProjection;
		mCameraStruct.mView = camera.mView;
		mCameraStruct.mInvViewProj = glm::inverse(camera.mProjection * camera.mView);

		upload_camera_uniform();

	}

	void Renderer::BeginFrame( ) {

		mContext.mRenderDevice->NewFrame();

		mLightPass.ClearLights();
		mContext.mRenderDevice->Clear(
			rhi::BufferBit::Color |
			rhi::BufferBit::Depth |
			rhi::BufferBit::Stencil
		);

	}

	void Renderer::EndFrame( ) {

		mShadowPass.Execute(mGeometryPass, mLightPass);

		mGeometryPass.Execute(mGBuffer);

		mLightPass.Execute(mShadowPass, mGBuffer, mScreenQuad);

		mEnvironmentPass.Execute(mGBuffer);

		mContext.mRenderDevice->SwapBuffers();

	}





	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void Renderer::init( ) {

		mContext.mEventBus->SubscribePermanently<EWindowResized>([this](const EWindowResized& e) {
			this->mContext.mRenderDevice->SetViewport(0, 0, e.mWidth, e.mHeight);
		});

		{ // Camera Uniform
			rhi::FBufferDescriptor desc;
			desc.mBufferUsage = rhi::BufferUsage::Dynamic;
			desc.mMapFlags = rhi::MapFlag::Write;
			desc.mSize = sizeof(detail::FCameraUniformBuffer);
			desc.mBufferType = rhi::BufferType::Uniform;
			mCameraBuffer = mContext.mRenderDevice->CreateBuffer(desc);
			mContext.mRenderDevice->SetUniformBufferBinding(mCameraBuffer, LUM_UBO_CAMERA_BINDING);
		}
		{ // Screen quad VBO

			std::vector<FVertex> vertices = {
				{ {-1.f, -1.f, 0.f}, {}, {0.f, 0.f}, {}, {} },
				{ { 1.f, -1.f, 0.f}, {}, {1.f, 0.f}, {}, {} },
				{ { 1.f,  1.f, 0.f}, {}, {1.f, 1.f}, {}, {} },
				{ {-1.f,  1.f, 0.f}, {}, {0.f, 1.f}, {}, {} },
			};

			rhi::FBufferDescriptor desc;
			desc.mBufferUsage = rhi::BufferUsage::Static;
			desc.mSize = ByteSize(vertices);
			desc.mBufferType = rhi::BufferType::Vertex;
			desc.mData = vertices.data();
			mScreenQuad.mVbo = mContext.mRenderDevice->CreateBuffer(desc);
		}
		{ // Screen quad EBO


			std::vector<uint32> indices = {
				0, 1, 2,
				2, 3, 0
			};

			rhi::FBufferDescriptor desc;
			desc.mBufferUsage = rhi::BufferUsage::Static;
			desc.mSize = ByteSize(indices);
			desc.mBufferType = rhi::BufferType::Element;
			desc.mData = indices.data();
			mScreenQuad.mEbo = mContext.mRenderDevice->CreateBuffer(desc);
		}
		{ // Screen quad VAO

			std::vector<rhi::FVertexAttribute> attrs(2);
			attrs[0].mFormat = rhi::DataFormat::Vec3;
			attrs[0].mRelativeOffset = offsetof(FVertex, mPosition);
			attrs[0].mShaderLocation = LUM_LAYOUT_POSITION;

			attrs[1].mFormat = rhi::DataFormat::Vec2;
			attrs[1].mRelativeOffset = offsetof(FVertex, mUv);
			attrs[1].mShaderLocation = LUM_LAYOUT_UV;

			rhi::FVertexLayoutDescriptor desc;
			desc.mAttributes = attrs;
			desc.mStride = sizeof(FVertex);
			mScreenQuad.mVao = mContext.mRenderDevice->CreateVertexLayout(desc, mScreenQuad.mVbo);

			mContext.mRenderDevice->AttachElementBufferToLayout(mScreenQuad.mEbo, mScreenQuad.mVao);

		}
	}

	void Renderer::upload_camera_uniform( ) {

		mContext.mRenderDevice->UpdateBuffer(mCameraBuffer, &mCameraStruct, 0, 0);

	}

}