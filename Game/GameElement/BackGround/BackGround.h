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
#include "PostEffect/Blur/Blur.h"

class Camera;

class BackGround {
public:
	BackGround();
	void Initialize();
	void Update();
	void Draw();

	void cDraw(Camera* camera);

private:
	std::unique_ptr<Sprite> back_;
	//std::unique_ptr<GaussianBlur> gaus_;

	std::unique_ptr<Blur> blur0_;
	std::unique_ptr<Blur> blur1_;
	std::unique_ptr<PostEffect> post_;

	std::unique_ptr<Model> model_;

	std::unique_ptr<Sprite> rainbow_;
	float texcoodY_;

	std::unique_ptr<Sprite> ho_;
	std::unique_ptr<PostEffect> af_;

	std::unique_ptr<HighLumi> high_;
};