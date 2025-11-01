#pragma once
#include "ev_define.hpp"
namespace ev {
	namespace detail {
		struct BasePool {
		public:
			virtual			~BasePool() noexcept = default;
			virtual void	Process() = 0;
		};

		template<EventT T>
		struct EventPool : public BasePool {

			EventPool() {
				m_callbacks.reserve(128);
				m_permament_callbacks.reserve(256);
			}

			FORCEINLINE void PushEvent(const T& event) {
				m_events.push_back(std::move(event));
			}
			FORCEINLINE void SubscribeCallback(const CallbackWrapper<T>& callback) {
				m_callbacks.push_back(std::move(callback));
			}
			FORCEINLINE void SubscribePermamently(const CallbackWrapper<T>& callback) {
				m_permament_callbacks.push_back(std::move(callback));
			}
			FORCEINLINE void Unsubscribe(const CallbackID& id) {
				m_callbacks.erase(std::remove_if(m_callbacks.begin(), m_callbacks.end(), [id](const CallbackWrapper<T>& call) {
					return call.GetID() == id;
					}), m_callbacks.end());
			}
			FORCEINLINE void Process() override {
				for (auto& ev : m_events) {
					for (auto& call : m_callbacks) {
						auto& callback = call.GetCallback();
						callback(ev);
					}
					for (auto& call : m_permament_callbacks) {
						auto& callback = call.GetCallback();
						callback(ev);
					}
				}
				m_events.clear();
				m_callbacks.clear();
			}
		private:

			std::vector<CallbackWrapper<T>> m_callbacks;
			std::vector<CallbackWrapper<T>> m_permament_callbacks;
			std::vector<T>					m_events;

		};
	}
}
