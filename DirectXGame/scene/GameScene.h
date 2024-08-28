#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Function.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Gradation.h"

class BaseScene;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public:
	// シーンを変える関数
	void ChangeScene(); 
public://ゲッター
	bool GetIsExit() { return isExit_; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// シーン
	std::unique_ptr<BaseScene> m_pScene = nullptr;
	SCENE CurrentScene_;
	SCENE NextScene_;

	//シーン遷移
	std::unique_ptr<Gradation> gradation_ = nullptr;
	const int kAnimationFrame_ = 80;
	int AnimationFrame_;
	bool isInNow_;
	bool isOutNow_;

	//アプリ終了フラグ
	bool isExit_ = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
