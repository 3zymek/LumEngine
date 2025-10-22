#pragma once
#include "Math.hpp"
#include <iostream>
using vec2f = Math::Vector2f;

class Object
{
public:

	uint64_t id;
	bool toDelete = false;
	Object() {
		static uint64_t globalID = 0;
		id = ++globalID;
	}
	~Object() {}

	//update1: personalna edycja (pozycja rotacja itp.)
	//update2 : zastosowanie tego w œwiecie(pozycja, kolizje, interakcje)

	void update1();
	void update2();

private:

};

