#include "GameOverScene.h"
#include "TextureManager.h"

GameOverScene::GameOverScene() {
	NextScene = GAMEOVER;
	isExit = false;
}

GameOverScene::~GameOverScene() {
	// BGMストップ
	audio_->StopWave(voiceHandleBGM_);
}

void GameOverScene::Init(Input* input, Audio* audio, GamePad* pad) {
	// 入力
	input_ = input;
	pad_ = pad;
	// オーディオ
	audio_ = audio;

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

	// モデルテクスチャ
	textureHandleSkydome_ = TextureManager::Load("black.png");
	textureHandleGround_ = TextureManager::Load("black.png");
	// 2Dスプライトの生成

	// インスタンスの生成
	gameOverCamera_ = std::make_unique<GameOverCamera>();
	skydome_ = std::make_unique<Skydome>();
	ground_ = std::make_unique<Ground>();
	spotlight_ = std::make_unique<Spotlight>();
	player_ = std::make_unique<Player>();
	gameOverUI_ = std::make_unique<GameOverUI>(input_,audio_,pad_);

	// インスタンス初期化
	gameOverCamera_->Initialize();
	skydome_->Initialize({0.0f, 0.0f, 0.0f}, textureHandleSkydome_);
	ground_->Initialize({0.0f, 0.0f, 0.0f}, textureHandleGround_);
	spotlight_->Initialize({0.0f, 0.0f, 0.0f});
	player_->Initialize({0.0f, 0.3f, 0.0f}, input_, audio_,pad_, UseScene::USEGAMEOVER);
	gameOverUI_->Initialize();

	// BGMのサウンドハンドル
	soundHandleBGM_ = audio_->LoadWave("Audio/gameOverBGM.wav");
	// 最初からBGM再生
	isSoundPlayBGM_ = true;
}

void GameOverScene::Update(Device device) {
	// カメラの更新処理
	gameOverCamera_->Update();
	viewProjection_.matView = gameOverCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = gameOverCamera_->GetViewProjection().matProjection;
	// ビュープロジェクション行列の更新と転送
	viewProjection_.TransferMatrix();

	// 各オブジェクトの更新処理
	ground_->Update();
	skydome_->Update();
	spotlight_->Update();
	player_->Update();
	gameOverUI_->Update(device);

	if (gameOverUI_->GetIsRetry()) {
		if (NextScene == GAMEOVER) {
			NextScene = SCENE::STAGE;
		}
	} else if (gameOverUI_->GetIsRetire()) {
		if (NextScene == GAMEOVER) {
			NextScene = SCENE::TITLE;
		}
	}
}

void GameOverScene::Draw(ID3D12GraphicsCommandList* commandList, DirectXCommon* dxCommon_) {
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

	spotlight_->Draw(viewProjection_);
	player_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	gameOverUI_->DrawUI();

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
	gameOverUI_->AudioPlay();

#pragma endregion
}
