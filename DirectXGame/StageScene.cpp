#include "StageScene.h"
#include "time.h"
#include "ImGuiManager.h"

StageScene::StageScene() { NextScene = STAGE; }

StageScene::~StageScene() {
	// 解放
	for (ShineBall* shineBall : shineBalls_) {
		delete shineBall;
	}
}

void StageScene::Init(Input* input) {
	// 入力
	input_ = input;
	// 3Dモデルの生成
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	modelShineBall_.reset(Model::CreateFromOBJ("shineBall", true));
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	// デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);
	debugCamera_->SetFarZ(2000.0f);
	// TPSカメラの生成
	tpsCamera_ = std::make_unique<TPSCamera>();
	// レティクル
	reticle_ = std::make_unique<Reticle>();
	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 敵キャラの生成
	enemy_ = std::make_unique<Enemy>();
	// 天球の生成
	skydome_ = std::make_unique<Skydome>();
	// 地面の生成
	ground_ = std::make_unique<Ground>();
	// 背景の生成
	background_ = std::make_unique<Background>();
	// 衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	//UIの生成
	ui_ = std::make_unique<UI>();

	// プレイヤーにシーンを渡す
	player_->SetStageScene(this);
	// エネミーにシーンを渡す
	enemy_->SetStageScene(this);
	// TPSカメラにシーンを渡す
	tpsCamera_->SetStageScene(this);
	//UIにシーンを渡す
	ui_->SetStageScene(this);

	// TPSカメラの初期化
	tpsCamera_->Initialize(input_);
	// レティクル初期化
	reticle_->Initialize();
	// 自キャラの初期化
	player_->Initialize({0.0f, 0.3f, 20.0f}, input_);
	// 敵キャラの初期化
	enemy_->Initialize({0.0f, 1.5f, 0.0f});
	// 天球の初期化
	skydome_->Initialize(modelSkydome_.get(), {0.0f, 0.0f, 0.0f});
	// 地面の初期化
	ground_->Initialize(modelGround_.get(), {0.0f, 0.0f, 0.0f});
	// 背景の初期化
	background_->Initialize({0.0f, 0.0f, 0.0f});
	//UIの初期化
	ui_->Initialize();

}

void StageScene::Update() {
	// デスフラグの立った光玉を削除
	shineBalls_.remove_if([](ShineBall* shineBall) {
		if (shineBall->isDead()) {
			delete shineBall;
			return true;
		}
		return false;
	});
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

	// レティクルの更新
	reticle_->Update(viewProjection_);
	// 自キャラの更新
	player_->Update();
	// 敵キャラの更新
	enemy_->Update();
	// 天球の更新
	skydome_->Update();
	// 地面の更新
	ground_->Update();
	// 背景の更新
	background_->Update();
	// 光玉の更新
	for (ShineBall* shineBall : shineBalls_) {
		shineBall->Update();
	}
	//UIの更新
	ui_->Update();

	// 当たり判定処理
	CheckAllCollision();

	// 光玉生成処理
	CreateShineBall();


#ifdef _DEBUG
	ImGui::Begin("scene");
	ImGui::Text("lengthPtoE : %f", Length(Subtract(player_->GetWorldPostion(), enemy_->GetStomach()->GetWorldPosition())));
	ImGui::End();
#endif // _DEBUG
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
	// 敵キャラの描画
	enemy_->Draw(viewProjection_);
	// 天球の描画
	skydome_->Draw(viewProjection_);
	// 地面の描画
	ground_->Draw(viewProjection_);
	// 背景の描画
	background_->Draw(viewProjection_);
	// 光玉の描画
	for (ShineBall* shineBall : shineBalls_) {
		shineBall->Draw(viewProjection_);
	}

	// 3Dレティクルの描画(デバッグ時)
	reticle_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// 2Dレティクル描画
	reticle_->DrawUI();
	//UI描画
	ui_->Draw();


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void StageScene::CheckAllCollision() {
	// 衝突マネージャーのクリア
	collisionManager_->ClearColliders();

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//必殺弾リストの取得
	const std::list<PlayerSpecialBullet*>& playerSpecialBullets = player_->GetSpecialBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBulletsStomach = enemy_->GetStomach()->GetBullets();
	const std::list<EnemyBullet*>& enemyBulletsChest = enemy_->GetChest()->GetBullets();
	const std::list<EnemyBullet*>& enemyBulletsHead = enemy_->GetHead()->GetBullets();
	// コライダー
	std::list<Collider*> colliders_;
	// コライダーをリストに登録
	colliders_.push_back(player_.get()->GetVehicle().get());
	colliders_.push_back(enemy_.get()->GetStomach().get());
	colliders_.push_back(enemy_.get()->GetChest().get());
	colliders_.push_back(enemy_.get()->GetHead().get());
	// 自弾コライダーをリストに登録
	for (PlayerBullet* pbullet : playerBullets) {
		colliders_.push_back(pbullet);
	}
	//必殺弾コライダーをリストに登録
	for (PlayerSpecialBullet* pbullet : playerSpecialBullets) {
		colliders_.push_back(pbullet);
	}
	// 敵弾コライダーをリストに登録
	for (EnemyBullet* ebullet : enemyBulletsStomach) {
		colliders_.push_back(ebullet);
	}
	for (EnemyBullet* ebullet : enemyBulletsChest) {
		colliders_.push_back(ebullet);
	}
	for (EnemyBullet* ebullet : enemyBulletsHead) {
		colliders_.push_back(ebullet);
	}
	// 光玉リストに登録
	for (ShineBall* shineBall : shineBalls_) {
		colliders_.push_back(shineBall);
	}

	// 衝突マネージャーのリストにコライダーを登録する
	collisionManager_->SetColliders(colliders_);
	// 衝突判定の当たり判定処理を呼び出す
	collisionManager_->CheckCollision();
}

void StageScene::CreateShineBall() {
	// 生成条件
	if (enemy_->GetSP() >= 7) {
		//  弾を生成
		ShineBall* shineBall = new ShineBall();
		// 弾にステージシーンを渡す
		shineBall->SetStageScene(this);
		// 弾の発射位置を確定
		Vector3 ballInitPosition = enemy_->GetChest()->GetWorldPosition();
		// 乱数生成
		uint32_t currenttime = (int)time(nullptr);
		srand(currenttime * 8);
		float theta = float(rand() % 360) / 180.0f * pi;
		// 弾の速度(ベクトル)を確定
		Vector3 velocity;
		velocity.x = 0.3f * std::cosf(theta);
		velocity.y = 0.5f;
		velocity.z = 0.3f * std::sinf(theta);

		// 弾を初期化
		shineBall->Initialize(modelShineBall_.get(), ballInitPosition, velocity);

		// 弾を登録する
		shineBalls_.push_back(shineBall);
		// 蓄積値リセット
		enemy_->SetSP(0);
	}
}
