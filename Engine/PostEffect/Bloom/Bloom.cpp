#include "Bloom.h"

Bloom::Bloom()
{
	postEffect_ = std::make_unique<PostEffect>();
	postEffect_->Initialize();

	highLumi_ = std::make_unique<HighLumi>();
	highLumi_->Initialize();

	gaussianBlur_ = std::make_unique<GaussianBlur>();
	gaussianBlur_->Initialize();
}

Bloom::~Bloom()
{
	
}

void Bloom::Initialize()
{

}

void Bloom::Update()
{
	
}

void Bloom::Draw(BlendMode blendMode)
{
	postEffect_->Draw();
	gaussianBlur_->Draw(BlendMode::kBlendModeAdd);
}

void Bloom::PreDrawScene()
{
	postEffect_->PreDrawScene();
}

void Bloom::PostDrawScene()
{
	postEffect_->PostDrawScene();

	highLumi_->PreDrawScene();

	postEffect_->Draw();

	highLumi_->PostDrawScene();

	gaussianBlur_->PreDrawScene();

	highLumi_->Draw();

	gaussianBlur_->PostDrawScene();
}
