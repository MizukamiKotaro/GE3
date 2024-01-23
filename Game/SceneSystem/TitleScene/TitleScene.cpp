#include "TitleScene.h"
#include "Kyoko.h"

TitleScene::TitleScene()
{
	FirstInit();

	titleSprite_ = std::make_unique<Sprite>("Resources/title.png");
}

void TitleScene::Init()
{

}

void TitleScene::Update()
{
#ifdef _DEBUG
	if (input_->PressedKey(DIK_RETURN)) {
		// シーン切り替え
		ChangeScene(SELECT);
	}
#endif // _DEBUG
}

void TitleScene::Draw()
{
	Kyoko::PreDraw();

	backgroundSprite_->Draw();

	titleSprite_->Draw();

	BlackDraw();

	Kyoko::PostDraw();
}

