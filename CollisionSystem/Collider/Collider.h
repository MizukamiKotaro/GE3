#pragma once
#include "Vector2.h"
#include <stdint.h>

class Collider
{
public:
	enum class ShapeType
	{
		CIRCLE
	};

	struct ShapeCircle {
		Vector2 pos_;
		float radius_;

		void SetInfo(const Vector2& pos, float radius);
		void SetPos(const Vector2& pos);
		void SetRadius(float radius);
	};

	void InitCircle(const Vector2& pos, float radius);

	virtual void OnCollision();

	uint32_t GetCollisionAttribute() const;

	void SetCollisionAttribute(uint32_t attribute);

	uint32_t GetCollisionMask() const;

	void SetCollisionMask(uint32_t mask);

	ShapeType GetShapeType() const;

	ShapeCircle GetCircle_() const;

protected:

	ShapeType shapeType_;

	ShapeCircle circle_;

private:

	uint32_t collisionAttribute_ = 0x00000000;

	uint32_t collisionMask_ = 0x00000000;

};

