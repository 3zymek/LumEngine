#pragma once
#include <iostream>
#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include "config.hpp"
#include "IEvent.hpp"
#include "IManager.hpp"

class EventManager : public IManager
{
public:

	EventManager() { LOG_ENGINE("Event manager - initialized") }

	template<class T>
	FORCEINLINE void subscribe(std::function<void(const T&)> callback) {
		auto wrapper = [callback](const IEvent& e) { callback(static_cast<const T&>(e)); };
		callbacks[typeid(T)].push_back(std::move(wrapper));
	}
	FORCEINLINE void emit(const IEvent& _event) {
		auto it = callbacks.find(typeid(_event));
		if (it != callbacks.end()) {
			for (const auto& callback : it->second) {
				callback(_event);
			}
		}
	}

private:

	std::unordered_map<std::type_index, std::vector<std::function<void(const IEvent&)>>> callbacks;

};

