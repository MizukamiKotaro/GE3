#pragma once

#include <memory>
#include "TextureManager.h"
#include "ModelDataManager.h"
#include "Audio.h"
#include "Input.h"
#include "Camera.h"
#include <optional>
#include "Sprite.h"
#include <memory>

enum SCENE { TITLE, SELECT, STAGE, CLEAR };

enum class Transition{
	kFromBlack,
	kOperation,
	kToBlack,
};

// シーン内での処理を行う基底クラス
class IScene
{
public:
	// シーン番号を管理する変数
	static int sceneNo_;
	// ステージ番号を管理する変数
	static int stagrNo_;

public:
	// 継承先で実装される関数
	// 抽象クラスなので純粋仮想関数とする
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	
	virtual ~IScene();

	// シーン番号のゲッター
	void FirstInit();
	int GetSceneNo();
	int GetStageNo();

	// シーン遷移用
	void FromBlackInitialize();
	void FromBlackUpdate();
	void ToBlackInitialize();
	void ToBlackUpdate(int sceneNo);

	void BlackDraw() { black_->Draw(*camera_.get()); }

protected:

	TextureManager* textureManager_ = nullptr;
	ModelDataManager* modelDataManager_ = nullptr;
	Audio* audio_ = nullptr;
	Input* input_ = nullptr;

protected:

	std::unique_ptr<Camera> camera_;

	std::optional<Transition> transitionRequest_ = std::nullopt;

	Transition transition_ = Transition::kFromBlack;

private:

	float transitionTimeCount_ = 0.0f;

	std::unique_ptr<Sprite> black_;
	const float kTransitionTime = 0.5f;

	bool sameScene_ = false;
};

