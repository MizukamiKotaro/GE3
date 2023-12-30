#pragma once

#include <memory>
#include "TextureManager.h"
#include "ModelDataManager.h"
#include "Audio.h"
#include "Input.h"
#include "Camera.h"

enum SCENE { TITLE, STAGE, CLEAR };

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
	virtual void DrawPostEffect() = 0;

	virtual ~IScene();

	// シーン番号のゲッター
	void FirstInit();
	int GetSceneNo();
	int GetStageNo();

protected:

	TextureManager* textureManager_ = nullptr;
	ModelDataManager* modelDataManager_ = nullptr;
	Audio* audio_ = nullptr;
	Input* input_ = nullptr;

protected:

	std::unique_ptr<Camera> camera_;
};

