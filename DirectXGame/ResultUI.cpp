#include "ResultUI.h"
#include "TextureManager.h"
#include "WinApp.h"

ResultUI::ResultUI(Input* input, Audio* audio, GamePad* pad) {
	// インプット
	input_ = input;
	pad_ = pad;
	// オーディオ
	audio_ = audio;

	// サウンドハンドル
	soundHandleCursorMoveSE_ = audio_->LoadWave("Audio/cursorMoveSE.wav");
	soundHandleDecideSE_ = audio_->LoadWave("Audio/decideSE.wav");

	// ファイル名をしていしてテクスチャを読み込む
	textureHandleExitButton_ = TextureManager::Load("Result/exitButton.png");
	textureHandleRetryButton_ = TextureManager::Load("Result/retryButton.png");
	textureHandleGameClear_ = TextureManager::Load("Result/gameClear.png");
	textureHandleOperationKeyBoardUI_ = TextureManager::Load("Result/operationKeyBoardUI.png");
	textureHandleOperationGamePadUI_ = TextureManager::Load("Result/operationGamePadUI.png");

	// スプライト生成
	spriteExitButton_.reset(Sprite::Create(textureHandleExitButton_, {WinApp::kWindowWidth / 2.0f + 430, WinApp::kWindowHeight / 2.0f + 150}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteRetryButton_.reset(Sprite::Create(textureHandleRetryButton_, {WinApp::kWindowWidth / 2.0f + 430, WinApp::kWindowHeight / 2.0f + 270}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteGameClear_.reset(Sprite::Create(textureHandleGameClear_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f - 180.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteOperationKeyBoardUI_.reset(Sprite::Create(textureHandleOperationKeyBoardUI_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteOperationGamePadUI_.reset(Sprite::Create(textureHandleOperationGamePadUI_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));

	// ボタンサイズ取得
	sizeExitButton_ = spriteExitButton_->GetSize();
	sizeRetryButton_ = spriteRetryButton_->GetSize();

	// ポジション
	positionExitButton_ = spriteExitButton_->GetPosition();
	positionRetryButton_ = spriteRetryButton_->GetPosition();
}

ResultUI::~ResultUI() {
	// 音ストップ
	audio_->StopWave(voiceHandleCursorMoveSE_);
	audio_->StopWave(voiceHandleDecideSE_);
}

void ResultUI::Initialize() {
	// ボタンのサイズ初期化
	spriteRetryButton_->SetSize(sizeRetryButton_);
	spriteExitButton_->SetSize(sizeExitButton_);

	// 変数
	selectButton_ = ResultButton::EXITBUTTONRESULT;
	isRetry_ = false;
	isExit_ = false;
}

void ResultUI::Update(Device device) {
	device_ = device;
	// 変数をリセット
	isRetry_ = false;
	isExit_ = false;

	switch (selectButton_) {
	case EXITBUTTONRESULT:
		// スプライトサイズを大きくする
		spriteExitButton_->SetSize({sizeExitButton_.x * kStrngthButtonRatio_, sizeExitButton_.y * kStrngthButtonRatio_});
		// 選んでるやつがはねる処理
		if (spriteExitButton_->GetPosition().y >= positionExitButton_.y) {
			velocity_.y = -kSlectJump_;
			position_.y = 0.0f;
		}
		// 重力加速度加算
		velocity_.y += -kGravity_;
		// 速度加算処理
		position_.y += velocity_.y;
		// スプライトの位置に反映
		spriteExitButton_->SetPosition({positionExitButton_.x, positionExitButton_.y + position_.y});
		// 選択変更
		if (input_->TriggerKey(DIK_DOWN)||pad_->TriggerLStickDOWN()) {
			selectButton_ = RETRYBUTTONRESULT;
			// スプライトサイズもとに戻す
			spriteExitButton_->SetSize(sizeExitButton_);
			// スプライトのポジションもとに戻す
			spriteExitButton_->SetPosition(positionExitButton_);
			// サウンド再生
			isSoundPlayCursorMoveSE_ = true;
		}
		// 選択
		if (input_->TriggerKey(DIK_SPACE) || pad_->TriggerA()) {
			isExit_ = true;
			// サウンド再生
			isSoundPlayDecideSE_ = true;
		}
		break;
	case RETRYBUTTONRESULT:
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
		if (input_->TriggerKey(DIK_UP) || pad_->TriggerLStickUP()) {
			selectButton_ = EXITBUTTONRESULT;
			// スプライトサイズもとに戻す
			spriteRetryButton_->SetSize(sizeRetryButton_);
			// スプライトのポジションもとに戻す
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
	default:
		break;
	}
}

void ResultUI::Draw() {}

void ResultUI::DrawUI() {
	spriteRetryButton_->Draw();
	spriteExitButton_->Draw();
	spriteGameClear_->Draw();
	if (device_ == KEYBOARD) {
		spriteOperationKeyBoardUI_->Draw();
	} else if (device_ == GAMEPAD) {
		spriteOperationGamePadUI_->Draw();
	}
}

void ResultUI::AudioPlay() {
	if (isSoundPlayCursorMoveSE_) {
		voiceHandleCursorMoveSE_ = audio_->PlayWave(soundHandleCursorMoveSE_, false, soundVolumeCursorMoveSE_);
		isSoundPlayCursorMoveSE_ = false;
	}
	if (isSoundPlayDecideSE_) {
		voiceHandleDecideSE_ = audio_->PlayWave(soundHandleDecideSE_, false, soundVolumeDecideSE_);
		isSoundPlayDecideSE_ = false;
	}
}
