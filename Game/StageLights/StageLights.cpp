#include "StageLights.h"
#include "Camera.h"
#include "TextureManager.h"
#include "Externals/imgui/imgui.h"

StageLights::StageLights()
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 2; j++) {
			spotLights_[i][j] = std::make_unique<SpotLight>();
			spotLights_[i][j]->light_->position = { 2.0f,-1.25f,-10.0f + 5.0f * i };
			spotLights_[i][j]->light_->direction = { 1.0f,1.0f,0.0f };
			spotLights_[i][j]->light_->intensity = 4.0f;
			spotLights_[i][j]->light_->decay = 2.0f;

			if (j != 0) {
				spotLights_[i][j]->light_->position.x *= -1;
				spotLights_[i][j]->light_->direction.x *= -1;
			}

			spotLights_[i][j]->Update();
		}
	}
}

void StageLights::Initialize() {
	
}

void StageLights::Update(float time) {
#ifdef _DEBUG
	ImGui::Begin("SpotLights");
	if (ImGui::TreeNode("0")) {
		ImGui::SliderFloat3("ポジション0", &spotLights_[0][0]->light_->position.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("ライトの角度0", &spotLights_[0][0]->light_->direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat("輝度0", &spotLights_[0][0]->light_->intensity, 0.0f, 100.0f);
		ImGui::SliderFloat("距離0", &spotLights_[0][0]->light_->distance, 0.0f, 100.0f);
		ImGui::SliderFloat("減衰率0", &spotLights_[0][0]->light_->decay, 0.0f, 100.0f);
		ImGui::SliderFloat("開く角度0", &spotLights_[0][0]->light_->cosAngle, -1.0f, 1.0f);
		ImGui::SliderFloat("減衰し始める角度0", &spotLights_[0][0]->light_->cosFalloffStart, -1.0f, 1.0f);
		ImGui::ColorEdit4("色", &spotLights_[0][0]->light_->color.x);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("1")) {
		ImGui::SliderFloat3("ポジション1", &spotLights_[1][0]->light_->position.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("ライトの角度1", &spotLights_[1][0]->light_->direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat("輝度1", &spotLights_[1][0]->light_->intensity, 0.0f, 100.0f);
		ImGui::SliderFloat("距離1", &spotLights_[1][0]->light_->distance, 0.0f, 100.0f);
		ImGui::SliderFloat("減衰率1", &spotLights_[1][0]->light_->decay, 0.0f, 100.0f);
		ImGui::SliderFloat("開く角度1", &spotLights_[1][0]->light_->cosAngle, -1.0f, 1.0f);
		ImGui::SliderFloat("減衰し始める角度1", &spotLights_[1][0]->light_->cosFalloffStart, -1.0f, 1.0f);
		ImGui::ColorEdit4("色", &spotLights_[1][0]->light_->color.x);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("2")) {
		ImGui::SliderFloat3("ポジション2", &spotLights_[2][0]->light_->position.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("ライトの角度2", &spotLights_[2][0]->light_->direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat("輝度2", &spotLights_[2][0]->light_->intensity, 0.0f, 100.0f);
		ImGui::SliderFloat("距離2", &spotLights_[2][0]->light_->distance, 0.0f, 100.0f);
		ImGui::SliderFloat("減衰率2", &spotLights_[2][0]->light_->decay, 0.0f, 100.0f);
		ImGui::SliderFloat("開く角度2", &spotLights_[2][0]->light_->cosAngle, -1.0f, 1.0f);
		ImGui::SliderFloat("減衰し始める角度2", &spotLights_[2][0]->light_->cosFalloffStart, -1.0f, 1.0f);
		ImGui::ColorEdit4("色", &spotLights_[2][0]->light_->color.x);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("3")) {
		ImGui::SliderFloat3("ポジション3", &spotLights_[3][0]->light_->position.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("ライトの角度3", &spotLights_[3][0]->light_->direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat("輝度3", &spotLights_[3][0]->light_->intensity, 0.0f, 100.0f);
		ImGui::SliderFloat("距離3", &spotLights_[3][0]->light_->distance, 0.0f, 100.0f);
		ImGui::SliderFloat("減衰率3", &spotLights_[3][0]->light_->decay, 0.0f, 100.0f);
		ImGui::SliderFloat("開く角度3", &spotLights_[3][0]->light_->cosAngle, -1.0f, 1.0f);
		ImGui::SliderFloat("減衰し始める角度3", &spotLights_[3][0]->light_->cosFalloffStart, -1.0f, 1.0f);
		ImGui::ColorEdit4("色", &spotLights_[3][0]->light_->color.x);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("4")) {
		ImGui::SliderFloat3("ポジション4", &spotLights_[4][0]->light_->position.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("ライトの角度4", &spotLights_[4][0]->light_->direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat("輝度4", &spotLights_[4][0]->light_->intensity, 0.0f, 100.0f);
		ImGui::SliderFloat("距離4", &spotLights_[4][0]->light_->distance, 0.0f, 100.0f);
		ImGui::SliderFloat("減衰率4", &spotLights_[4][0]->light_->decay, 0.0f, 100.0f);
		ImGui::SliderFloat("開く角度4", &spotLights_[4][0]->light_->cosAngle, -1.0f, 1.0f);
		ImGui::SliderFloat("減衰し始める角度4", &spotLights_[4][0]->light_->cosFalloffStart, -1.0f, 1.0f);
		ImGui::ColorEdit4("色", &spotLights_[4][0]->light_->color.x);
		ImGui::TreePop();
	}

	ImGui::End();

#endif // _DEBUG

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 2; j++) {

			if (j != 0) {
				*spotLights_[i][j]->light_ = (*spotLights_[i][0]->light_);

				spotLights_[i][j]->light_->position.x *= -1;
				spotLights_[i][j]->light_->direction.x *= -1;
			}

			spotLights_[i][j]->Update();
		}
	}
}

void StageLights::Draw(Camera* camera) {

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 2; j++) {
			spotLights_[i][j]->Draw(*camera);
		}
	}
}
