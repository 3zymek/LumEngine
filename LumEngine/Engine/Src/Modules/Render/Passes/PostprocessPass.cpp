#include "Render/Passes/PostprocessPass.hpp"
#include "Render/ShaderManager.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void PostprocessPass::Initialize( RendererContext& ctx ) {

		ctx.Validate( );

		mCtx = ctx;

		mShader = mCtx().mShaderMgr( ).LoadShader( "shaders/postprocess_pass.vert", "shaders/postprocess_pass.frag", ResourceRoot::Internal );

	}

	void PostprocessPass::Execute( const detail::ScreenQuad& quad, const PostprocessPassExecute& exe ) {
		
		auto& device = mCtx( ).mRenderDev( );
		device.BindFramebuffer( quad.mPostprocessFbo );
		device.BindShader( mShader );
		device.SetUniform( mShader, 0, exe.mJitterOffset );
		device.BindTexture( quad.mSceneTex, LUM_TEX_FRAME );
		if(exe.mTAAEnabled)
			device.BindTexture( exe.mPreviousFrameTex, LUM_TEX_FRAME_HISTORY );

		device.DrawElements( quad.mVao, 6 );

		if (exe.mTAAEnabled)
			device.Copy( quad.mPostprocessTex, exe.mPreviousFrameTex );

		device.BindFramebuffer( rhi::kDefaultFramebuffer );


	}

}