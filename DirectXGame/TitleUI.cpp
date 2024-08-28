#include "TitleUI.h"
#include "TextureManager.h"
#include "WinApp.h"

TitleUI::TitleUI(Input* input) {
	// インプット
	input_ = input;

	// ファイル名をしていしてテクスチャを読み込む
	textureHandlePlayButton_ = TextureManager::Load("Title/play!Button.png");
	textureHandleHowToPlayButton_ = TextureManager::Load("Title/howToPlayButton.png");
	textureHandleExitButton_ = TextureManager::Load("Title/exitButton.png");
	textureHandleBackWhite_ = TextureManager::Load("white.png");
	textureHandleTitleRogo_ = TextureManager::Load("Title/titleRogo.png");
	textureHandleHTPSlide_[0] = TextureManager::Load("Title/howToPlayPage0.png");
	textureHandleHTPSlide_[1] = TextureManager::Load("Title/howToPlayPage1.png");
	textureHandleHTPSlide_[2] = TextureManager::Load("Title/howToPlayPage2.png");
	textureHandleHTPSlide_[3] = TextureManager::Load("Title/howToPlayPage3.png");

	// スプライト生成
	spritePlayButton_.reset(Sprite::Create(textureHandlePlayButton_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHowToPlayButton_.reset(Sprite::Create(textureHandleHowToPlayButton_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f + 100.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteExitButton_.reset(Sprite::Create(textureHandleExitButton_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f + 200.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteBackWhite_.reset(Sprite::Create(textureHandleBackWhite_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, kWhiteBackAlpha_), {0.5f, 0.5f}));
	spriteTitleRogo_.reset(Sprite::Create(textureHandleTitleRogo_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f - 180.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHTPSlide_[0].reset(Sprite::Create(textureHandleHTPSlide_[0], {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHTPSlide_[1].reset(Sprite::Create(textureHandleHTPSlide_[1], {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHTPSlide_[2].reset(Sprite::Create(textureHandleHTPSlide_[2], {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteHTPSlide_[3].reset(Sprite::Create(textureHandleHTPSlide_[3], {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));

	// ボタンサイズ取得
	sizePlayButton_ = spritePlayButton_->GetSize();
	sizeHowToPlayButton_ = spriteHowToPlayButton_->GetSize();
	sizeExitButton_ = spriteExitButton_->GetSize();
	//回転量取得
	rotationTitleRogo_ = spriteTitleRogo_->GetRotation();

}

TitleUI::~TitleUI() {}

void TitleUI::Initialize() {
	// ボタンのサイズ初期化
	spritePlayButton_->SetSize(sizePlayButton_);
	spriteHowToPlayButton_->SetSize(sizeHowToPlayButton_);
	spriteExitButton_->SetSize(sizeExitButton_);
	//回転量初期化
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

void TitleUI::Update() {
	//変数をリセット
	isPlay_ = false;
	isExit_ = false;

	// 状況毎の処理
	switch (titleSituation_) {
	case TITLEMODE:
		//タイトルロゴ回転処理
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
			// 選択変更
			if (input_->TriggerKey(DIK_DOWN)) {
				selectButton_ = HOWTOPLAYBUTTONTITLE;
				// スプライトサイズもとに戻す
				spritePlayButton_->SetSize(sizePlayButton_);
			}
			// 選択
			if (input_->TriggerKey(DIK_SPACE)) {
				isPlay_ = true;
			}
			break;
		case HOWTOPLAYBUTTONTITLE:
			// 選択変更
			// スプライトサイズを大きくする
			spriteHowToPlayButton_->SetSize({sizeHowToPlayButton_.x * kStrngthButtonRatio_, sizeHowToPlayButton_.y * kStrngthButtonRatio_});
			if (input_->TriggerKey(DIK_UP)) {
				selectButton_ = PLAYBUTTONTITLE;
				// スプライトサイズもとに戻す
				spriteHowToPlayButton_->SetSize(sizeHowToPlayButton_);
			} else if (input_->TriggerKey(DIK_DOWN)) {
				selectButton_ = EXITBUTTONTITLE;
				// スプライトサイズもとに戻す
				spriteHowToPlayButton_->SetSize(sizeHowToPlayButton_);
			}
			// 選択
			if (input_->TriggerKey(DIK_SPACE)) {
				titleSituation_ = HOWTOPLAYMODE;
			}
			break;
		case EXITBUTTONTITLE:
			// スプライトサイズを大きくする
			spriteExitButton_->SetSize({sizeExitButton_.x * kStrngthButtonRatio_, sizeExitButton_.y * kStrngthButtonRatio_});
			// 選択変更
			if (input_->TriggerKey(DIK_UP)) {
				selectButton_ = HOWTOPLAYBUTTONTITLE;
				// スプライトサイズもとに戻す
				spriteExitButton_->SetSize(sizeExitButton_);
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
	case HOWTOPLAYMODE:
		// 左右キーでページ切り替え
		if (input_->TriggerKey(DIK_RIGHT)) {
			// 0,1,2ページならインクリメント
			if (slidePage_ >= 0 && slidePage_ < 3) {
				slidePage_++;
			}
			// 3ページならメニュー画面へ
			else if (slidePage_ == 3) {
				titleSituation_ = TitleSituation::TITLEMODE;
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
		break;
	case HOWTOPLAYMODE:
		spriteHTPSlide_[slidePage_]->Draw();
		break;
	default:
		break;
	}
}
