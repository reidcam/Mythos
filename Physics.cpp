#include "Physics.h"

float PhysicsRayCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
{
	hit_vec.clear();
	uintptr_t userDataA = fixture->GetUserData().pointer;
	Actor* actor_a = reinterpret_cast<Actor*>(userDataA);

	if (actor_a == nullptr) // phantom fixture, ignore this fixture
		return -1.0f;

	HitResult result = { actor_a, point, normal, fixture->IsSensor() };
	hit_vec.push_back(result);

	return fraction;
}

luabridge::LuaRef RaycastHandler::Raycast(const b2Vec2& pos, b2Vec2 dir, float dist)
{
	const float EPSILON = 1e-6;
	if (std::fabs(dist) < EPSILON || dist < 0.0f) // dist is 0 or negative
		return luabridge::LuaRef(StateDB::lua_state); // nil

	dir.Normalize();
	b2Vec2 endpos = pos + (dist * dir);

	if (!StateDB::world_initialized) // no rigidbodies, return nil
		return luabridge::LuaRef(StateDB::lua_state); // nil

	StateDB::world->RayCast(&callback, pos, endpos);

	if (callback.hit_vec.size() == 0) // no hit, return nil
		return luabridge::LuaRef(StateDB::lua_state); // nil


	return luabridge::LuaRef(StateDB::lua_state, callback.hit_vec[0]);
}

luabridge::LuaRef RaycastHandler::RaycastAll(const b2Vec2& pos, b2Vec2 dir, float dist)
{
	const float EPSILON = 1e-6;
	if (std::fabs(dist) < EPSILON || dist < 0.0f) // dist is 0 or negative
		return luabridge::LuaRef(StateDB::lua_state); // nil

	dir.Normalize();
	b2Vec2 endpos = pos + (dist * dir);

	if (!StateDB::world_initialized) // no rigidbodies, return nil
		return luabridge::LuaRef(StateDB::lua_state); // nil

	StateDB::world->RayCast(&callback, pos, endpos);

	if (callback.hit_vec.size() == 0) // no hit, return nil
		return luabridge::LuaRef(StateDB::lua_state); // nil


	return luabridge::LuaRef(StateDB::lua_state, callback.hit_vec);
}
