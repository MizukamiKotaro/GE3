#include "ClearScene.h"
#include "Kyoko.h"

ClearScene::ClearScene()
{
	FirstInit();

	clearSprite_ = std::make_unique<Sprite>("Resources/result.png");
}

void ClearScene::Init()
{

}

void ClearScene::Update()
{
#ifdef _DEBUG
	if (input_->PressedKey(DIK_SPACE)) {
		// シーン切り替え
		ChangeScene(SELECT);
	}
#endif // _DEBUG
}

void ClearScene::Draw()
{
	Kyoko::PreDraw();

	backgroundSprite_->Draw();

	clearSprite_->Draw();

	BlackDraw();

	Kyoko::PostDraw();
}


