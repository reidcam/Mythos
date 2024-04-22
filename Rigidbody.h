#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "glm/glm.hpp"
#include <string>
#include "Component.h"
#include "StateDB.h"

struct Actor; // forward declaration

class Rigidbody
{
public:
	b2Body* body;

	// Rigidbody parameters
	float x = 0.0f;
	float y = 0.0f;
	std::string body_type = "dynamic";
	bool precise = true;
	float gravity_scale = 1.0f;
	float density = 1.0f;
	float angular_friction = 0.3f;
	float rotation = 0.0f; // units of degrees (not radians), will need to convert to radians for box2D

	// collider/trigger properties
	bool has_collider = true;
	bool has_trigger = true;
	std::string collider_type = "box";
	std::string trigger_type = "box";
	float collider_width = 1.0f;
	float collider_height = 1.0f;
	float collider_radius = 0.5f;
	float trigger_width = 1.0f;
	float trigger_height = 1.0f;
	float trigger_radius = 0.5f;
	float friction = 0.3f;
	float bounciness = 0.3f;


	// component parameters
	std::string type = "Rigidbody";
	std::string key = "???";
	Actor* actor = nullptr;
	bool enabled = true;

	void OnStart();
	void OnDestroy();

	// Setter functions
	void AddForce(const b2Vec2& force);
	void SetVelocity(const b2Vec2& velocity);
	void SetPosition(const b2Vec2& position);
	void SetRotation(const float degrees_clockwise);
	void SetAngularVelocity(const float degrees_clockwise);
	void SetGravityScale(const float scale);
	void SetUpDirection(b2Vec2 direction);
	void SetRightDirection(b2Vec2 direction);

	// Getter functions
	b2Vec2 GetPosition();
	float GetRotation();
	b2Vec2 GetVelocity();
	float GetAngularVelocity();
	float GetGravityScale();
	b2Vec2 GetUpDirection();
	b2Vec2 GetRightDirection();
};

#endif