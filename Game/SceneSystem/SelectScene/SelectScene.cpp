#include "SelectScene.h"
#include "Kyoko.h"

SelectScene::SelectScene()
{
	FirstInit();

	selectSprite_ = std::make_unique<Sprite>("Resources/select.png");
}

void SelectScene::Init()
{

}

void SelectScene::Update()
{
#ifdef _DEBUG

	if (input_->PressedKey(DIK_SPACE)) {
		// シーン切り替え
		stageNo_ = 0;
		ChangeScene(STAGE);
	}
	if (input_->PressedKey(DIK_ESCAPE)) {
		// シーン切り替え
		stageNo_ = 0;
		ChangeScene(TITLE);
	}

#endif // _DEBUG

}

void SelectScene::Draw()
{
	Kyoko::PreDraw();

	backgroundSprite_->Draw();

	selectSprite_->Draw();

	BlackDraw();

	Kyoko::PostDraw();
}

