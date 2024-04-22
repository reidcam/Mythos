#ifndef STATEDB_H
#define STATEDB_H

#include "ThirdParty/lua-5.4.6/lua.hpp"
#include "ThirdParty/LuaBridge/LuaBridge.h"
#include "box2d/box2d.h"


class StateDB
{
public:
	inline static lua_State* lua_state;
	inline static int added_component_count = 0; // counts the number of times that Actor::AddComponent has been called

	// used for world physics
	inline static bool world_initialized = false;
	inline static b2World* world;


	inline static void InitializeWorld() {
		if (!StateDB::world_initialized) { // initialize physics world
			StateDB::world = new b2World(b2Vec2(0.0f, 9.8f));
			StateDB::world_initialized = true;
		}
	};
	inline static void AdvanceWorld() {
		if (world_initialized)
			StateDB::world->Step(1.0f / 60.0f, 8, 3);
	};
};
#endif