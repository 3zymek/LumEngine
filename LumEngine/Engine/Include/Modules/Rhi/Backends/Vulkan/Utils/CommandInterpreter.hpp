#pragma once

#include <algorithm>
#include <concepts>
#include <new>
#include <type_traits>
#include <utility>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "Core/CoreTypes.hpp"
#include "Core/Utils/TypeUtils.hpp"
#include "Rhi/Interfaces/Core/RhiCommandBuffer.hpp"

namespace lum::rhi::vk {

	class VulkanDevice;

	class VulkanCommandCall {
	public:
		using LambdaStorage = std::aligned_storage<256, alignof(std::max_align_t)>::type;
		using InvokeFn = void (*)(void* capture, VkCommandBuffer cmdBuffer, uint8*& ptr);
		using DestroyFn = void (*)(void* capture);

		LambdaStorage m_Storage{};
		InvokeFn      m_InvokeFn = nullptr;
		DestroyFn     m_DestroyFn = nullptr;

		~VulkanCommandCall( ) {
			if (m_DestroyFn) {
				m_DestroyFn( &m_Storage );
			}
		}
		VulkanCommandCall( ) = default;
		VulkanCommandCall( const VulkanCommandCall& ) = delete;
		VulkanCommandCall& operator=( const VulkanCommandCall& ) = delete;
		VulkanCommandCall( VulkanCommandCall&& other ) noexcept
			: m_InvokeFn( other.m_InvokeFn ), m_DestroyFn( other.m_DestroyFn ) {
			std::memcpy( &m_Storage, &other.m_Storage, sizeof( LambdaStorage ) );
			other.m_DestroyFn = nullptr;
		}
		VulkanCommandCall& operator=( VulkanCommandCall&& other ) noexcept {
			if (this != &other) {
				if (m_DestroyFn) {
					m_DestroyFn( &m_Storage );
				}
				m_InvokeFn = other.m_InvokeFn;
				m_DestroyFn = other.m_DestroyFn;
				std::memcpy( &m_Storage, &other.m_Storage, sizeof( LambdaStorage ) );
				other.m_DestroyFn = nullptr;
			}
			return *this;
		}
	};

	class VulkanCommandInterpreter {
	public:

		VulkanCommandInterpreter( ) noexcept {
			m_CommandCalls.resize( t_EnumCount<CommandType> );
		}

		template<typename tCommandData, typename tLambda>
			requires std::derived_from<tCommandData, Command>
		void RegisterTypedCall( tLambda&& lambda ) {

			LUM_SASSERT( cHasCmdType<tCommandData> );
			
			register_call(
				tCommandData::s_Type,
				[ lambda = std::forward<tLambda>( lambda ) ]( VkCommandBuffer vkCmdBuff, uint8*& ptr ) {
					const auto* data = reinterpret_cast<const tCommandData*>(ptr);
					ptr += sizeof( tCommandData );
					lambda( vkCmdBuff, *data );
				}
			);

		}

		void ExecuteCall( CommandType type, VkCommandBuffer vkCmdBuf, uint8*& ptr ) noexcept;

	private:

		template<typename tLambda>
		void register_call( CommandType type, tLambda&& lambda ) noexcept {

			using DecayedLambda = std::decay_t<tLambda>;
			
			LUM_ASSERT( sizeof( DecayedLambda ) <= sizeof( VulkanCommandCall::LambdaStorage ), "Lambda is too large for LambdaStorage buffer!" );

			VulkanCommandCall call{};
			new (&call.m_Storage) DecayedLambda( std::forward<tLambda>( lambda ) );

			call.m_DestroyFn = []( void* capture ) -> void {
				reinterpret_cast<DecayedLambda*>(capture)->~DecayedLambda( );
				};

			call.m_InvokeFn = []( void* capture, VkCommandBuffer cmdBuffer, uint8*& ptr ) -> void {
				(*reinterpret_cast<DecayedLambda*>(capture))(cmdBuffer, ptr);
				};

			m_CommandCalls[ ToUnderlyingEnum( type ) ] = std::move( call );
		}

		std::vector<VulkanCommandCall> m_CommandCalls{};
	};


} // namespace lum::rhi::vk