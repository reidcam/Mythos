#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include "box2d/box2d.h"
#include "Actor.h"
#include "Collision.h"

class CollisionDetector : public b2ContactListener
{
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};
#endif