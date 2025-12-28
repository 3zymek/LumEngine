#pragma once
#include "core/core_pch.hpp"
#include "core/core_defines.hpp"
#include <functional>
namespace ev {
	namespace detail {

		using Event_t = uint32_t;

		inline constexpr Event_t MAX_EVENT_TYPES			= settings::MAX_EVENT_TYPES;
		inline constexpr Event_t MAX_EVENTS_PER_FRAME		= settings::MAX_EVENTS_PER_FRAME;
		inline constexpr Event_t MAX_CALLBACKS_PER_FRAME	= settings::MAX_CALLBACKS_PER_FRAME;
		inline constexpr Event_t MAX_PERM_CALLBACKS			= settings::MAX_PERM_CALLBACKS;

		#define LumEventTag \
			inline constexpr static bool __lumevent__ = true;

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
			inline static CallbackID Get() {
				return Count()++;
			}
		private:
			CallbackIDGenerator() {}
			inline static CallbackID& Count() {
				static CallbackID globalID = 0;
				return globalID;
			}
		};

		struct EventTypeID {
			template<EventT T>
			inline static EventType_t Get() {
				static EventType_t typeID = Count()++;
				return typeID;
			}
		private:
			EventTypeID() {}
			inline static EventType_t& Count() {
				static EventType_t globalID = 0;
				return globalID;
			}

		};


	}
}