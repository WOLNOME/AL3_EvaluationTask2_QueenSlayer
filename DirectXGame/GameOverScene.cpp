#include "GameOverScene.h"

GameOverScene::GameOverScene() {
	NextScene = GAMEOVER; 
	isExit = false;
}

GameOverScene::~GameOverScene() {
	// BGMストップ
	audio_->StopWave(voiceHandleBGM_);
}

void GameOverScene::Init(Input* input, Audio* audio) {
	// 入力
	input_ = input;
	//オーディオ
	audio_ = audio;

	// BGMのサウンドハンドル
	soundHandleBGM_ = audio_->LoadWave("Audio/gameOverBGM.wav");
	// 最初からBGM再生
	isSoundPlayBGM_ = true;

}

void GameOverScene::Update() {
	// このキーを押すと参照したシーンに移動
	if (input_->TriggerKey(DIK_TAB)) {
		NextScene = TITLE;
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

#pragma region BGM,SE関連
	/// <summary>
	/// ここにAudio関連の処理を追加できる
	/// </summary>

	// BGMやSE関連
	if (isSoundPlayBGM_) {
		voiceHandleBGM_ = audio_->PlayWave(soundHandleBGM_, true, soundVolumeBGM_);
		isSoundPlayBGM_ = false;
	}

#pragma endregion
}
