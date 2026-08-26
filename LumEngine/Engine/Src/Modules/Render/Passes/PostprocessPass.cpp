#include "Render/Passes/PostprocessPass.hpp"
#include "Render/ShaderManager.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void PostprocessPass::Initialize( RendererContext& ctx ) {

		ctx.Validate( );

		m_Ctx = ctx;

		m_Shader = m_Ctx().m_ShaderMgr( ).LoadShader( "shaders/postprocess_pass.vert", "shaders/postprocess_pass.frag", ResourceRoot::Internal );

	}

	void PostprocessPass::Execute( const detail::ScreenQuad& quad, const PostprocessPassExecute& exe ) {
		
		auto& device = m_Ctx( ).m_RenderDev( );
		device.BindFramebuffer( quad.m_PostprocessFbo );
		device.BindShader( m_Shader );
		device.SetUniform( m_Shader, 0, exe.m_JitterOffset );
		device.BindTexture( quad.m_SceneTex, LUM_TEX_FRAME );
		if(exe.m_TAAEnabled)
			device.BindTexture( exe.m_PreviousFrameTex, LUM_TEX_FRAME_HISTORY );

		device.DrawElements( quad.m_Vao, 6 );

		if (exe.m_TAAEnabled)
			device.Copy( quad.m_PostprocessTex, exe.m_PreviousFrameTex );

		device.BindFramebuffer( rhi::kDefaultFramebuffer );


	}

}