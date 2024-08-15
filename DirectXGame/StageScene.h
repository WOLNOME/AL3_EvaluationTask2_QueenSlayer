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
#include "TPSCamera.h"
#include "CollisionManager.h"


class StageScene :	public BaseScene
{
public:
	StageScene();
	~StageScene()override;
public:
	void Init(Input* input) override;
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* commandList, DirectXCommon* dxCommon_) override;

public://関数
	void CheckAllCollision();

public://ゲッター
	SCENE GetNextScene()override { return NextScene; }
	Player* GetPlayer() { return player_.get(); }
	TPSCamera* GetTPSCamera() { return tpsCamera_.get(); }

private:
	//入力
	Input* input_ = nullptr;
	//3Dモデル
	std::unique_ptr<Model> modelSkydome_ = nullptr;
	std::unique_ptr<Model> modelGround_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//TPSカメラ
	std::unique_ptr<TPSCamera> tpsCamera_ = nullptr;

	//自キャラ
	std::unique_ptr<Player> player_ = nullptr;
	//敵キャラ
	std::unique_ptr<Enemy> enemy_ = nullptr;
	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//地面
	std::unique_ptr<Ground> ground_ = nullptr;

	// 衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
};

