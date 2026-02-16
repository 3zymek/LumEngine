#pragma once
#include "core/core_pch.hpp"
#include "event/event_common.hpp"
#include "event/event_bus.hpp"
namespace lum {
	namespace ev {
		namespace detail {

			using UnsubscribeInvoke = void (*)(SubscribtionID, EventBus&);

			struct EmitterSlot {
				SubscribtionID		id = std::numeric_limits<SubscribtionID>::max();
				UnsubscribeInvoke	unsub;
			};

		}

		/*! @class EventEmitter
		*  @brief RAII wrapper for the EventBus that manages event subscriptions.
		*
		*  EventEmitter simplifies callback management for AAA engine systems:
		*  - automatically unsubscribes permanent callbacks in the destructor,
		*  - supports one-time and permanent event subscriptions,
		*  - delegates event emission to the EventBus,
		*  - zero heap allocations.
		*
		*  @note All function calls should be made from the main thread or the EventBus owner context.
		*/
		class EventEmitter {

			using EmitterSlot = detail::EmitterSlot;
			using SubscribtionID = detail::SubscribtionID;

		public:

			/*! @brief Constructs an EventEmitter bound to a specific EventBus.
			*
			*  All subsequent emits and subscriptions will be routed through this EventBus.
			*
			*  @param bus Reference to the EventBus to use.
			*  @thread_safety Must be called from the main thread or EventBus owner context.
			*/
			explicit EventEmitter(EventBus& bus) : m_bus(bus) { mSubscriptions.reserve(limits::gMaxPermanentCallbacks); }
			~EventEmitter() { Destroy(); }

			/*! @brief Emits an event to all subscribers of the specified type.
			*
			*  Delegates the call to the underlying EventBus.
			*
			*  @tparam EventType Type of the event to emit.
			*  @param event Reference to the event instance to dispatch.
			*/
			template<detail::LumEvent EventType>
			void Emit(const EventType& event) {
				m_bus.Emit<EventType>(event);
			}

			/*! @brief Subscribes a one-time callback for the specified event type.
			*
			*  The callback will be invoked once and automatically removed by the EventBus.
			*
			*  @tparam EventType Type of event to subscribe to.
			*  @tparam Lambda Type of the callback function or lambda.
			*  @param lambda Function to call when the event is emitted.
			*/
			template<detail::LumEvent EventType, typename Lambda>
			void Subscribe(Lambda&& lambda) {
				m_bus.Subscribe<EventType>(std::forward<Lambda>(lambda));
			}

			/*! @brief Subscribes a permanent callback for the specified event type.
			*
			*  The callback remains active until manually unsubscribed or until the EventEmitter is destroyed.
			*  EventEmitter automatically handles unsubscribing in its destructor.
			*
			*  @tparam EventType Type of event to subscribe to.
			*  @tparam Lambda Type of the callback function or lambda.
			*  @param lambda Function to call when the event is emitted.
			*  @note Subscription will be ignored if MAX_PERM_CALLBACKS limit is reached.
			*/
			template<detail::LumEvent EventType, typename Lambda>
			void SubscribePermamently(Lambda&& lambda) {
				if (mSubscriptions.size() >= limits::gMaxPermanentCallbacks)
					return;

				auto id = m_bus.SubscribePermanently<EventType>(std::forward<Lambda>(lambda));
				mSubscriptions.push_back({ id, [](SubscribtionID id, EventBus& bus)
					{ bus.UnsubscribePermanent<EventType>(id); }
					});
			}

		private:

			void Destroy() {
				for (auto& slot : mSubscriptions) {
					slot.unsub(slot.id, m_bus);
				}
				LUM_LOG_DEBUG("Destroyed emitter and unsubscribed all callbacks");
			}

			EventBus& m_bus;

			std::vector<EmitterSlot> mSubscriptions;

		};
	}
}