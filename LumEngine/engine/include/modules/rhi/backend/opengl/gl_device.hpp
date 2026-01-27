#pragma once
#include "rhi/core/rhi_device.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/rhi_pch.hpp"
#include "core/core_defines.hpp"
#include "rhi/core/rhi_vertex_layout.hpp"
#include "core/asset_service.hpp"
#include "core/utils/sparse_set.hpp"
namespace lum::gl {

	class GL_Device : public rhi::RHI_Device {

		using BufferHandle		= rhi::BufferHandle;
		using BufferDescriptor	= rhi::BufferDescriptor;
		using Mapflag			= rhi::Mapflag;

		using VertexLayout				= rhi::VertexLayout;
		using VertexLayoutHandle		= rhi::VertexLayoutHandle;
		using VertexLayoutDescriptor	= rhi::VertexLayoutDescriptor;

		using ShaderHandle		= rhi::ShaderHandle;
		using ShaderDescriptor	= rhi::ShaderDescriptor;

		using Texture			= rhi::Texture;
		using TextureHandle		= rhi::TextureHandle;
		using TextureDescriptor = rhi::TextureDescriptor;

		using Sampler			= rhi::Sampler;
		using SamplerHandle		= rhi::SamplerHandle;
		using SamplerDescriptor = rhi::SamplerDescriptor;

		using Framebuffer					= rhi::Framebuffer;
		using FramebufferHandle				= rhi::FramebufferHandle;
		using FramebufferTextureDescriptor	= rhi::FramebufferTextureDescriptor;
		using FramebufferAttachment			= rhi::FramebufferAttachment;

		using Pipeline				= rhi::Pipeline;
		using PipelineHandle		= rhi::PipelineHandle;
		using PipelineDescriptor	= rhi::PipelineDescriptor;

		using State			= rhi::State;
		using PolygonMode	= rhi::PolygonMode;
		using Face			= rhi::Face;

	public:

		GL_Device(Window* win) : window(win) {}

		///////////////////////////////////////////////////
		/// Buffers
		///////////////////////////////////////////////////

		BufferHandle		create_vertex_buffer			( const BufferDescriptor& )							override;
		BufferHandle		create_element_buffer			( const BufferDescriptor& )							override;
		BufferHandle		create_uniform_buffer			( const BufferDescriptor& )							override;
		void				update_buffer				( const BufferHandle&, cvptr, usize, usize )		override;
		void				delete_buffer				( BufferHandle& )									override;
		vptr				map_buffer					( const BufferHandle&, Mapflag, usize, usize )		override;
		void				unmap_buffer					( const BufferHandle& )								override;
		void				attach_element_buffer_to_layout	( const BufferHandle&, const VertexLayoutHandle& )	override;
		void				set_uniform_buffer_binding		( const BufferHandle&, int32 )						override;



		///////////////////////////////////////////////////
		/// Framebuffers
		///////////////////////////////////////////////////

		FramebufferHandle	create_framebuffer			( )															override;
		TextureHandle		create_framebuffer_texture	( const FramebufferTextureDescriptor& )						override;
		void				set_framebuffer_color_texture	( const FramebufferHandle&, const TextureHandle&, uint8 )	override;
		void				set_framebuffer_depth_texture	( const FramebufferHandle&, const TextureHandle&)			override;
		void				set_framebuffer_stencil_texture( const FramebufferHandle&, const TextureHandle&)			override;
		void				clear_framebuffer			( FramebufferHandle, glm::vec4, float32)					override;
		void				delete_framebuffer			( FramebufferHandle& )										override;
		void				bind_framebuffer				( const FramebufferHandle& )								override;
		void				unbind_framebuffer			( )															override;



		///////////////////////////////////////////////////
		/// Layouts
		///////////////////////////////////////////////////

		VertexLayoutHandle	create_vertex_layout( const VertexLayoutDescriptor&, const BufferHandle&)	override;
		void				delete_vertex_layout( VertexLayoutHandle& )								override;



		///////////////////////////////////////////////////
		/// Shaders ( !!! ADD CACHE LOCATIONS !!! )
		///////////////////////////////////////////////////

		ShaderHandle	create_shader( const ShaderDescriptor& )							override;
		void			bind_shader	( const ShaderHandle& )								override;
		void			delete_shader( ShaderHandle& )									override;
		void			set_mat4		( const ShaderHandle&, ccharptr, const glm::mat4& )	override;
		void			set_f		( const ShaderHandle&, ccharptr, float32 )			override;
		void			set_i		( const ShaderHandle&, ccharptr, int32 )			override;
		void			set_vec4		( const ShaderHandle&, ccharptr, const glm::vec4& )	override;
		void			set_vec3		( const ShaderHandle&, ccharptr, const glm::vec3& )	override;
		void			set_vec2		( const ShaderHandle&, ccharptr, const glm::vec2& )	override;



		///////////////////////////////////////////////////
		/// Textures
		///////////////////////////////////////////////////

		TextureHandle	create_texture_2d		( const TextureDescriptor& )		override;
		TextureHandle	create_texture_3d		( const TextureDescriptor& )		override;
		void			delete_texture		( TextureHandle& )					override;
		void			set_texture_binding	( const TextureHandle&, uint16)		override;
		void			bind_texture			( const TextureHandle&, uint16 )	override;



		///////////////////////////////////////////////////
		/// Samplers
		///////////////////////////////////////////////////

		SamplerHandle	create_sampler		( const SamplerDescriptor& )		override;
		void			set_sampler_binding	( const SamplerHandle&, uint16 )	override;
		void			bind_sampler			( const SamplerHandle&, uint16)		override;
		void			delete_sampler		( SamplerHandle )					override;



		///////////////////////////////////////////////////
		/// Pipelines
		///////////////////////////////////////////////////

		rhi::PipelineHandle create_pipeline(const PipelineDescriptor& )	override;
		void delete_pipeline(PipelineHandle&)							override;
		void bind_pipeline(const PipelineHandle&)						override;



		///////////////////////////////////////////////////
		/// Other
		///////////////////////////////////////////////////

		void draw			( const VertexLayoutHandle&, uint32 )	override;
		void draw_elements	( const VertexLayoutHandle&, uint32 )	override;
		void begin_frame		( )										override;
		void end_frame		( )										override;



	protected:

		LUM_CONST_VAR_QUALIFIER
		static GLenum skPolygonModeLookup[] = 
		{ 
			GL_POINT, 
			GL_LINE, 
			GL_FILL 
		};

		LUM_CONST_VAR_QUALIFIER
		static GLenum skFacesLookup[] =
		{ 
			GL_FRONT, 
			GL_BACK, 
			GL_FRONT_AND_BACK 
		};

		LUM_CONST_VAR_QUALIFIER
		static GLenum skTextureMinFilterLookup[] =
		{
			GL_LINEAR,
			GL_LINEAR_MIPMAP_NEAREST,
			GL_LINEAR_MIPMAP_LINEAR,

			GL_NEAREST,
			GL_NEAREST_MIPMAP_NEAREST,
			GL_NEAREST_MIPMAP_LINEAR
		};

		LUM_CONST_VAR_QUALIFIER
		static GLenum skSamplerWrapLookup[] =
		{
			GL_REPEAT,
			GL_MIRRORED_REPEAT,

			GL_CLAMP_TO_EDGE,
			GL_CLAMP_TO_BORDER
		};

		LUM_CONST_VAR_QUALIFIER
		static GLenum skCompareFlagLookup[] =
		{
			GL_LESS,
			GL_LEQUAL,
			GL_GREATER,
			GL_GEQUAL,
			GL_NOTEQUAL,
			GL_ALWAYS,
			GL_NEVER
		};

		LUM_CONST_VAR_QUALIFIER
		static GLenum skBlendFactorLookup[] =
		{
			GL_ZERO,
			GL_ONE,

			GL_SRC_COLOR,
			GL_ONE_MINUS_SRC_COLOR,
			GL_SRC_ALPHA,
			GL_ONE_MINUS_SRC_ALPHA,

			GL_DST_COLOR,
			GL_ONE_MINUS_DST_COLOR,
			GL_DST_ALPHA,
			GL_ONE_MINUS_DST_ALPHA,

			GL_CONSTANT_COLOR,
			GL_ONE_MINUS_CONSTANT_COLOR,
			GL_CONSTANT_ALPHA,
			GL_ONE_MINUS_CONSTANT_ALPHA,

			GL_SRC_ALPHA_SATURATE,

			GL_SRC1_COLOR,
			GL_ONE_MINUS_SRC1_COLOR,
			GL_SRC1_ALPHA,
			GL_ONE_MINUS_SRC1_ALPHA,

		};
		LUM_CONST_VAR_QUALIFIER
		static GLenum skBlendOpLookup[] =
		{
			GL_FUNC_ADD,
			GL_FUNC_SUBTRACT,
			GL_FUNC_REVERSE_SUBTRACT,
			GL_MIN,
			GL_MAX
		};

		Window* window = nullptr;

		///////////////////////////////////////////////////
		/// Private helpers ( !!! REPAIR TYPE CASTING !!! )
		///////////////////////////////////////////////////

		void		_cache_uniforms_locations		( );
		bool		_is_valid_buffer_descriptor		( const BufferDescriptor&)		noexcept;
		GLbitfield	_translate_mapping_flags		( Mapflag )						noexcept;

	};

}