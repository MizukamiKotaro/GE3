#include "ClearScene.h"
#include "Kyoko.h"

ClearScene::ClearScene()
{
	FirstInit();


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



	BlackDraw();

	Kyoko::PostDraw();
}


