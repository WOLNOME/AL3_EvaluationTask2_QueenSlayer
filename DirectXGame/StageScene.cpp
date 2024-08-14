#include "StageScene.h"

StageScene::StageScene() { NextScene = STAGE; }

StageScene::~StageScene() {}

void StageScene::Init(Input* input) {
	// 入力
	input_ = input;
	// 3Dモデルの生成
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	// デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);
	debugCamera_->SetFarZ(2000.0f);
	//TPSカメラの生成
	tpsCamera_ = std::make_unique<TPSCamera>();
	// 自キャラの生成
	player_ = std::make_unique<Player>();
	//敵キャラの生成
	enemy_ = std::make_unique<Enemy>();
	// 天球の生成
	skydome_ = std::make_unique<Skydome>();
	//地面の生成
	ground_ = std::make_unique<Ground>();

	//プレイヤーにシーンを渡す
	player_->SetStageScene(this);
	//TPSカメラにシーンを渡す
	tpsCamera_->SetStageScene(this);

	//TPSカメラの初期化
	tpsCamera_->Initialize(input_);
	// 自キャラの初期化
	player_->Initialize({0.0f, 0.3f, 20.0f},input_);
	//敵キャラの初期化
	enemy_->Initialize({0.0f, 1.5f, 0.0f});
	// 天球の初期化
	skydome_->Initialize(modelSkydome_.get(), {0.0f, 0.0f, 0.0f});
	// 地面の初期化
	ground_->Initialize(modelGround_.get(), {0.0f, 0.0f, 0.0f});

}

void StageScene::Update() {
	// 自キャラの更新
	player_->Update();
	//敵キャラの更新
	enemy_->Update();
	//天球の更新
	skydome_->Update();
	// 地面の更新
	ground_->Update();



#ifdef _DEBUG
	if (input_->TriggerKey(DIK_TAB)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		} else {
			isDebugCameraActive_ = true;
		}
	}
#endif // _DEBUG
	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();

	} else {
		// TPSカメラの更新
		tpsCamera_->Update();
		viewProjection_.matView = tpsCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = tpsCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
	}
}

void StageScene::Draw(ID3D12GraphicsCommandList* commandList, DirectXCommon* dxCommon_) {
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// プレイヤーの描画
	player_->Draw(viewProjection_);
	//敵キャラの描画
	enemy_->Draw(viewProjection_);
	//天球の描画
	skydome_->Draw(viewProjection_);
	//地面の描画
	ground_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
