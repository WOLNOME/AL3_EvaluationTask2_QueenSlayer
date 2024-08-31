#include "TitleUI.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "WinApp.h"

TitleUI::TitleUI(Input* input, Audio* audio, GamePad* pad) {
	// インプット
	input_ = input;
	pad_ = pad;
	// オーディオ
	audio_ = audio;

	// サウンドハンドル
	soundHandleCursorMoveSE_ = audio_->LoadWave("Audio/cursorMoveSE.wav");
	soundHandleDecideSE_ = audio_->LoadWave("Audio/decideSE.wav");

	// ファイル名をしていしてテクスチャを読み込む
	textureHandlePlayButton_ = TextureManager::Load("Title/play!Button.png");
	textureHandleHowToPlayButton_ = TextureManager::Load("Title/howToPlayButton.png");
	textureHandleExitButton_ = TextureManager::Load("Title/exitButton.png");
	textureHandleBackWhite_ = TextureManager::Load("white.png");
	textureHandleTitleRogo_ = TextureManager::Load("Title/titleRogo.png");
	textureHandleOperationKeyBoardUI_ = TextureManager::Load("Title/operationKeyBoardUI.png");
	textureHandleOperationGamePadUI_ = TextureManager::Load("Title/operationGamePadUI.png");
	textureHandleHTPSlide_[0] = TextureManager::Load("Title/howToPlayPage0.png");
	textureHandleHTPSlide_[1] = TextureManager::Load("Title/howToPlayPage1.png");
	textureHandleHTPSlide_[2] = TextureManager::Load("Title/howToPlayPage2.png");
	textureHandleHTPSlide_[3] = TextureManager::Load("Title/howToPlayPage3.png");
	textureHandleHTPSlide_[4] = TextureManager::Load("Title/howToPlayPage4.png");

	// スプライト生成
	spritePlayButton_.reset(Sprite::Create(textureHandlePlayButton_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHowToPlayButton_.reset(Sprite::Create(textureHandleHowToPlayButton_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f + 100.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteExitButton_.reset(Sprite::Create(textureHandleExitButton_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f + 200.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteBackWhite_.reset(Sprite::Create(textureHandleBackWhite_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, kWhiteBackAlpha_), {0.5f, 0.5f}));
	spriteTitleRogo_.reset(Sprite::Create(textureHandleTitleRogo_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f - 180.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteOperationKeyBoardUI_.reset(Sprite::Create(textureHandleOperationKeyBoardUI_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteOperationGamePadUI_.reset(Sprite::Create(textureHandleOperationGamePadUI_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHTPSlide_[0].reset(Sprite::Create(textureHandleHTPSlide_[0], {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHTPSlide_[1].reset(Sprite::Create(textureHandleHTPSlide_[1], {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHTPSlide_[2].reset(Sprite::Create(textureHandleHTPSlide_[2], {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHTPSlide_[3].reset(Sprite::Create(textureHandleHTPSlide_[3], {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHTPSlide_[4].reset(Sprite::Create(textureHandleHTPSlide_[4], {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));

	// ボタンサイズ取得
	sizePlayButton_ = spritePlayButton_->GetSize();
	sizeHowToPlayButton_ = spriteHowToPlayButton_->GetSize();
	sizeExitButton_ = spriteExitButton_->GetSize();

	// ポジション
	positionPlayButton_ = spritePlayButton_->GetPosition();
	positionHowToPlayButton_ = spriteHowToPlayButton_->GetPosition();
	positionExitButton_ = spriteExitButton_->GetPosition();

	// 回転量取得
	rotationTitleRogo_ = spriteTitleRogo_->GetRotation();
}

TitleUI::~TitleUI() {
	// 音ストップ
	audio_->StopWave(voiceHandleCursorMoveSE_);
	audio_->StopWave(voiceHandleDecideSE_);
}

void TitleUI::Initialize() {
	// ボタンのサイズ初期化
	spritePlayButton_->SetSize(sizePlayButton_);
	spriteHowToPlayButton_->SetSize(sizeHowToPlayButton_);
	spriteExitButton_->SetSize(sizeExitButton_);
	// 回転量初期化
	spriteTitleRogo_->SetRotation(rotationTitleRogo_);

	// 変数
	titleSituation_ = TitleSituation::TITLEMODE;
	selectButton_ = TitleButton::PLAYBUTTONTITLE;
	isPlay_ = false;
	isExit_ = false;
	slidePage_ = 0;
	rotateSpeed_ = kRotateSpeed_;
	rotateAmount_ = rotationTitleRogo_;
}

void TitleUI::Update(Device device) {
	// 変数をリセット
	isPlay_ = false;
	isExit_ = false;
	device_ = device;

	// 状況毎の処理
	switch (titleSituation_) {
	case TITLEMODE:
		// タイトルロゴ回転処理
		rotateAmount_ += rotateSpeed_;
		if (rotateAmount_ > kRotateSpeed_ * 150) {
			rotateSpeed_ = -kRotateSpeed_;
		} else if (rotateAmount_ < -kRotateSpeed_ * 150) {
			rotateSpeed_ = kRotateSpeed_;
		}
		spriteTitleRogo_->SetRotation(rotateAmount_);

		// ボタンごとの処理
		switch (selectButton_) {
		case PLAYBUTTONTITLE:
			// スプライトサイズを大きくする
			spritePlayButton_->SetSize({sizePlayButton_.x * kStrngthButtonRatio_, sizePlayButton_.y * kStrngthButtonRatio_});
			// 選んでるやつがはねる処理
			if (spritePlayButton_->GetPosition().y >= positionPlayButton_.y) {
				velocity_.y = -kSlectJump_;
				position_.y = 0.0f;
			}
			// 重力加速度加算
			velocity_.y += -kGravity_;
			// 速度加算処理
			position_.y += velocity_.y;
			// スプライトの位置に反映
			spritePlayButton_->SetPosition({positionPlayButton_.x, positionPlayButton_.y + position_.y});
			// 選択変更
			if (input_->TriggerKey(DIK_DOWN) || pad_->TriggerLStickDOWN()) {
				selectButton_ = HOWTOPLAYBUTTONTITLE;
				// スプライトサイズもとに戻す
				spritePlayButton_->SetSize(sizePlayButton_);
				// スプライトのポジションもとに戻す
				spritePlayButton_->SetPosition(positionPlayButton_);
				// サウンド再生
				isSoundPlayCursorMoveSE_ = true;
			}
			// 選択
			if (input_->TriggerKey(DIK_SPACE) || pad_->TriggerA()) {
				isPlay_ = true;
				// サウンド再生
				isSoundPlayDecideSE_ = true;
			}
			break;
		case HOWTOPLAYBUTTONTITLE:
			// 選択変更
			// スプライトサイズを大きくする
			spriteHowToPlayButton_->SetSize({sizeHowToPlayButton_.x * kStrngthButtonRatio_, sizeHowToPlayButton_.y * kStrngthButtonRatio_});
			// 選んでるやつがはねる処理
			if (spriteHowToPlayButton_->GetPosition().y >= positionHowToPlayButton_.y) {
				velocity_.y = -kSlectJump_;
				position_.y = 0.0f;
			}
			// 重力加速度加算
			velocity_.y += -kGravity_;
			// 速度加算処理
			position_.y += velocity_.y;
			// スプライトの位置に反映
			spriteHowToPlayButton_->SetPosition({positionHowToPlayButton_.x, positionHowToPlayButton_.y + position_.y});
			if (input_->TriggerKey(DIK_UP) || pad_->TriggerLStickUP()) {
				selectButton_ = PLAYBUTTONTITLE;
				// スプライトサイズもとに戻す
				spriteHowToPlayButton_->SetSize(sizeHowToPlayButton_);
				// スプライトのポジションもとに戻す
				spriteHowToPlayButton_->SetPosition(positionHowToPlayButton_);
				// サウンド再生
				isSoundPlayCursorMoveSE_ = true;
			} else if (input_->TriggerKey(DIK_DOWN) || pad_->TriggerLStickDOWN()) {
				selectButton_ = EXITBUTTONTITLE;
				// スプライトサイズもとに戻す
				spriteHowToPlayButton_->SetSize(sizeHowToPlayButton_);
				// スプライトのポジションもとに戻す
				spriteHowToPlayButton_->SetPosition(positionHowToPlayButton_);
				// サウンド再生
				isSoundPlayCursorMoveSE_ = true;
			}
			// 選択
			if (input_->TriggerKey(DIK_SPACE) || pad_->TriggerA()) {
				titleSituation_ = HOWTOPLAYMODE;
				// サウンド再生
				isSoundPlayDecideSE_ = true;
			}
			break;
		case EXITBUTTONTITLE:
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
			if (input_->TriggerKey(DIK_UP) || pad_->TriggerLStickUP()) {
				selectButton_ = HOWTOPLAYBUTTONTITLE;
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
		default:
			break;
		}
		break;
	case HOWTOPLAYMODE:
		// 左右キーでページ切り替え
		if (input_->TriggerKey(DIK_RIGHT) || pad_->TriggerLStickRIGHT()) {
			// 0,1,2ページならインクリメント
			if (slidePage_ >= 0 && slidePage_ < kHowToPlayPageNum - 1) {
				slidePage_++;
				// サウンド再生
				isSoundPlayCursorMoveSE_ = true;
			}
			// 3ページならメニュー画面へ
			else if (slidePage_ == kHowToPlayPageNum - 1) {
				titleSituation_ = TitleSituation::TITLEMODE;
				// ページも0に戻す
				slidePage_ = 0;
				// サウンド再生
				isSoundPlayDecideSE_ = true;
			}
		} else if (input_->TriggerKey(DIK_LEFT) || pad_->TriggerLStickLEFT()) {
			// 1,2,3ページならデクリメント
			if (slidePage_ >= 1 && slidePage_ <= kHowToPlayPageNum - 1) {
				slidePage_--;
				// サウンド再生
				isSoundPlayCursorMoveSE_ = true;
			}
		}
		break;
	default:
		break;
	}
#ifdef _DEBUG
	if (pad_->PushA()) {
		ImGui::Begin("A");
		ImGui::End();
	}
	if (pad_->PushB()) {
		ImGui::Begin("B");
		ImGui::End();
	}
	if (pad_->PushX()) {
		ImGui::Begin("X");
		ImGui::End();
	}
	if (pad_->PushY()) {
		ImGui::Begin("Y");
		ImGui::End();
	}
	if (pad_->PushRStickButton()) {
		ImGui::Begin("RStickButton");
		ImGui::End();
	}
	if (pad_->PushLStickButton()) {
		ImGui::Begin("LStickButton");
		ImGui::End();
	}
	if (pad_->PushRB()) {
		ImGui::Begin("RB");
		ImGui::End();
	}
	if (pad_->PushLB()) {
		ImGui::Begin("LB");
		ImGui::End();
	}
	if (pad_->PushCrossUP()) {
		ImGui::Begin("CrossUP");
		ImGui::End();
	}
	if (pad_->PushCrossDOWN()) {
		ImGui::Begin("CrossDOWN");
		ImGui::End();
	}
	if (pad_->PushCrossRIGHT()) {
		ImGui::Begin("CrossRIGHT");
		ImGui::End();
	}
	if (pad_->PushCrossLEFT()) {
		ImGui::Begin("CrossLEFT");
		ImGui::End();
	}
	if (pad_->PushSTART()) {
		ImGui::Begin("START");
		ImGui::End();
	}
	if (pad_->PushBACK()) {
		ImGui::Begin("BACK");
		ImGui::End();
	}
	if (pad_->TiltLStickUP()) {
		ImGui::Begin("LstickUP");
		ImGui::End();
	}
	if (pad_->TiltLStickUPRIGHT()) {
		ImGui::Begin("LstickUPRIGHT");
		ImGui::End();
	}
	if (pad_->TiltLStickRIGHT()) {
		ImGui::Begin("LstickRIGHT");
		ImGui::End();
	}
	if (pad_->TiltLStickDOWNRIGHT()) {
		ImGui::Begin("LstickDOWNRIGHT");
		ImGui::End();
	}
	if (pad_->TiltLStickDOWN()) {
		ImGui::Begin("LstickDOWN");
		ImGui::End();
	}
	if (pad_->TiltLStickDOWNLEFT()) {
		ImGui::Begin("LstickDOWNLEFT");
		ImGui::End();
	}
	if (pad_->TiltLStickLEFT()) {
		ImGui::Begin("LstickLEFT");
		ImGui::End();
	}
	if (pad_->TiltLStickUPLEFT()) {
		ImGui::Begin("LstickUPLEFT");
		ImGui::End();
	}

	if (pad_->TriggerRStickUP()) {
		ImGui::Begin("RstickUP");
		ImGui::End();
	}
	if (pad_->TriggerRStickUPRIGHT()) {
		ImGui::Begin("RstickUPRIGHT");
		ImGui::End();
	}
	if (pad_->TriggerRStickRIGHT()) {
		ImGui::Begin("RstickRIGHT");
		ImGui::End();
	}
	if (pad_->TriggerRStickDOWNRIGHT()) {
		ImGui::Begin("RstickDOWNRIGHT");
		ImGui::End();
	}
	if (pad_->TriggerRStickDOWN()) {
		ImGui::Begin("RstickDOWN");
		ImGui::End();
	}
	if (pad_->TriggerRStickDOWNLEFT()) {
		ImGui::Begin("RstickDOWNLEFT");
		ImGui::End();
	}
	if (pad_->TriggerRStickLEFT()) {
		ImGui::Begin("RstickLEFT");
		ImGui::End();
	}
	if (pad_->TriggerRStickUPLEFT()) {
		ImGui::Begin("RstickUPLEFT");
		ImGui::End();
	}

#endif // _DEBUG
}

void TitleUI::Draw() {}

void TitleUI::DrawUI() {
	// ホワイトバックは常に一番前
	spriteBackWhite_->Draw();
	switch (titleSituation_) {
	case TITLEMODE:
		spriteTitleRogo_->Draw();
		spritePlayButton_->Draw();
		spriteHowToPlayButton_->Draw();
		spriteExitButton_->Draw();
		if (device_ == Device::KEYBOARD) {
			spriteOperationKeyBoardUI_->Draw();
		} else if (device_ == Device::GAMEPAD) {
			spriteOperationGamePadUI_->Draw();
		}
		break;
	case HOWTOPLAYMODE:
		spriteHTPSlide_[slidePage_]->Draw();
		break;
	default:
		break;
	}
}

void TitleUI::AudioPlay() {
	if (isSoundPlayCursorMoveSE_) {
		voiceHandleCursorMoveSE_ = audio_->PlayWave(soundHandleCursorMoveSE_, false, soundVolumeCursorMoveSE_);
		isSoundPlayCursorMoveSE_ = false;
	}
	if (isSoundPlayDecideSE_) {
		voiceHandleDecideSE_ = audio_->PlayWave(soundHandleDecideSE_, false, soundVolumeDecideSE_);
		isSoundPlayDecideSE_ = false;
	}
}
