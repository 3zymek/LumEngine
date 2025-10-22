#include "ObjectManager.hpp"
#include "Object.hpp"
#include "EngineEvents.hpp"
#include "EventManager.hpp"
std::optional<std::reference_wrapper<Object>> ObjectManager::createObject() {
	auto obj = std::make_shared<Object>();
	uint64_t id = obj->id;
	push(std::move(obj));
	m_EngineEventSystem.emit(ObjectCreated{ id });
	return std::ref(*objects[id]);
}
void ObjectManager::push(std::shared_ptr<Object> _obj) {
	objects.emplace(_obj->id, std::move(_obj));
}
void ObjectManager::update1All() {
	if (objects.size()) {
		for (auto it = objects.begin(); it != objects.end(); it++) {
			it->second->update1();
		}
	}
}
void ObjectManager::update2All() {
	if (objects.size()) {
		for (auto it = objects.begin(); it != objects.end(); it++) {
			it->second->update2();
		}
	}
}
void ObjectManager::trashObjects() {
	if (objects.size()) {
		for (auto it = objects.begin(); it != objects.end();) {
			if (it->second->toDelete) {
				m_EngineEventSystem.emit(ObjectDestroyed{ it->first });
				it = objects.erase(it);
			}
			else it++;
		}
	}
}
std::optional<std::reference_wrapper<Object>> ObjectManager::getbyID(uint64_t _id) const {
	auto it = objects.find(_id);
	if (it == objects.end()) return std::nullopt;
	return std::ref(*it->second);
}
void ObjectManager::deleteObject(Object& _obj) {
	_obj.toDelete = true;
}
void ObjectManager::deleteObject(uint64_t _id) {
	if (auto obj = getbyID(_id)) {
		obj->get().toDelete = true;
	}
}