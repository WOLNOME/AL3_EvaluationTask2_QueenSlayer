#include "Pause.h"
#include "TextureManager.h"
#include "WinApp.h"

Pause::Pause(Input* input) {
	// インプット
	input_ = input;

	// ファイル名をしていしてテクスチャを読み込む
	textureHandleContinueButton_ = TextureManager::Load("Pause/continueButton.png");
	textureHandleHowToPlayButton_ = TextureManager::Load("Pause/howToPlayButton.png");
	textureHandleBackToTitleButton_ = TextureManager::Load("Pause/backToTitleButton.png");
	textureHandleBackBlack_ = TextureManager::Load("black.png");
	textureHandlePauseMark_ = TextureManager::Load("Pause/pauseMark.png");
	textureHandleHTPSlide_[0] = TextureManager::Load("Pause/howToPlayPage0.png");
	textureHandleHTPSlide_[1] = TextureManager::Load("Pause/howToPlayPage1.png");
	textureHandleHTPSlide_[2] = TextureManager::Load("Pause/howToPlayPage2.png");
	textureHandleHTPSlide_[3] = TextureManager::Load("Pause/howToPlayPage3.png");

	// スプライト生成
	spriteContinueButton_.reset(Sprite::Create(textureHandleContinueButton_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHowToPlayButton_.reset(Sprite::Create(textureHandleHowToPlayButton_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f + 100.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteBackToTitleButton_.reset(Sprite::Create(textureHandleBackToTitleButton_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f + 200.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteBackBlack_.reset(Sprite::Create(textureHandleBackBlack_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, kBlackBackAlpha_), {0.5f, 0.5f}));
	spritePauseMark_.reset(Sprite::Create(textureHandlePauseMark_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f - 150.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHTPSlide_[0].reset(Sprite::Create(textureHandleHTPSlide_[0], {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHTPSlide_[1].reset(Sprite::Create(textureHandleHTPSlide_[1], {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHTPSlide_[2].reset(Sprite::Create(textureHandleHTPSlide_[2], {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHTPSlide_[3].reset(Sprite::Create(textureHandleHTPSlide_[3], {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));

	// ボタンサイズ取得
	sizeContinueButton_ = spriteContinueButton_->GetSize();
	sizeHowToPlayButton_ = spriteHowToPlayButton_->GetSize();
	sizeBackToTitleButton_ = spriteBackToTitleButton_->GetSize();
}

Pause::~Pause() {}

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
			// 選択変更
			if (input_->TriggerKey(DIK_DOWN)) {
				selectButton_ = HOWTOPLAYBUTTON;
				// スプライトサイズもとに戻す
				spriteContinueButton_->SetSize(sizeContinueButton_);
			}
			// 選択
			if (input_->TriggerKey(DIK_SPACE)) {
				isContinue_ = true;
			}
			break;
		case HOWTOPLAYBUTTON:
			// 選択変更
			// スプライトサイズを大きくする
			spriteHowToPlayButton_->SetSize({sizeHowToPlayButton_.x * kStrngthButtonRatio_, sizeHowToPlayButton_.y * kStrngthButtonRatio_});
			if (input_->TriggerKey(DIK_UP)) {
				selectButton_ = CONTINUEBUTTON;
				// スプライトサイズもとに戻す
				spriteHowToPlayButton_->SetSize(sizeHowToPlayButton_);
			} else if (input_->TriggerKey(DIK_DOWN)) {
				selectButton_ = BACKTOTITLEBUTTON;
				// スプライトサイズもとに戻す
				spriteHowToPlayButton_->SetSize(sizeHowToPlayButton_);
			}
			// 選択
			if (input_->TriggerKey(DIK_SPACE)) {
				pauseSituation_ = HOWTOPLAY;
			}
			break;
		case BACKTOTITLEBUTTON:
			// スプライトサイズを大きくする
			spriteBackToTitleButton_->SetSize({sizeBackToTitleButton_.x * kStrngthButtonRatio_, sizeBackToTitleButton_.y * kStrngthButtonRatio_});
			// 選択変更
			if (input_->TriggerKey(DIK_UP)) {
				selectButton_ = HOWTOPLAYBUTTON;
				// スプライトサイズもとに戻す
				spriteBackToTitleButton_->SetSize(sizeBackToTitleButton_);
			}
			// 選択
			if (input_->TriggerKey(DIK_SPACE)) {
				isExit_ = true;
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
			}
			// 3ページならメニュー画面へ
			else if (slidePage_ == 3) {
				pauseSituation_ = MENU;
				// ページも0に戻す
				slidePage_ = 0;
			}
		} else if (input_->TriggerKey(DIK_LEFT)) {
			// 1,2,3ページならデクリメント
			if (slidePage_ >= 1 && slidePage_ <= 3) {
				slidePage_--;
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
		break;
	case HOWTOPLAY:
		spriteHTPSlide_[slidePage_]->Draw();
		break;
	default:
		break;
	}
}
