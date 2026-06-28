#include "render/passes/postprocess_pass.hpp"
#include "render/shader_manager.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void PostprocessPass::Initialize( const RendererContext& ctx ) {

		ValidateRendererContext( ctx );

		mContext = ctx;

		mShader = mContext.mShaderMgr->LoadShader( "shaders/postprocess_pass.vert", "shaders/postprocess_pass.frag", ResourceRoot::Internal );

	}

	void PostprocessPass::Execute( const detail::FScreenQuad& quad, const PostprocessPassExecute& exe ) {
		
		mContext.mRenderDev->BindFramebuffer( quad.mPostprocessFbo );
		mContext.mRenderDev->BindShader( mShader );
		mContext.mRenderDev->SetUniform( mShader, 0, exe.mJitterOffset );
		mContext.mRenderDev->BindTexture( quad.mSceneTex, LUM_TEX_FRAME );
		if(exe.mTAAEnabled)
			mContext.mRenderDev->BindTexture( exe.mPreviousFrameTex, LUM_TEX_FRAME_HISTORY );

		mContext.mRenderDev->DrawElements( quad.mVao, 6 );

		if (exe.mTAAEnabled)
			mContext.mRenderDev->Copy( quad.mPostprocessTex, exe.mPreviousFrameTex );

		mContext.mRenderDev->BindFramebuffer( rhi::kDefaultFramebuffer );


	}

}