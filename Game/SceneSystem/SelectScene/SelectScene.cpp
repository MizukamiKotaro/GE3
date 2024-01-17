#include "SelectScene.h"
#include "Kyoko.h"

SelectScene::SelectScene()
{
	FirstInit();


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

#endif // _DEBUG

}

void SelectScene::Draw()
{
	Kyoko::PreDraw();

	

	BlackDraw();

	Kyoko::PostDraw();
}

