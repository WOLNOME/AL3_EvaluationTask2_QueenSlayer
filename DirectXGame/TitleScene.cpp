#include "TitleScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"

TitleScene::TitleScene() {
	NextScene = TITLE;
	isExit = false;
}

TitleScene::~TitleScene() {
	// BGMストップ
	audio_->StopWave(voiceHandleBGM_);
}

void TitleScene::Init(Input* input, Audio* audio, GamePad* pad) {
	// 入力
	input_ = input;
	pad_ = pad;
	// オーディオ
	audio_ = audio;

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

	// モデルテクスチャ
	textureHandleSkydome_ = TextureManager::Load("skydome/sky_sphere.png");
	textureHandleGround_ = TextureManager::Load("ground/ground.png");
	// 2Dスプライトの生成

	// インスタンスの生成
	titleCamera_ = std::make_unique<TitleCamera>();
	skydome_ = std::make_unique<Skydome>();
	ground_ = std::make_unique<Ground>();
	background_ = std::make_unique<Background>();
	enemy_ = std::make_unique<Enemy>();
	titleUI_ = std::make_unique<TitleUI>(input_, audio_,pad_);

	// インスタンス初期化
	skydome_->Initialize({0.0f, 0.0f, 0.0f}, textureHandleSkydome_);
	ground_->Initialize({0.0f, 0.0f, 0.0f}, textureHandleGround_);
	background_->Initialize({0.0f, 0.0f, 0.0f});
	enemy_->Initialize({0.0f, 1.5f, 0.0f}, audio_, UseScene::USETITLE);
	titleUI_->Initialize();

	// タイトルカメラの親を設定
	titleCamera_->SetParent(&enemy_->GetChest()->GetWorldTransform());
	// タイトルカメラ初期化
	titleCamera_->Initialize();

	// タイマー初期化
	timer_ = 0;

	// BGMのサウンドハンドル
	soundHandleBGM_ = audio_->LoadWave("Audio/titleBGM.wav");
	// 最初からBGM再生
	isSoundPlayBGM_ = true;
}

void TitleScene::Update(Device device) {

	// タイトルカメラの更新
	titleCamera_->Update();
	viewProjection_.matView = titleCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = titleCamera_->GetViewProjection().matProjection;
	// ビュープロジェクション行列の更新と転送
	viewProjection_.TransferMatrix();

	// オブジェクトの更新処理
	skydome_->Update();
	ground_->Update();
	background_->Update();
	enemy_->Update();

	// スプライトの更新処理
	titleUI_->Update(device);

	// プレイボタンが押されたら
	if (titleUI_->GetIsPlay()) {
		NextScene = SCENE::STAGE;
	}
	// イグジットボタンが押されたら
	if (titleUI_->GetIsExit()) {
		if (NextScene == SCENE::TITLE) {
			isExit = true;
		}
	}
}

void TitleScene::Draw(ID3D12GraphicsCommandList* commandList, DirectXCommon* dxCommon_) {

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

	// オブジェクトの描画
	skydome_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);
	background_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライトの描画
	titleUI_->DrawUI();

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
	titleUI_->AudioPlay();

#pragma endregion
}
