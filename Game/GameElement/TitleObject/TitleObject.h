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

class TitleObject
{
public:
	TitleObject();

	void Initialize();

	void Update(Camera* camera);

	void Draw(Camera* camera);

public:

	void WrightPostEffect(Camera* camera);

private:

	/*GlobalVariables* globalVariable_;

	std::string groupName_;

	enum EnumV2Info {
		Size,
		Interval,
		Position,
		Offset,
		V2EndCount,
	};

	Vector2 v2Info_[V2EndCount];

	std::string v2ItemNames[V2EndCount] = {
		"サイズ",
		"フレームの空白の幅",
		"固定の場合のポジション",
		"親子付けした場合のローカルのポジション"
	};*/
private:

	std::unique_ptr<Model> titleObj_;

	std::unique_ptr<HighLumi> highLumi_;

	std::unique_ptr<GaussianBlur> gaussian_;

	std::unique_ptr<PostEffect> post_;

	std::array<std::unique_ptr<Sprite>, 10> sprites_;
};
