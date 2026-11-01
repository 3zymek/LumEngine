/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#pragma once

#include "Core/Utils/SafePtr.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiCommandBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiResourceSet.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"
#include "Rhi/Interfaces/RhiAdapter.hpp"
#include "Rhi/RhiTypes.hpp"

namespace lum {

	class ISurfaceProvider;

	namespace ev {
		class EventBus;
	} // namespace ev

} // namespace lum

namespace lum::rhi {

	/* @brief Configuration structure used to initialize a render device.
	*
	* Defines required extensions, native window information, adapter requirements,
	* external dependencies, and resource limits for the render device.
	*/
	struct RenderDeviceCreateInfo {

		/* @brief Instance extensions required during render device initialization. */
		std::vector<const char*> m_InstanceExtensions{};

		/* @brief Native window handle used for surface creation. */
		void* m_NativeWindowHandle = nullptr;

		/* @brief Enables validation layers and debug checks when supported. */
		bool m_EnableValidation = false;

		/* @brief Requirements used to select a compatible graphics adapter. */
		AdapterRequirements	m_AdapterRequirements{};

		/* @brief Provides the native rendering surface. REQUIRED */
		SafePtr<ISurfaceProvider> m_SurfaceProvider = nullptr;

		/* @brief Event bus used for render device events. REQUIRED */
		SafePtr<ev::EventBus> m_EventBus = nullptr;

	};

	/* @brief Abstract interface for a graphics rendering device.
	*
	* Defines the API-independent interface for initializing the rendering backend,
	* managing GPU resources, updating frames, and submitting command buffers.
	* Backend-specific implementations provide the actual rendering functionality.
	*/
	class IRenderDevice {
	public:

		//=========================================================================
		// Device lifecycle
		//=========================================================================

		/* @brief Initializes the render device using the provided configuration.
		* @param info Configuration containing device requirements and initialization settings.
		*/
		virtual void Initialize( const RenderDeviceCreateInfo& info ) noexcept = 0;

		/* @brief Releases all resources owned by the render device. */
		virtual void Finalize( ) noexcept = 0;

		/* @brief Updates the render device state for the next frame. */
		virtual void UpdateFrame( ) noexcept = 0;



		//=========================================================================
		// Buffer management
		//=========================================================================

		/* @brief Creates a GPU buffer using the provided configuration.
		* @param info Buffer creation parameters.
		* @return Handle identifying the created buffer.
		*/
		virtual BufferHandle CreateBuffer( const BufferCreateInfo& info ) noexcept = 0;

		/* @brief Destroys a GPU buffer and invalidates its handle.
		* @param handle Handle of the buffer to destroy.
		*/
		virtual void DestroyBuffer( BufferHandle& handle ) noexcept = 0;



		//=========================================================================
		// Pipeline management
		//=========================================================================

		/* @brief Creates a graphics or compute pipeline.
		* @param info Pipeline creation parameters.
		* @return Handle identifying the created pipeline.
		*/
		virtual PipelineHandle CreatePipeline( const PipelineCreateInfo& info ) noexcept = 0;

		/* @brief Destroys a pipeline and invalidates its handle.
		* @param handle Handle of the pipeline to destroy.
		*/
		virtual void DestroyPipeline( PipelineHandle& handle ) noexcept = 0;



		//=========================================================================
		// Image management
		//=========================================================================

		/* @brief Creates a GPU image using the provided configuration.
		* @param info Image creation parameters.
		* @return Handle identifying the created image.
		*/
		virtual ImageHandle CreateImage( const ImageCreateInfo& info ) noexcept = 0;

		/* @brief Destroys a GPU image and invalidates its handle.
		* @param handle Handle of the image to destroy.
		*/
		virtual void DestroyImage( ImageHandle& handle ) noexcept = 0;



		//=========================================================================
		// Sampler management
		//=========================================================================

		/* @brief Creates a sampler used for image sampling operations.
		* @param info Sampler creation parameters.
		* @return Handle identifying the created sampler.
		*/
		virtual SamplerHandle CreateSampler( const SamplerCreateInfo& info ) noexcept = 0;

		/* @brief Destroys a sampler and invalidates its handle.
		* @param handle Handle of the sampler to destroy.
		*/
		virtual void DestroySampler( SamplerHandle& handle ) noexcept = 0;



		//=========================================================================
		// Resource layout management
		//=========================================================================

		/* @brief Creates a resource layout describing shader resource bindings.
		* @param info Resource layout creation parameters.
		* @return Handle identifying the created resource layout.
		*/
		virtual ResourceLayoutHandle CreateResourceLayout( const ResourceLayoutCreateInfo& info ) noexcept = 0;

		/* @brief Destroys a resource layout and invalidates its handle.
		* @param handle Handle of the resource layout to destroy.
		*/
		virtual void DestroyResourceLayout( ResourceLayoutHandle& handle ) noexcept = 0;



		//=========================================================================
		// Resource set management
		//=========================================================================

		/* @brief Creates a resource set using the provided configuration.
		* @param info Resource set creation parameters.
		* @return Handle identifying the created resource set.
		*/
		virtual ResourceSetHandle CreateResourceSet( const ResourceSetCreateInfo& info ) noexcept = 0;

		/* @brief Destroys a resource set and invalidates its handle.
		* @param handle Handle of the resource set to destroy.
		*/
		virtual void DestroyResourceSet( ResourceSetHandle& handle ) noexcept = 0;



		//=========================================================================
		// Command submission
		//=========================================================================

		/* @brief Submits a recorded command buffer to the render device.
		* @param buffer Command buffer containing recorded rendering commands.
		*/
		virtual void SendCommandBuffer( CommandBuffer& buffer ) noexcept = 0;

		virtual ~IRenderDevice( ) = default;

	};

} // namespace lum::rhi