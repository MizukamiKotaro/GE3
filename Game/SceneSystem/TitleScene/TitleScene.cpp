#include "TitleScene.h"
#include "Kyoko.h"

TitleScene::TitleScene()
{
	FirstInit();


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



	BlackDraw();

	Kyoko::PostDraw();
}

