#pragma once
#include "IEvent.hpp"
#include "config.hpp"
struct ObjectCreated	: IEvent 
{ 
	uint64_t id;
	ObjectCreated(uint64_t _id) : id(_id) { LOG_ENGINE("Event - object created: " << id) }
};
struct ObjectDestroyed	: IEvent 
{ 
	uint64_t id;
	ObjectDestroyed(uint64_t _id) : id(_id) { LOG_ENGINE("Event - object destroyed: " << id) }
};