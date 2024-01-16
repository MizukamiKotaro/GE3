#pragma once

class IEntity {
public:
	virtual void Init() = 0;

	virtual void Update(const float deltaTime) = 0;

	virtual void Draw() = 0;

	virtual void OnCollision(IEntity *other) { other; }
};