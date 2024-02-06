#pragma once
#include <memory>
#include <array>
#include <string>
#include "GlobalVariables/GlobalVariables.h"
#include "Model.h"
#include "Sprite.h"
#include "PostEffect/HighLumi/HighLumi.h"
#include "PostEffect/GaussianBlur/GaussianBlur.h"
#include "PostEffect/PostEffect.h"

class Camera;

class ClearPlayer {
public:

	ClearPlayer();

	void Initialize();

	void Update();

	void Draw(Camera* camera);

private:

	std::array<std::unique_ptr<Model>, 4> hoges_;

	float vel_;
};