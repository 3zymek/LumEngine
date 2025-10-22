#pragma once
#include "IManager.hpp"
#include <unordered_map>
#include <optional>
#include <functional>
#include "config.hpp"

class EventManager;
class Object;

class ObjectManager : public IManager
{
public:

	ObjectManager(EventManager& _eventS) : m_EngineEventSystem(_eventS) { LOG_ENGINE("Object manager - initialized"); }

	std::optional<std::reference_wrapper<Object>> createObject();
	NODISCARD std::optional<std::reference_wrapper<Object>> getbyID(uint64_t _id) const;
	void deleteObject(Object&);
	void deleteObject(uint64_t);
	void update1All();
	void update2All();
	void trashObjects();

private:

	void push(std::shared_ptr<Object>);

	EventManager& m_EngineEventSystem;

	std::unordered_map<uint64_t, std::shared_ptr<Object>> objects;

};

