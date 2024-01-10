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
	if (transitionRequest_) {
		transition_ = transitionRequest_.value();

		switch (transition_)
		{
		case Transition::kFromBlack:
			FromBlackInitialize();
			break;
		case Transition::kOperation:
			break;
		case Transition::kToBlack:
			ToBlackInitialize();
			break;
		default:
			break;
		}

		transitionRequest_ = std::nullopt;
	}

	switch (transition_)
	{
	case Transition::kFromBlack:
		FromBlackUpdate();
		break;
	case Transition::kOperation:
		if (input_->PressedKey(DIK_SPACE)) {
			transitionRequest_ = Transition::kToBlack;
		}
		break;
	case Transition::kToBlack:
		ToBlackUpdate(STAGE);
		break;
	default:
		break;
	}
}

void SelectScene::Draw()
{
	Kyoko::PreDraw();

	

	BlackDraw();

	Kyoko::PostDraw();
}

