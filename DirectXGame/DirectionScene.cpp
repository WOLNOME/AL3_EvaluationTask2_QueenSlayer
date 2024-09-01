#include "DirectionScene.h"
#include "TextureManager.h"
#include "WinApp.h"

DirectionScene::DirectionScene() {
	NextScene = DIRECTIION;
	isExit = false;
}

DirectionScene::~DirectionScene() {
	// BGMストップ
	audio_->StopWave(voiceHandleExplosionSE_);
}

void DirectionScene::Init(Input* input, Audio* audio, GamePad* pad) {
	// 入力
	input_ = input;
	pad_ = pad;
	// オーディオ
	audio_ = audio;

	// BGMのサウンドハンドル
	soundHandleExplosionSE_ = audio_->LoadWave("Audio/damageLargeSE.wav");

	// ワールドトランスフォーム初期化
	worldTransformEnemyHead_.Initialize();
	worldTransformEnemyChest_.Initialize();
	worldTransformEnemyStomach_.Initialize();
	worldTransformEnemyHeadInitPos_.Initialize();
	worldTransformEnemyChestInitPos_.Initialize();
	worldTransformEnemyStomachInitPos_.Initialize();
	// ワールドトランスフォーム初期位置設定
	worldTransformEnemyHead_.translation_.y = kEnemyRadius_ + (kEnemyRadius_ * 2) * 2;
	worldTransformEnemyChest_.translation_.y = kEnemyRadius_ + (kEnemyRadius_ * 2) * 1;
	worldTransformEnemyStomach_.translation_.y = kEnemyRadius_ + (kEnemyRadius_ * 2) * 0;
	worldTransformEnemyHeadInitPos_.translation_.y = kEnemyRadius_ + (kEnemyRadius_ * 2) * 2;
	worldTransformEnemyChestInitPos_.translation_.y = kEnemyRadius_ + (kEnemyRadius_ * 2) * 1;
	worldTransformEnemyStomachInitPos_.translation_.y = kEnemyRadius_ + (kEnemyRadius_ * 2) * 0;
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	// モデルテクスチャ
	textureHandleSkydome_ = TextureManager::Load("skydome/sky_sphere.png");
	textureHandleGround_ = TextureManager::Load("ground/ground.png");
	// 3Dモデル
	modelEnemyHead_.reset(Model::CreateFromOBJ("enemyHead", true));
	modelEnemyChest_.reset(Model::CreateFromOBJ("enemyChest", true));
	modelEnemyStomach_.reset(Model::CreateFromOBJ("enemyStomach", true));
	// スプライトテクスチャ
	textureHandleSkipKeyBoardUI_ = TextureManager::Load("Direction/skipKeyBoardUI.png");
	textureHandleSkipGamePadUI_ = TextureManager::Load("Direction/skipGamePadUI.png");
	// 2Dスプライト
	spriteSkipKeyBoardUI_.reset(Sprite::Create(textureHandleSkipKeyBoardUI_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteSkipGamePadUI_.reset(Sprite::Create(textureHandleSkipGamePadUI_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	// インスタンス生成
	directionCamera_ = std::make_unique<DirectionCamera>();
	skydome_ = std::make_unique<Skydome>();
	ground_ = std::make_unique<Ground>();
	background_ = std::make_unique<Background>();
	particleHead_ = std::make_unique<Particle>();
	particleChest_ = std::make_unique<Particle>();
	particleStomach_ = std::make_unique<Particle>();
	// インスタンス初期化
	directionCamera_->Initialize(worldTransformEnemyHeadInitPos_.translation_, kCameraMoveTime_);
	ground_->Initialize({0.0f, 0.0f, 0.0f}, textureHandleGround_);
	skydome_->Initialize({0.0f, 0.0f, 0.0f}, textureHandleSkydome_);
	background_->Initialize({0.0f, 0.0f, 0.0f});
	particleHead_->Initialize(&worldTransformEnemyHeadInitPos_, 0, ParticleKind::POP, 30, false);
	particleChest_->Initialize(&worldTransformEnemyChestInitPos_, 0, ParticleKind::POP, 30, false);
	particleStomach_->Initialize(&worldTransformEnemyStomachInitPos_, 0, ParticleKind::POP, 30, false);
}

void DirectionScene::Update(Device device) {
	device_ = device;
	// タイマーインクリメント
	timer_++;
	// スキップ
	if (input_->TriggerKey(DIK_SPACE) || pad_->TriggerSTART()) {
		if (NextScene == SCENE::DIRECTIION) {
			NextScene = SCENE::RESULT;
		}
	}

	// nfに一回更新処理を行う
	if (timer_ % framePerUpdate == 0) {
		// カメラの更新処理
		directionCamera_->Update(timer_);
		viewProjection_.matView = directionCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = directionCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();

		// 各オブジェクトの更新処理
		ground_->Update();
		skydome_->Update();
		background_->Update();
		if (timer_ > kExplosionTime_) {
			particleHead_->Update();
			particleChest_->Update();
			particleStomach_->Update();
		}

		// カメラ移動中は敵振動処理
		if (timer_ < kCameraMoveTime_) {
			srand((int)std::powf((float)timer_, 1));
			worldTransformEnemyHead_.translation_.x = worldTransformEnemyHeadInitPos_.translation_.x + float(rand() % 6 - 3) * (1.0f / 10.0f);
			worldTransformEnemyChest_.translation_.x = worldTransformEnemyChestInitPos_.translation_.x + float(rand() % 6 - 3) * (1.0f / 10.0f);
			worldTransformEnemyStomach_.translation_.x = worldTransformEnemyStomachInitPos_.translation_.x + float(rand() % 6 - 3) * (1.0f / 10.0f);
			srand((int)std::powf((float)timer_, 2));
			worldTransformEnemyHead_.translation_.z = worldTransformEnemyHeadInitPos_.translation_.z + float(rand() % 6 - 3) * (1.0f / 10.0f);
			worldTransformEnemyChest_.translation_.z = worldTransformEnemyChestInitPos_.translation_.z + float(rand() % 6 - 3) * (1.0f / 10.0f);
			worldTransformEnemyStomach_.translation_.z = worldTransformEnemyStomachInitPos_.translation_.z + float(rand() % 6 - 3) * (1.0f / 10.0f);
		}
		if (timer_ == kExplosionTime_) {
			// 速度の設定
			srand(int(timer_ * 3));
			velocityEnemyHead_.x = float(rand() % 10 - 5) * (1.0f / 10.0f);
			srand(int(timer_ * 4));
			velocityEnemyHead_.z = float(rand() % 10 - 5) * (1.0f / 10.0f);
			srand(int(timer_ * 5));
			velocityEnemyChest_.x = float(rand() % 10 - 5) * (1.0f / 10.0f);
			srand(int(timer_ * 2));
			velocityEnemyChest_.z = float(rand() % 10 - 5) * (1.0f / 10.0f);
			srand(int(timer_ * 6));
			velocityEnemyStomach_.x = float(rand() % 10 - 5) * (1.0f / 10.0f);
			srand(int(timer_ * 4));
			velocityEnemyStomach_.z = float(rand() % 10 - 5) * (1.0f / 10.0f);

			velocityEnemyHead_.y = 0.6f;
			velocityEnemyChest_.y = 0.6f;
			velocityEnemyStomach_.y = 0.6f;

			// サウンド再生
			isSoundPlayExplosionSE_ = true;
		}
		if (timer_ > kExplosionTime_) {
			// 重力処理
			velocityEnemyHead_.y += kGravity_.y;
			velocityEnemyChest_.y += kGravity_.y;
			velocityEnemyStomach_.y += kGravity_.y;

			// 速度加算
			worldTransformEnemyHead_.translation_.x += velocityEnemyHead_.x;
			worldTransformEnemyHead_.translation_.y += velocityEnemyHead_.y;
			worldTransformEnemyHead_.translation_.z += velocityEnemyHead_.z;
			worldTransformEnemyChest_.translation_.x += velocityEnemyChest_.x;
			worldTransformEnemyChest_.translation_.y += velocityEnemyChest_.y;
			worldTransformEnemyChest_.translation_.z += velocityEnemyChest_.z;
			worldTransformEnemyStomach_.translation_.x += velocityEnemyStomach_.x;
			worldTransformEnemyStomach_.translation_.y += velocityEnemyStomach_.y;
			worldTransformEnemyStomach_.translation_.z += velocityEnemyStomach_.z;
		}
	}

	// 遅くする処理
	if (timer_ == kExplosionTime_) {
		framePerUpdate = 3;
	}
	if (timer_ == kMoveScene_) {
		framePerUpdate = 4;
		NextScene = SCENE::RESULT;
	}

	// ワールドトランスフォームの更新
	worldTransformEnemyHead_.UpdateMatrix();
	worldTransformEnemyHeadInitPos_.UpdateMatrix();
	worldTransformEnemyChest_.UpdateMatrix();
	worldTransformEnemyChestInitPos_.UpdateMatrix();
	worldTransformEnemyStomach_.UpdateMatrix();
	worldTransformEnemyStomachInitPos_.UpdateMatrix();
}

void DirectionScene::Draw(ID3D12GraphicsCommandList* commandList, DirectXCommon* dxCommon_) {
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

	// オブジェクト
	ground_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	background_->Draw(viewProjection_);

	modelEnemyHead_->Draw(worldTransformEnemyHead_, viewProjection_);
	modelEnemyChest_->Draw(worldTransformEnemyChest_, viewProjection_);
	modelEnemyStomach_->Draw(worldTransformEnemyStomach_, viewProjection_);

	if (timer_ > kExplosionTime_) {
		particleHead_->Draw(viewProjection_);
		particleChest_->Draw(viewProjection_);
		particleStomach_->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	if (device_ == KEYBOARD) {
		spriteSkipKeyBoardUI_->Draw();
	} else if (device_ == GAMEPAD) {
		spriteSkipGamePadUI_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

#pragma region BGM,SE関連
	/// <summary>
	/// ここにAudio関連の処理を追加できる
	/// </summary>

	if (isSoundPlayExplosionSE_) {
		voiceHandleExplosionSE_ = audio_->PlayWave(soundHandleExplosionSE_, false, soundVolumeExplosionSE_);
		isSoundPlayExplosionSE_ = false;
	}

#pragma endregion
}
