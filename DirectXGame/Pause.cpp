#include "Pause.h"
#include "TextureManager.h"
#include "WinApp.h"

Pause::Pause(Input* input,Audio* audio) {
	// インプット
	input_ = input;
	// オーディオ
	audio_ = audio;

	// サウンドハンドル
	soundHandleCursorMoveSE_ = audio_->LoadWave("Audio/cursorMoveSE.wav");
	soundHandleDecideSE_ = audio_->LoadWave("Audio/decideSE.wav");

	// ファイル名をしていしてテクスチャを読み込む
	textureHandleContinueButton_ = TextureManager::Load("Pause/continueButton.png");
	textureHandleHowToPlayButton_ = TextureManager::Load("Pause/howToPlayButton.png");
	textureHandleBackToTitleButton_ = TextureManager::Load("Pause/backToTitleButton.png");
	textureHandleBackBlack_ = TextureManager::Load("black.png");
	textureHandlePauseMark_ = TextureManager::Load("Pause/pauseMark.png");
	textureHandleOperationUI_ = TextureManager::Load("Pause/operationUI.png");
	textureHandleHTPSlide_[0] = TextureManager::Load("Pause/howToPlayPage0.png");
	textureHandleHTPSlide_[1] = TextureManager::Load("Pause/howToPlayPage1.png");
	textureHandleHTPSlide_[2] = TextureManager::Load("Pause/howToPlayPage2.png");
	textureHandleHTPSlide_[3] = TextureManager::Load("Pause/howToPlayPage3.png");

	// スプライト生成
	spriteContinueButton_.reset(Sprite::Create(textureHandleContinueButton_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHowToPlayButton_.reset(Sprite::Create(textureHandleHowToPlayButton_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f + 100.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteBackToTitleButton_.reset(Sprite::Create(textureHandleBackToTitleButton_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f + 200.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteBackBlack_.reset(Sprite::Create(textureHandleBackBlack_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, kBlackBackAlpha_), {0.5f, 0.5f}));
	spritePauseMark_.reset(Sprite::Create(textureHandlePauseMark_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f - 180.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteOperationUI_.reset(Sprite::Create(textureHandleOperationUI_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHTPSlide_[0].reset(Sprite::Create(textureHandleHTPSlide_[0], {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHTPSlide_[1].reset(Sprite::Create(textureHandleHTPSlide_[1], {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHTPSlide_[2].reset(Sprite::Create(textureHandleHTPSlide_[2], {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHTPSlide_[3].reset(Sprite::Create(textureHandleHTPSlide_[3], {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));

	// ボタンサイズ取得
	sizeContinueButton_ = spriteContinueButton_->GetSize();
	sizeHowToPlayButton_ = spriteHowToPlayButton_->GetSize();
	sizeBackToTitleButton_ = spriteBackToTitleButton_->GetSize();

	// ポジション
	positionContinueButton_ = spriteContinueButton_->GetPosition();
	positionHowToPlayButton_ = spriteHowToPlayButton_->GetPosition();
	positionBackToTitleButton_ = spriteBackToTitleButton_->GetPosition();
}

Pause::~Pause() {
	//音ストップ
	audio_->StopWave(voiceHandleCursorMoveSE_);
	audio_->StopWave(voiceHandleDecideSE_);
}

void Pause::Initialize() {
	// ボタンのサイズ初期化
	spriteContinueButton_->SetSize(sizeContinueButton_);
	spriteHowToPlayButton_->SetSize(sizeHowToPlayButton_);
	spriteBackToTitleButton_->SetSize(sizeBackToTitleButton_);
	// 変数
	pauseSituation_ = PauseSituation::MENU;
	selectButton_ = MenuButton::CONTINUEBUTTON;
	isContinue_ = false;
	isExit_ = false;
	slidePage_ = 0;
}

void Pause::Update() {
	// 状況毎の処理
	switch (pauseSituation_) {
	case MENU:
		// ボタンごとの処理
		switch (selectButton_) {
		case CONTINUEBUTTON:
			// スプライトサイズを大きくする
			spriteContinueButton_->SetSize({sizeContinueButton_.x * kStrngthButtonRatio_, sizeContinueButton_.y * kStrngthButtonRatio_});
			// 選んでるやつがはねる処理
			if (spriteContinueButton_->GetPosition().y >= positionContinueButton_.y) {
				velocity_.y = -kSlectJump_;
				position_.y = 0.0f;
			}
			// 重力加速度加算
			velocity_.y += -kGravity_;
			// 速度加算処理
			position_.y += velocity_.y;
			// スプライトの位置に反映
			spriteContinueButton_->SetPosition({positionContinueButton_.x, positionContinueButton_.y + position_.y});
			// 選択変更
			if (input_->TriggerKey(DIK_DOWN)) {
				selectButton_ = HOWTOPLAYBUTTON;
				// スプライトサイズもとに戻す
				spriteContinueButton_->SetSize(sizeContinueButton_);
				// スプライトのポジションもとに戻す
				spriteContinueButton_->SetPosition(positionContinueButton_);
				// サウンド再生
				isSoundPlayCursorMoveSE_ = true;
			}
			// 選択
			if (input_->TriggerKey(DIK_SPACE)) {
				isContinue_ = true;
				// サウンド再生
				isSoundPlayDecideSE_ = true;
			}
			break;
		case HOWTOPLAYBUTTON:
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
			if (input_->TriggerKey(DIK_UP)) {
				selectButton_ = CONTINUEBUTTON;
				// スプライトサイズもとに戻す
				spriteHowToPlayButton_->SetSize(sizeHowToPlayButton_);
				// スプライトのポジションもとに戻す
				spriteHowToPlayButton_->SetPosition(positionHowToPlayButton_);
				// サウンド再生
				isSoundPlayCursorMoveSE_ = true;
			} else if (input_->TriggerKey(DIK_DOWN)) {
				selectButton_ = BACKTOTITLEBUTTON;
				// スプライトサイズもとに戻す
				spriteHowToPlayButton_->SetSize(sizeHowToPlayButton_);
				// スプライトのポジションもとに戻す
				spriteHowToPlayButton_->SetPosition(positionHowToPlayButton_);
				// サウンド再生
				isSoundPlayCursorMoveSE_ = true;
			}
			// 選択
			if (input_->TriggerKey(DIK_SPACE)) {
				pauseSituation_ = HOWTOPLAY;
				// サウンド再生
				isSoundPlayDecideSE_ = true;
			}
			break;
		case BACKTOTITLEBUTTON:
			// スプライトサイズを大きくする
			spriteBackToTitleButton_->SetSize({sizeBackToTitleButton_.x * kStrngthButtonRatio_, sizeBackToTitleButton_.y * kStrngthButtonRatio_});
			// 選んでるやつがはねる処理
			if (spriteBackToTitleButton_->GetPosition().y >= positionBackToTitleButton_.y) {
				velocity_.y = -kSlectJump_;
				position_.y = 0.0f;
			}
			// 重力加速度加算
			velocity_.y += -kGravity_;
			// 速度加算処理
			position_.y += velocity_.y;
			// スプライトの位置に反映
			spriteBackToTitleButton_->SetPosition({positionBackToTitleButton_.x, positionBackToTitleButton_.y + position_.y});
			// 選択変更
			if (input_->TriggerKey(DIK_UP)) {
				selectButton_ = HOWTOPLAYBUTTON;
				// スプライトサイズもとに戻す
				spriteBackToTitleButton_->SetSize(sizeBackToTitleButton_);
				// スプライトのポジションもとに戻す
				spriteBackToTitleButton_->SetPosition(positionBackToTitleButton_);
				// サウンド再生
				isSoundPlayCursorMoveSE_ = true;
			}
			// 選択
			if (input_->TriggerKey(DIK_SPACE)) {
				isExit_ = true;
				// サウンド再生
				isSoundPlayDecideSE_ = true;
			}
			break;
		default:
			break;
		}
		break;
	case HOWTOPLAY:
		// 左右キーでページ切り替え
		if (input_->TriggerKey(DIK_RIGHT)) {
			// 0,1,2ページならインクリメント
			if (slidePage_ >= 0 && slidePage_ < 3) {
				slidePage_++;
				// サウンド再生
				isSoundPlayCursorMoveSE_ = true;
			}
			// 3ページならメニュー画面へ
			else if (slidePage_ == 3) {
				pauseSituation_ = MENU;
				// ページも0に戻す
				slidePage_ = 0;
				// サウンド再生
				isSoundPlayDecideSE_ = true;
			}
		} else if (input_->TriggerKey(DIK_LEFT)) {
			// 1,2,3ページならデクリメント
			if (slidePage_ >= 1 && slidePage_ <= 3) {
				slidePage_--;
				// サウンド再生
				isSoundPlayCursorMoveSE_ = true;
			}
		}
		break;
	default:
		break;
	}
}

void Pause::Draw() {}

void Pause::DrawUI() {
	// ブラックバックは常に一番前
	spriteBackBlack_->Draw();
	switch (pauseSituation_) {
	case MENU:
		spritePauseMark_->Draw();
		spriteContinueButton_->Draw();
		spriteHowToPlayButton_->Draw();
		spriteBackToTitleButton_->Draw();
		spriteOperationUI_->Draw();
		break;
	case HOWTOPLAY:
		spriteHTPSlide_[slidePage_]->Draw();
		break;
	default:
		break;
	}
}

void Pause::AudioPlay() {
	if (isSoundPlayCursorMoveSE_) {
		voiceHandleCursorMoveSE_ = audio_->PlayWave(soundHandleCursorMoveSE_, false, soundVolumeCursorMoveSE_);
		isSoundPlayCursorMoveSE_ = false;
	}
	if (isSoundPlayDecideSE_) {
		voiceHandleDecideSE_ = audio_->PlayWave(soundHandleDecideSE_, false, soundVolumeDecideSE_);
		isSoundPlayDecideSE_ = false;
	}
}
