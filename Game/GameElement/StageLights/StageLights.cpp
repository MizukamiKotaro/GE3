#include "StageLights.h"
#include "Camera.h"
#include "TextureManager.h"
#include "Externals/imgui/imgui.h"
#include "RandomGenerator/RandomGenerator.h"
#include <numbers>
#include "RandomGenerator/RandomGenerator.h"
#include "Input.h"
#include <algorithm>
#include <numbers>

StageLights::StageLights()
{
	globalVariable_ = GlobalVariables::GetInstance();

	SetGlobalVariable();

	for (int i = 0; i < heightNum_; i++) {
		for (int j = 0; j < sideNum_; j++) {

			heightLights_[i][j] = std::make_unique<PointLight>();

			if (i < 2 && j < sideNum_ - 2) {
				topDownLights_[i][j] = std::make_unique<PointLight>();
			}
		}


	}

}

void StageLights::Initialize()
{
}

void StageLights::Update()
{
}

void StageLights::Draw(const Camera* camera)
{
}

void StageLights::SetGlobalVariable()
{
	fInfo_[EnumFInfo::Radius] = 1.0f;
	v2Info_[EnumV2Info::Space] = { 1.0f,2.0f };

	globalVariable_->CreateGroup(groupName_);

	for (int i = 0; i < EnumFInfo::FEndCount; i++) {
		globalVariable_->AddItem(groupName_, fItemNames[i], fInfo_[i]);
	}

	for (int i = 0; i < EnumV2Info::V2EndCount; i++) {
		globalVariable_->AddItem(groupName_, v2ItemNames[i], v2Info_[i]);
	}


	ApplyGlobalVariable();
}

void StageLights::ApplyGlobalVariable()
{
	for (int i = 0; i < EnumFInfo::FEndCount; i++) {
		fInfo_[i] = globalVariable_->GetFloatValue(groupName_, fItemNames[i]);
	}
	
	for (int i = 0; i < EnumV2Info::V2EndCount; i++) {
		v2Info_[i] = globalVariable_->GetVector2Value(groupName_, v2ItemNames[i]);
	}

	for (int i = 0; i < heightNum_; i++) {
		for (int j = 0; j < sideNum_; j++) {

			heightLights_[i][j]->light_->radius = fInfo_[EnumFInfo::Radius];
			heightLights_[i][j]->light_->position.z = fInfo_[EnumFInfo::PosZ];

			if (i < 2 && j < sideNum_ - 2) {
				topDownLights_[i][j]->light_->radius = fInfo_[EnumFInfo::Radius];
				topDownLights_[i][j]->light_->position.z = fInfo_[EnumFInfo::PosZ];
			}
		}


	}

	/*plane_->transform_.scale_.x = v2Info_[EnumV2Info::Scale].x;
	plane_->transform_.scale_.y = v2Info_[EnumV2Info::Scale].y;

	plane_->transform_.translate_ = v3Info_[EnumV3Info::Pos];*/
}
