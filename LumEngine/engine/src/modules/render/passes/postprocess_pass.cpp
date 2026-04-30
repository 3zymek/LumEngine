#include "render/passes/postprocess_pass.hpp"
#include "render/shader_manager.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void PostprocessPass::Initialize( const FRendererContext& ctx ) {

		ValidateRendererContext( ctx );

		mContext = ctx;

		mShader = mContext.mShaderMgr->LoadShader( "shaders/postprocess_pass.vert", "shaders/postprocess_pass.frag", RootID::Internal );

	}

	void PostprocessPass::Execute( const detail::FScreenQuad& quad ) {

		mContext.mRenderDev->BindFramebuffer( quad.mPostprocessFbo );
		mContext.mRenderDev->BindShader( mShader );
		mContext.mRenderDev->BindTexture( quad.mSceneTex, LUM_TEX_FRAME );
		mContext.mRenderDev->BindTexture( quad.mSceneHistoryTex, LUM_TEX_FRAME_HISTORY );

		mContext.mRenderDev->DrawElements( quad.mVao, 6 );

		mContext.mRenderDev->Copy( quad.mSceneTex, quad.mSceneHistoryTex );

		mContext.mRenderDev->BindFramebuffer( rhi::gDefaultFramebuffer );


	}

}