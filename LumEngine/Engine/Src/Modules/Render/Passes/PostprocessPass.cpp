#include "Render/Passes/PostprocessPass.hpp"
#include "Render/ShaderManager.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void PostprocessPass::Initialize( const RendererContext& ctx ) {

		ValidateRendererContext( ctx );

		mCtx = ctx;

		mShader = mCtx.mShaderMgr->LoadShader( "shaders/postprocess_pass.vert", "shaders/postprocess_pass.frag", ResourceRoot::Internal );

	}

	void PostprocessPass::Execute( const detail::ScreenQuad& quad, const PostprocessPassExecute& exe ) {
		
		mCtx.mRenderDev->BindFramebuffer( quad.mPostprocessFbo );
		mCtx.mRenderDev->BindShader( mShader );
		mCtx.mRenderDev->SetUniform( mShader, 0, exe.mJitterOffset );
		mCtx.mRenderDev->BindTexture( quad.mSceneTex, LUM_TEX_FRAME );
		if(exe.mTAAEnabled)
			mCtx.mRenderDev->BindTexture( exe.mPreviousFrameTex, LUM_TEX_FRAME_HISTORY );

		mCtx.mRenderDev->DrawElements( quad.mVao, 6 );

		if (exe.mTAAEnabled)
			mCtx.mRenderDev->Copy( quad.mPostprocessTex, exe.mPreviousFrameTex );

		mCtx.mRenderDev->BindFramebuffer( rhi::kDefaultFramebuffer );


	}

}