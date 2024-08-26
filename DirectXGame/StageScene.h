#pragma once
#include <Model.h>
#include "Input.h"
#include "ViewProjection.h"
#include "BaseScene.h"
#include "Player.h"
#include "Enemy.h"
#include "DebugCamera.h"
#include "Skydome.h"
#include "Ground.h"
#include "Background.h"
#include "ShineBall.h"
#include "TPSCamera.h"
#include "CollisionManager.h"
#include "Reticle.h"
#include "UI.h"
#include "Audio.h"
#include "Pause.h"


class StageScene :	public BaseScene
{
public:
	StageScene();
	~StageScene()override;
public:
	void Init(Input* input,Audio* audio) override;
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* commandList, DirectXCommon* dxCommon_) override;

public://関数
	void CheckAllCollision();

private:
	void CreateShineBall();

public://ゲッター
	SCENE GetNextScene()override { return NextScene; }
	Player* GetPlayer() { return player_.get(); }
	TPSCamera* GetTPSCamera() { return tpsCamera_.get(); }
	Reticle* GetReticle() { return reticle_.get(); }
	Enemy* GetEnemy() { return enemy_.get(); }

private:
	//入力
	Input* input_ = nullptr;
	//オーディオ
	Audio* audio_ = nullptr;
	//3Dモデル
	std::unique_ptr<Model> modelSkydome_ = nullptr;
	std::unique_ptr<Model> modelGround_ = nullptr;
	std::unique_ptr<Model> modelShineBall_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//TPSカメラ
	std::unique_ptr<TPSCamera> tpsCamera_ = nullptr;
	//レティクル
	std::unique_ptr<Reticle> reticle_ = nullptr;

	//自キャラ
	std::unique_ptr<Player> player_ = nullptr;
	//敵キャラ
	std::unique_ptr<Enemy> enemy_ = nullptr;
	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//地面
	std::unique_ptr<Ground> ground_ = nullptr;
	//背景
	std::unique_ptr<Background> background_ = nullptr;
	//光玉
	std::list<ShineBall*> shineBalls_;

	//UI
	std::unique_ptr<UI> ui_ = nullptr;
	//ポーズメニュー
	std::unique_ptr<Pause> pause_ = nullptr;

	// 衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	//BGM関連
	uint32_t soundHandleBGM_;
	uint32_t voiceHandleBGM_;
	bool isSoundPlayBGM_ = false;
	const float soundVolumeBGM_ = 0.45f;


private:
	//タイマー
	int timer_ = 0;
	//何fに更新処理を行うか変数
	int framePerUpdate_ = 1;
	//ポーズ中か判定
	bool isPause_;

};

