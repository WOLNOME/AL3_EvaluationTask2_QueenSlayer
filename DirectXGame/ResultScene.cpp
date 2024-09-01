#include "ResultScene.h"
#include "TextureManager.h"

ResultScene::ResultScene() {
	NextScene = RESULT;
	isExit = false;
}

ResultScene::~ResultScene() {
	// BGMストップ
	audio_->StopWave(voiceHandleBGM_);
}

void ResultScene::Init(Input* input, Audio* audio, GamePad* pad) {
	// 入力
	input_ = input;
	pad_ = pad;
	// オーディオ
	audio_ = audio;

	// ワールドトランスフォーム初期化
	worldTransformParticle_.Initialize();

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

	// モデルテクスチャ
	textureHandleSkydome_ = TextureManager::Load("white.png");
	textureHandleGround_ = TextureManager::Load("white.png");
	// 2Dスプライトの生成

	// インスタンスの生成
	resultCamera_ = std::make_unique<ResultCamera>();
	skydome_ = std::make_unique<Skydome>();
	ground_ = std::make_unique<Ground>();
	player_ = std::make_unique<Player>();
	resultUI_ = std::make_unique<ResultUI>(input_, audio_,pad_);
	particle_ = std::make_unique<Particle>();

	// インスタンス初期化
	resultCamera_->Initialize();
	skydome_->Initialize({0.0f, 0.0f, 0.0f}, textureHandleSkydome_);
	ground_->Initialize({0.0f, 0.0f, 0.0f}, textureHandleGround_);
	player_->Initialize({0.0f, 0.3f, 0.0f}, input_, audio_, pad_, UseScene::USERESULT);
	resultUI_->Initialize();
	particle_->Initialize(&worldTransformParticle_, 0, ParticleKind::BRIZZARD, 50, false);

	// BGMのサウンドハンドル
	soundHandleBGM_ = audio_->LoadWave("Audio/resultBGM.wav");
	// 最初からBGM再生
	isSoundPlayBGM_ = true;
}

void ResultScene::Update(Device device) {
	// カメラの更新処理
	resultCamera_->Update();
	viewProjection_.matView = resultCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = resultCamera_->GetViewProjection().matProjection;
	// ビュープロジェクション行列の更新と転送
	viewProjection_.TransferMatrix();

	// 各オブジェクトの更新処理
	ground_->Update();
	skydome_->Update();
	player_->Update();
	resultUI_->Update(device);
	particle_->Update();

	if (resultUI_->GetIsRetry()) {
		if (NextScene == RESULT) {
			NextScene = SCENE::STAGE;
		}
	} else if (resultUI_->GetIsExit()) {
		if (NextScene == RESULT) {
			NextScene = SCENE::TITLE;
		}
	}

	worldTransformParticle_.UpdateMatrix();
}

void ResultScene::Draw(ID3D12GraphicsCommandList* commandList, DirectXCommon* dxCommon_) {
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
	player_->Draw(viewProjection_);
	particle_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	resultUI_->DrawUI();

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
	resultUI_->AudioPlay();

#pragma endregion
}
