#include "StageScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "time.h"

StageScene::StageScene() {
	NextScene = STAGE;
	isExit = false;
}

StageScene::~StageScene() {
	// 解放
	for (ShineBall* shineBall : shineBalls_) {
		delete shineBall;
	}
	// BGMストップ
	audio_->StopWave(voiceHandleBGM_);
}

void StageScene::Init(Input* input, Audio* audio, GamePad* pad) {
	// 入力
	input_ = input;
	pad_ = pad;
	// オーディオ
	audio_ = audio;

	// テクスチャハンドル
	textureHandleSkydome_ = TextureManager::Load("skydome/sky_sphere.png");
	textureHandleGround_ = TextureManager::Load("ground/ground.png");

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
	// UIの生成
	ui_ = std::make_unique<UI>();
	// ポーズメニュー生成
	pause_ = std::make_unique<Pause>(input_, audio_, pad_);

	// プレイヤーにシーンを渡す
	player_->SetStageScene(this);
	// エネミーにシーンを渡す
	enemy_->SetStageScene(this);
	// TPSカメラにシーンを渡す
	tpsCamera_->SetStageScene(this);
	// UIにシーンを渡す
	ui_->SetStageScene(this);

	// TPSカメラの初期化
	tpsCamera_->Initialize(input_, pad_);
	// レティクル初期化
	reticle_->Initialize();
	// 自キャラの初期化
	player_->Initialize({0.0f, 0.3f, 20.0f}, input_, audio_, pad_, UseScene::USESTAGE);
	// 敵キャラの初期化
	enemy_->Initialize({0.0f, 1.5f, 0.0f}, audio_, UseScene::USESTAGE);
	// 天球の初期化
	skydome_->Initialize({0.0f, 0.0f, 0.0f}, textureHandleSkydome_);
	// 地面の初期化
	ground_->Initialize({0.0f, 0.0f, 0.0f}, textureHandleGround_);
	// 背景の初期化
	background_->Initialize({0.0f, 0.0f, 0.0f});
	// UIの初期化
	ui_->Initialize();

	// BGMのサウンドハンドル
	soundHandleBGM_ = audio_->LoadWave("Audio/stageBGM.wav");
	// 最初からBGM再生
	isSoundPlayBGM_ = true;

	// 変数系
	isPause_ = false;
}

void StageScene::Update(Device device) {
	device;
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
	// ポーズメニュー移行処理
	if (input_->TriggerKey(DIK_ESCAPE)||pad_->TriggerSTART()) {
		if (!isPause_) {
			isPause_ = true;
			// ポーズメニュー初期化
			pause_->Initialize();
		} else {
			isPause_ = false;
		}
	}
	// ポーズメニュー中処理
	if (isPause_) {
		// ポーズの更新
		pause_->Update();
		// コンティニューボタン押したら
		if (pause_->GetIsContinue()) {
			isPause_ = false;
		}
		// BackToTitleボタン押したら
		if (pause_->GetIsBackToTitle()) {
			NextScene = SCENE::TITLE;
		}
	}
	// 通常ゲーム処理
	else {
		// 必殺弾が当たったらヒットストップ掛ける
		if (player_->GetIsSpecialBulletDirection()) {
			framePerUpdate_ = 3;
		} else {
			framePerUpdate_ = 1;
		}
		// クリアしたらゆっくりになる
		if (enemy_->GetHP() <= 0) {
			framePerUpdate_ = 3;
		} else if (player_->GetHP() <= 0) {
			framePerUpdate_ = 3;
		}

		// タイマーインクリメント
		timer_++;
		// nfに1回更新処理を行う
		if (timer_ % framePerUpdate_ == 0) {
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
			// UIの更新
			ui_->Update();
		}
		// 当たり判定処理
		CheckAllCollision();

		// 光玉生成処理
		CreateShineBall();

		// クリアシーン遷移処理
		if (enemy_->GetHP() <= 0) {
			NextScene = DIRECTIION;
		}
		// ゲームオーバー遷移処理
		else if (player_->GetHP() <= 0) {
			if (NextScene == STAGE) {
				NextScene = GAMEOVER;
			}
		}

#ifdef _DEBUG
		ImGui::Begin("scene");
		ImGui::Text("lengthPtoE : %f", Length(Subtract(player_->GetWorldPostion(), enemy_->GetStomach()->GetWorldPosition())));
		ImGui::End();
#endif // _DEBUG
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
	// ロックオンUI描画
	tpsCamera_->DrawUI();
	// UI描画
	ui_->Draw();
	// ポーズ中スプライト処理
	if (isPause_) {
		pause_->DrawUI();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

#pragma region BGM,SE関連
	/// <summary>
	/// ここにAudio関連の処理を追加できる
	/// </summary>

	// BGMやSE関連
	if (isSoundPlayBGM_) {
		voiceHandleBGM_ = audio_->PlayWave(soundHandleBGM_, true, soundVolumeBGM_);
		isSoundPlayBGM_ = false;
	}
	// プレイヤー周り効果音
	player_->AudioPlay();
	// 敵周り効果音
	enemy_->AudioPlay();
	// ポーズ
	pause_->AudioPlay();

#pragma endregion
}

void StageScene::CheckAllCollision() {
	// 衝突マネージャーのクリア
	collisionManager_->ClearColliders();

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 必殺弾リストの取得
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
	// 必殺弾コライダーをリストに登録
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
		shineBall->Initialize(ballInitPosition, velocity);

		// 弾を登録する
		shineBalls_.push_back(shineBall);
		// 蓄積値リセット
		enemy_->SetSP(0);
	}
}
