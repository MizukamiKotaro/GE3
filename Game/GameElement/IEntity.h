#pragma once

class Stage;

class IEntity {
public:
	virtual void Init() = 0;

	virtual void Update(const float deltaTime) = 0;

	virtual void Draw() = 0;

	virtual void OnCollision(IEntity *other) { other; }

	static void SetStage(Stage *stage) { pStage_ = stage; }

	static Stage *GetStage() { return pStage_; }

protected:
	static Stage *pStage_;
};