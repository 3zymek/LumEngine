#pragma once
#include "Math.hpp"
#include "config.hpp"
#include "ObjectManager.hpp"
#include "EventManager.hpp"
#include "Object.hpp"

class ObjectAPI {
public:

	ObjectAPI(ObjectManager& _manager) : m_manager(_manager) {}

	FORCEINLINE std::optional<std::reference_wrapper<Object>> create() {
		return m_manager.createObject();
	}
	FORCEINLINE std::optional<std::reference_wrapper<Object>> getbyID(uint64_t _id) {
		return m_manager.getbyID(_id);
	}
	FORCEINLINE void destroy(uint64_t _id) {
		m_manager.deleteObject(_id);
	}
	FORCEINLINE void destroy(Object& _obj) {
		m_manager.deleteObject(_obj);
	}


private:

	ObjectManager& m_manager;

};


class Engine
{ 
private:

	using EM = EventManager;
	using OM = ObjectManager;

	MAYBE_UNUSED std::unique_ptr<EM>	m_GameEventSystem = std::make_unique<EM>();
	std::unique_ptr<EM>					m_EngineEventSystem = std::make_unique<EM>();
	std::unique_ptr<OM>					m_objManager = std::make_unique<OM>(*m_EngineEventSystem);

public:

	Engine() { LOG_ENGINE("Engine - initialized") }

	ObjectAPI object{ *m_objManager };

	void init();
	void run();

#ifdef DEBUG_ENGINE
	NODISCARD EM&	getGameEventManager();
	NODISCARD OM&	getObjectManager();
#endif // DEBUG

};

