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

class Decoration {
public:

	Decoration();

	void Initialize();

	void Update(Camera* camera);

	void Draw(Camera* camera);

public:

	void WrightPostEffect(Camera* camera);

	void SetIsSword(const bool* isSword) { isSword_ = isSword; }

private:

private:

	const bool* isSword_ = nullptr;
	const bool* isNeedle_ = nullptr;
	const bool* isPunch_ = nullptr;

	std::unique_ptr<Model> swordDecration_;

	std::unique_ptr<HighLumi> highLumi_;

	std::unique_ptr<GaussianBlur> gaussian_;

	std::unique_ptr<PostEffect> post_;

	std::unique_ptr<Sprite> rainbow_;
	float texcoodY_;

};