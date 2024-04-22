#ifndef PHYSICS_H
#define PHYSICS_H

#include "StateDB.h"

struct Actor; // forward declaration

struct HitResult
{
	Actor* actor = nullptr; // the actor the raycast found
	b2Vec2 point = b2Vec2(0.0f, 0.0f); // the point at which the raycast struck a fixture of the actor
	b2Vec2 normal = b2Vec2(0.0f, 0.0f); // the normal vector at the point
	bool is_trigger = true; // whether or not the fixture encountered is a trigger (sensor)
};

class PhysicsRayCallback : public b2RayCastCallback
{
public:
	virtual float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;

	std::vector<HitResult> hit_vec;
};

class RaycastHandler
{
public:
	static inline PhysicsRayCallback callback = PhysicsRayCallback();
	static luabridge::LuaRef Raycast(const b2Vec2& pos, b2Vec2 dir, float dist);
	static luabridge::LuaRef RaycastAll(const b2Vec2& pos, b2Vec2 dir, float dist);
};

#endif