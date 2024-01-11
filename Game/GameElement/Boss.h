#pragma once
#include "Stage.h"

class Boss {
public:
	Boss() = default;
	~Boss() = default;

	void Init();

	void Update(const float deltaTime);

	void Draw();

	void SetStage(Stage *stage) { pStage_ = stage; }

private:

	Stage *pStage_ = nullptr;

};