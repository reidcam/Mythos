#include "CollisionDetector.h"

void CollisionDetector::BeginContact(b2Contact* contact)
{
	b2Fixture* fixture_a = contact->GetFixtureA();
	b2Fixture* fixture_b = contact->GetFixtureB();
	uintptr_t userDataA = fixture_a->GetUserData().pointer;
	Actor* actor_a = reinterpret_cast<Actor*>(userDataA);
	uintptr_t userDataB = fixture_b->GetUserData().pointer;
	Actor* actor_b = reinterpret_cast<Actor*>(userDataB);

	// one of the colliders is a phantom fixture
	if (actor_a == nullptr || actor_b == nullptr)
		return;

	b2WorldManifold	world_manifold;
	contact->GetWorldManifold(&world_manifold);

	Collision collision;
	collision.point = world_manifold.points[0]; // get the first point of collision

	b2Vec2 relative_velocity = fixture_a->GetBody()->GetLinearVelocity() - fixture_b->GetBody()->GetLinearVelocity();
	collision.relative_velocity = relative_velocity;

	collision.normal = world_manifold.normal;

	collision.other = actor_b;

	// determine if we have 2 triggers or 2 colliders
	if (fixture_a->IsSensor() && fixture_b->IsSensor()) {
		collision.point = b2Vec2(-999.0f, -999.0f); // not valid for trigger
		collision.normal = b2Vec2(-999.0f, -999.0f); // not valid for trigger
		actor_a->OnTriggerEnter(collision);

		collision.other = actor_a;

		actor_b->OnTriggerEnter(collision);
	}
	else if (!fixture_a->IsSensor() && !fixture_b->IsSensor()) {
		actor_a->OnCollisionEnter(collision);

		collision.other = actor_a;

		actor_b->OnCollisionEnter(collision);
	}
}

void CollisionDetector::EndContact(b2Contact* contact)
{
	b2Fixture* fixture_a = contact->GetFixtureA();
	b2Fixture* fixture_b = contact->GetFixtureB();
	uintptr_t userDataA = fixture_a->GetUserData().pointer;
	Actor* actor_a = reinterpret_cast<Actor*>(userDataA);
	uintptr_t userDataB = fixture_b->GetUserData().pointer;
	Actor* actor_b = reinterpret_cast<Actor*>(userDataB);

	// one of the colliders is a phantom fixture
	if (actor_a == nullptr || actor_b == nullptr)
		return;

	Collision collision;
	collision.point = b2Vec2(-999.0f, -999.0f); // not valid for exit

	b2Vec2 relative_velocity = fixture_a->GetBody()->GetLinearVelocity() - fixture_b->GetBody()->GetLinearVelocity();
	collision.relative_velocity = relative_velocity;

	collision.normal = b2Vec2(-999.0f, -999.0f); // not valid for exit

	collision.other = actor_b;

	// determine if we have 2 triggers or 2 colliders
	if (fixture_a->IsSensor() && fixture_b->IsSensor()) {
		actor_a->OnTriggerExit(collision);

		collision.other = actor_a;

		actor_b->OnTriggerExit(collision);
	}
	else if (!fixture_a->IsSensor() && !fixture_b->IsSensor()) {
		actor_a->OnCollisionExit(collision);

		collision.other = actor_a;

		actor_b->OnCollisionExit(collision);
	}
}