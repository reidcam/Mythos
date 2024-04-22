#include "Rigidbody.h"

void Rigidbody::OnStart()
{
	StateDB::InitializeWorld(); // initialize physics world

	b2BodyDef body_def;
	if (body_type == "dynamic")
		body_def.type = b2_dynamicBody;
	else if (body_type == "kinematic")
		body_def.type = b2_kinematicBody;
	else if (body_type == "static")
		body_def.type = b2_staticBody;

	body_def.position = b2Vec2(x, y);
	body_def.bullet = precise;
	body_def.angularDamping = angular_friction;
	body_def.gravityScale = gravity_scale;

	body = StateDB::world->CreateBody(&body_def);


	// set the shape
	b2Shape* collider_shape = nullptr;
	b2Shape* trigger_shape = nullptr;
	if (collider_type == "box") {
		b2PolygonShape* polygon_shape = new b2PolygonShape();
		polygon_shape->SetAsBox(collider_width * 0.5f, collider_height * 0.5f);
		collider_shape = polygon_shape;
	}
	else if (collider_type == "circle") {
		b2CircleShape* circle_shape = new b2CircleShape();
		circle_shape->m_radius = collider_radius;
		collider_shape = circle_shape;
	}

	if (trigger_type == "box") {
		b2PolygonShape* polygon_shape = new b2PolygonShape();
		polygon_shape->SetAsBox(trigger_width * 0.5f, trigger_height * 0.5f);
		trigger_shape = polygon_shape;
	}
	else if (trigger_type == "circle") {
		b2CircleShape* circle_shape = new b2CircleShape();
		circle_shape->m_radius = trigger_radius;
		trigger_shape = circle_shape;
	}

	// define the fixture
	b2FixtureDef fixure_def;
	fixure_def.shape = collider_shape;
	fixure_def.density = density;
	fixure_def.restitution = bounciness;
	fixure_def.friction = friction;


	// create the fixtures
	if (!has_collider && !has_trigger) {
		fixure_def.userData.pointer = reinterpret_cast<uintptr_t>(nullptr);
		fixure_def.isSensor = true; // phantom is a fake trigger
		body->CreateFixture(&fixure_def);
	}
	if (has_collider) {
		fixure_def.userData.pointer = reinterpret_cast<uintptr_t>(actor);
		fixure_def.isSensor = false; // false bc collider
		body->CreateFixture(&fixure_def);
	}
	if (has_trigger) {
		fixure_def.userData.pointer = reinterpret_cast<uintptr_t>(actor);
		fixure_def.shape = trigger_shape;
		fixure_def.isSensor = true; // true bc trigger
		body->CreateFixture(&fixure_def);
	}

	// must be called after the body is created
	SetRotation(rotation);
}

void Rigidbody::OnDestroy()
{
	StateDB::world->DestroyBody(body);
}

void Rigidbody::AddForce(const b2Vec2& force)
{
	if (body == nullptr)
		return;

	body->ApplyForceToCenter(force, true);
}

void Rigidbody::SetVelocity(const b2Vec2& velocity)
{
	if (body == nullptr)
		return;

	body->SetLinearVelocity(velocity);
}

void Rigidbody::SetPosition(const b2Vec2& position)
{
	if (body == nullptr) { // OnStart hasn't been called yet
		x = position.x;
		y = position.y;
	}
	else {
		float current_rotation = body->GetAngle();
		body->SetTransform(position, current_rotation);
	}
}

void Rigidbody::SetRotation(const float degrees_clockwise)
{
	if (body == nullptr)
		rotation = degrees_clockwise;
	else {
		// convert to radians and apply
		body->SetTransform(body->GetPosition(), static_cast<int>(degrees_clockwise) * (b2_pi / 180.0f));
	}
}

void Rigidbody::SetAngularVelocity(const float degrees_clockwise)
{
	if (body == nullptr) // no valid parameter to set before body is initialized
		return;

	// convert to radians
	body->SetAngularVelocity(degrees_clockwise * (b2_pi / 180.0f));
}

void Rigidbody::SetGravityScale(const float scale)
{
	if (body == nullptr)
		gravity_scale = scale;
	else
		body->SetGravityScale(scale);
}

void Rigidbody::SetUpDirection(b2Vec2 direction)
{
	direction.Normalize();

	if (body == nullptr) {
		// get radians and then convert to degrees to store
		rotation = glm::atan(direction.x, -direction.y) * (180.0f / b2_pi);
	}
	else {
		float new_angle_radians = glm::atan(direction.x, -direction.y);
		body->SetTransform(body->GetPosition(), new_angle_radians);
	}
}

void Rigidbody::SetRightDirection(b2Vec2 direction)
{
	direction.Normalize();

	if (body == nullptr) {
		// get radians and then convert to degrees to store
		rotation = (glm::atan(direction.x, -direction.y) - (b2_pi / 2.0f)) * (180.0f / b2_pi);
	}
	else {
		float new_angle_radians = glm::atan(direction.x, -direction.y) - (b2_pi / 2.0f);
		body->SetTransform(body->GetPosition(), new_angle_radians);
	}
}

b2Vec2 Rigidbody::GetPosition()
{
	if (body == nullptr) // OnStart hasn't been called yet
		return b2Vec2(x, y);

	return body->GetPosition();
}

float Rigidbody::GetRotation()
{
	if (body == nullptr) // OnStart hasn't been called yet
		return rotation;

	// body->GetAngle() returns the value in radians, need to convert to degrees
	return body->GetAngle() * (180.0f / b2_pi);
}

b2Vec2 Rigidbody::GetVelocity()
{
	if (body == nullptr) // OnStart hasn't been called yet
		return b2Vec2(0.0f, 0.0f); // no velocity before init

	return body->GetLinearVelocity();
}

float Rigidbody::GetAngularVelocity()
{
	if (body == nullptr)
		return 0.0f; // no velocity before init

	return body->GetAngularVelocity() * (180.0f / b2_pi);
}

float Rigidbody::GetGravityScale()
{
	if (body == nullptr) // OnStart hasn't been called yet
		return gravity_scale;

	return body->GetGravityScale();
}

b2Vec2 Rigidbody::GetUpDirection()
{
	b2Vec2 upDirection = b2Vec2(0.0f, 0.0f);

	if (body == nullptr) {
		float rotation_rad = rotation * (b2_pi / 180.0f);
		upDirection = b2Vec2(glm::sin(rotation_rad), -glm::cos(rotation_rad));
	}
	else
		upDirection = b2Vec2(glm::sin(body->GetAngle()), -glm::cos(body->GetAngle()));

	upDirection.Normalize();

	return upDirection;
}

b2Vec2 Rigidbody::GetRightDirection()
{
	b2Vec2 rightDirection = b2Vec2(0.0f, 0.0f);

	if (body == nullptr) {
		float rotation_rad = rotation * (b2_pi / 180.0f);
		rightDirection = b2Vec2(glm::cos(rotation_rad), glm::sin(rotation_rad));
	}
	else
		rightDirection = b2Vec2(glm::cos(body->GetAngle()), glm::sin(body->GetAngle()));

	rightDirection.Normalize();

	return rightDirection;
}
