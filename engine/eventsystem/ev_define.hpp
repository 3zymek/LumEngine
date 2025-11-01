#pragma once
#include "e_define.hpp"
#include "ev_base.hpp"
#include <vector>
#include <functional>

namespace ev {
	namespace detail {

		constexpr int MAX_EVENT_TYPES = 20;

		template<typename T>
		concept EventT = std::is_base_of_v<BaseEvent, T> and !std::is_same_v<BaseEvent, T>;

		template<EventT T>
		using Callback			= std::function<void(const T&)>;
		using DestructFunction	= std::function<void()>;
		using CallbackID		= uint64_t;
		using EventType_t		= uint32_t;

		template<EventT T>
		struct CallbackWrapper {
			explicit CallbackWrapper(Callback<T> call) : callback(call) {}
			CallbackWrapper() = default;

			Callback<T>& GetCallback() {
				return callback;
			}
			CallbackID GetID() const {
				return callID;
			}

		private:

			Callback<T> callback;
			CallbackID	callID = CallbackIDGenerator<T>::Get();
		};

		template<EventT T>
		struct CallbackIDGenerator {
			FORCEINLINE static CallbackID Get() {
				return Count()++;
			}
		private:
			CallbackIDGenerator() {}
			FORCEINLINE static CallbackID& Count() {
				static CallbackID globalID = 0;
				return globalID;
			}
		};

		struct EventTypeID {
			template<EventT T>
			FORCEINLINE static EventType_t Get() {
				static EventType_t typeID = Count()++;
				return typeID;
			}
		private:
			EventTypeID() {}
			FORCEINLINE static EventType_t& Count() {
				static EventType_t globalID = 0;
				return globalID;
			}

		};

		#define EV_ASSERT_IS_EVENT(T) \
			static_assert(std::is_base_of_v<BaseEvent, T> and !std::is_same_v<BaseEvent, T>, "Event must be derived from BaseEvent")

		#ifdef DEBUG_EVENT
			#define EVENT_LOG(x) \
				do { std::cout << "[EVENT] " << x << '\n'; } while(0);
		#else
			#define EVENT_LOG(x)
		#endif // DEBUG_ENGINE


	}
}