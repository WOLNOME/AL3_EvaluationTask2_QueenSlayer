#include "GameOverUI.h"
#include "TextureManager.h"
#include "WinApp.h"

GameOverUI::GameOverUI(Input* input, Audio* audio, GamePad* pad) {
	// インプット
	input_ = input;
	pad_ = pad;
	// オーディオ
	audio_ = audio;

	// サウンドハンドル
	soundHandleCursorMoveSE_ = audio_->LoadWave("Audio/cursorMoveSE.wav");
	soundHandleDecideSE_ = audio_->LoadWave("Audio/decideSE.wav");

	// ファイル名をしていしてテクスチャを読み込む
	textureHandleRetryButton_ = TextureManager::Load("GameOver/retryButton.png");
	textureHandleRetireButton_ = TextureManager::Load("GameOver/retireButton.png");
	textureHandleGameOver_ = TextureManager::Load("GameOver/gameOver.png");
	textureHandleOperationUI_ = TextureManager::Load("GameOver/operationUI.png");

	// スプライト生成
	spriteRetryButton_.reset(Sprite::Create(textureHandleRetryButton_, {WinApp::kWindowWidth / 2.0f + 430, WinApp::kWindowHeight / 2.0f + 150}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteRetireButton_.reset(Sprite::Create(textureHandleRetireButton_, {WinApp::kWindowWidth / 2.0f + 430, WinApp::kWindowHeight / 2.0f + 270}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteGameOver_.reset(Sprite::Create(textureHandleGameOver_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f - 180.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteOperationUI_.reset(Sprite::Create(textureHandleOperationUI_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));

	// ボタンサイズ取得
	sizeRetryButton_ = spriteRetryButton_->GetSize();
	sizeRetireButton_ = spriteRetireButton_->GetSize();

	// ポジション
	positionRetryButton_ = spriteRetryButton_->GetPosition();
	positionRetireButton_ = spriteRetireButton_->GetPosition();
}

GameOverUI::~GameOverUI() {
	// 音ストップ
	audio_->StopWave(voiceHandleCursorMoveSE_);
	audio_->StopWave(voiceHandleDecideSE_);
}

void GameOverUI::Initialize() {
	// ボタンのサイズ初期化
	spriteRetryButton_->SetSize(sizeRetryButton_);
	spriteRetireButton_->SetSize(sizeRetireButton_);

	// 変数
	selectButton_ = GameOverButton::RETRYBUTTONGAMEOVER;
	isRetry_ = false;
	isRetire_ = false;
}

void GameOverUI::Update() {
	// 変数をリセット
	isRetry_ = false;
	isRetire_ = false;

	switch (selectButton_) {
	case RETRYBUTTONGAMEOVER:
		// スプライトサイズを大きくする
		spriteRetryButton_->SetSize({sizeRetryButton_.x * kStrngthButtonRatio_, sizeRetryButton_.y * kStrngthButtonRatio_});
		// 選んでるやつがはねる処理
		if (spriteRetryButton_->GetPosition().y >= positionRetryButton_.y) {
			velocity_.y = -kSlectJump_;
			position_.y = 0.0f;
		}
		// 重力加速度加算
		velocity_.y += -kGravity_;
		// 速度加算処理
		position_.y += velocity_.y;
		// スプライトの位置に反映
		spriteRetryButton_->SetPosition({positionRetryButton_.x, positionRetryButton_.y + position_.y});
		// 選択変更
		if (input_->TriggerKey(DIK_DOWN)||pad_->TriggerLStickDOWN()) {
			selectButton_ = RETIREBUTTONGAMEOVER;
			// スプライトサイズもとに戻す
			spriteRetryButton_->SetSize(sizeRetryButton_);
			//スプライトのポジションもとに戻す
			spriteRetryButton_->SetPosition(positionRetryButton_);
			// サウンド再生
			isSoundPlayCursorMoveSE_ = true;
		}
		// 選択
		if (input_->TriggerKey(DIK_SPACE) || pad_->TriggerA()) {
			isRetry_ = true;
			// サウンド再生
			isSoundPlayDecideSE_ = true;
		}
		break;
	case RETIREBUTTONGAMEOVER:
		// スプライトサイズを大きくする
		spriteRetireButton_->SetSize({sizeRetireButton_.x * kStrngthButtonRatio_, sizeRetireButton_.y * kStrngthButtonRatio_});
		// 選んでるやつがはねる処理
		if (spriteRetireButton_->GetPosition().y >= positionRetireButton_.y) {
			velocity_.y = -kSlectJump_;
			position_.y = 0.0f;
		}
		// 重力加速度加算
		velocity_.y += -kGravity_;
		// 速度加算処理
		position_.y += velocity_.y;
		// スプライトの位置に反映
		spriteRetireButton_->SetPosition({positionRetireButton_.x, positionRetireButton_.y + position_.y});
		// 選択変更
		if (input_->TriggerKey(DIK_UP)||pad_->TriggerLStickUP()) {
			selectButton_ = RETRYBUTTONGAMEOVER;
			// スプライトサイズもとに戻す
			spriteRetireButton_->SetSize(sizeRetireButton_);
			// スプライトのポジションもとに戻す
			spriteRetireButton_->SetPosition(positionRetireButton_);
			// サウンド再生
			isSoundPlayCursorMoveSE_ = true;
		}
		// 選択
		if (input_->TriggerKey(DIK_SPACE) || pad_->TriggerA()) {
			isRetire_ = true;
			// サウンド再生
			isSoundPlayDecideSE_ = true;
		}
		break;
	default:
		break;
	}
}

void GameOverUI::Draw() {}

void GameOverUI::DrawUI() {
	spriteRetryButton_->Draw();
	spriteRetireButton_->Draw();
	spriteGameOver_->Draw();
	spriteOperationUI_->Draw();
}

void GameOverUI::AudioPlay() {
	if (isSoundPlayCursorMoveSE_) {
		voiceHandleCursorMoveSE_ = audio_->PlayWave(soundHandleCursorMoveSE_, false, soundVolumeCursorMoveSE_);
		isSoundPlayCursorMoveSE_ = false;
	}
	if (isSoundPlayDecideSE_) {
		voiceHandleDecideSE_ = audio_->PlayWave(soundHandleDecideSE_, false, soundVolumeDecideSE_);
		isSoundPlayDecideSE_ = false;
	}
}
