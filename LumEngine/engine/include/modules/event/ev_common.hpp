#pragma once
#include "include/core/core_defines.hpp"
#include <vector>
#include <functional>
namespace ev {
	namespace detail {

		inline constexpr unsigned int MAX_EVENT_TYPES = settings::MAX_EVENT_TYPES;
		#define LumEventTag \
			constexpr static bool isEvent = true;

		template<typename T>
		concept EventT = requires { T::isEvent; };

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
			force_inline static CallbackID Get() {
				return Count()++;
			}
		private:
			CallbackIDGenerator() {}
			force_inline static CallbackID& Count() {
				static CallbackID globalID = 0;
				return globalID;
			}
		};

		struct EventTypeID {
			template<EventT T>
			force_inline static EventType_t Get() {
				static EventType_t typeID = Count()++;
				return typeID;
			}
		private:
			EventTypeID() {}
			force_inline static EventType_t& Count() {
				static EventType_t globalID = 0;
				return globalID;
			}

		};


	}
}