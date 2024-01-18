#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>
#include <string>
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vector4.h"
#include "Utils/Math/Matrix4x4.h"
#include "GraphicsPipelines/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "PostEffect/PostEffect.h"
#include "PostEffect/HighLumi/HighLumi.h"
#include "PostEffect/GaussianBlur/GaussianBlur.h"
#include <memory>

// スプライト
class Bloom
{
public:

	Bloom();

	~Bloom();

	void Initialize();

	void Update();

	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal);


	void PreDrawScene();

	void PostDrawScene();

private:

	std::unique_ptr<PostEffect> postEffect_;
	std::unique_ptr<HighLumi> highLumi_;
	std::unique_ptr<GaussianBlur> gaussianBlur_;

};

