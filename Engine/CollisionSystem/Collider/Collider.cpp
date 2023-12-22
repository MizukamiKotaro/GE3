#include "Collider.h"

void Collider::ShapeCircle::SetInfo(const Vector2& pos, float radius)
{
	pos_ = pos;
	radius_ = radius;
}

void Collider::ShapeCircle::SetPos(const Vector2& pos)
{
	pos_ = pos;
}

void Collider::ShapeCircle::SetRadius(float radius)
{
	radius_ = radius;
}

void Collider::InitCircle(const Vector2& pos, float radius)
{
	shapeType_ = ShapeType::CIRCLE;

	circle_.pos_ = pos;
	circle_.radius_ = radius;
}

void Collider::OnCollision()
{
}

uint32_t Collider::GetCollisionAttribute() const
{
	return collisionAttribute_;
}

void Collider::SetCollisionAttribute(uint32_t attribute)
{
	collisionAttribute_ = collisionAttribute_ | attribute;
}

uint32_t Collider::GetCollisionMask() const
{
	return collisionMask_;
}

void Collider::SetCollisionMask(uint32_t mask)
{
	collisionMask_ = collisionMask_ | mask;
}

Collider::ShapeType Collider::GetShapeType() const
{
	return shapeType_;
}

Collider::ShapeCircle Collider::GetCircle_() const
{
	return circle_;
}
