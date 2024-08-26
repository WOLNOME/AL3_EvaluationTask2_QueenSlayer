#include "UI.h"
#include "StageScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

UI::UI() {}

UI::~UI() {}

void UI::Initialize() {
	// ファイル名を指定してテクスチャを読み込む
	textureEnemyGreenHP_ = TextureManager::Load("UI/EnemyGreenHP.png");
	textureEnemyFrameHP_ = TextureManager::Load("UI/EnemyFrameHP.png");
	textureEnemyName_ = TextureManager::Load("UI/EnemyName.png");
	texturePlayerGreenHP_ = TextureManager::Load("UI/PlayerGreenHP.png");
	texturePlayerFrameHP_ = TextureManager::Load("UI/PlayerFrameHP.png");
	texturePlayerYellowSP_ = TextureManager::Load("UI/PlayerYellowSP.png");
	texturePlayerMaxSP_ = TextureManager::Load("UI/PlayerMaxSP.png");
	texturePlayerFrameSP_ = TextureManager::Load("UI/PlayerFrameSP.png");
	textureDefaultUI_ = TextureManager::Load("UI/DefaultUI.png");
	textureInductionSP_ = TextureManager::Load("UI/inductionSPUI.png");

	// 2Dスプライト
	spriteEnemyGreenHP_.reset(Sprite::Create(textureEnemyGreenHP_, {WinApp::kWindowWidth / 2.0f, 85}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteEnemyFrameHP_.reset(Sprite::Create(textureEnemyFrameHP_, {WinApp::kWindowWidth / 2.0f, 85}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteEnemyName_.reset(Sprite::Create(textureEnemyName_, {WinApp::kWindowWidth / 2.0f, 35}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spritePlayerGreenHP_.reset(Sprite::Create(texturePlayerGreenHP_, {WinApp::kWindowWidth / 2.0f, 475}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spritePlayerFrameHP_.reset(Sprite::Create(texturePlayerFrameHP_, {WinApp::kWindowWidth / 2.0f, 475}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spritePlayerYellowSP_.reset(Sprite::Create(texturePlayerYellowSP_, {WinApp::kWindowWidth / 2.0f, 485}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spritePlayerMaxSP_.reset(Sprite::Create(texturePlayerMaxSP_, {WinApp::kWindowWidth / 2.0f, 485}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spritePlayerFrameSP_.reset(Sprite::Create(texturePlayerFrameSP_, {WinApp::kWindowWidth / 2.0f, 485}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteDefaultUI_.reset(Sprite::Create(textureDefaultUI_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	spriteInductionSP_.reset(Sprite::Create(textureInductionSP_, {WinApp::kWindowWidth / 2.0f, 575}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));

	//サイズ取得
	enemyGreenHPSize_ = spriteEnemyGreenHP_->GetSize();
	playerGreenHPSize_ = spritePlayerGreenHP_->GetSize();
	playerYellowSPSize_ = spritePlayerYellowSP_->GetSize();
}

void UI::Update() {
	//敵のHPに応じて体力バーを縮める処理
	Vector2 newEnemyGreenHPSize = enemyGreenHPSize_;
	newEnemyGreenHPSize.x *= ((float)stageScene_->GetEnemy()->GetHP() / stageScene_->GetEnemy()->kMaxHP_);
	newEnemyGreenHPSize.x = max(newEnemyGreenHPSize.x, 0.0f);
	newEnemyGreenHPSize.x = min(newEnemyGreenHPSize.x, enemyGreenHPSize_.x);
	spriteEnemyGreenHP_->SetSize(newEnemyGreenHPSize);
	// 自機のHPに応じて体力バーを縮める処理
	Vector2 newPlayerGreenHPSize = playerGreenHPSize_;
	newPlayerGreenHPSize.x *= ((float)stageScene_->GetPlayer()->GetHP() / stageScene_->GetPlayer()->kMaxHP_);
	newPlayerGreenHPSize.x = max(newPlayerGreenHPSize.x, 0.0f);
	newPlayerGreenHPSize.x = min(newPlayerGreenHPSize.x, playerGreenHPSize_.x);
	spritePlayerGreenHP_->SetSize(newPlayerGreenHPSize);
	// 自機のSPに応じてSPバーを拡大する処理
	Vector2 newPlayerYellowSPSize = playerYellowSPSize_;
	newPlayerYellowSPSize.x *= ((float)stageScene_->GetPlayer()->GetSPGauge() / stageScene_->GetPlayer()->kMaxSPGauge_);
	newPlayerYellowSPSize.x = max(newPlayerYellowSPSize.x, 0.0f);
	newPlayerYellowSPSize.x = min(newPlayerYellowSPSize.x, playerYellowSPSize_.x);
	spritePlayerYellowSP_->SetSize(newPlayerYellowSPSize);
	//SP誘導UI点滅処理
	blinkingTimer_++;
	if (blinkingTimer_ % kBlinkingTimeInductionSP_ == 0) {
		if (isDisplayInductionSP_) {
			isDisplayInductionSP_ = false;
		} else {
			isDisplayInductionSP_ = true;
		}
	}
	if (blinkingTimer_ == kBlinkingTimeInductionSP_ * 100) {
		blinkingTimer_ = 0;
	}

}

void UI::Draw() { 
	//下に行くほど優先
	spriteEnemyFrameHP_->Draw();
	spriteEnemyGreenHP_->Draw();
	spriteEnemyName_->Draw();
	spritePlayerFrameHP_->Draw();
	spritePlayerGreenHP_->Draw();
	spritePlayerFrameSP_->Draw();
	//playerSPゲージによって
	if (stageScene_->GetPlayer()->GetSPGauge() >= stageScene_->GetPlayer()->kMaxSPGauge_) {
		spritePlayerMaxSP_->Draw();
		//SP誘導UI
		if (isDisplayInductionSP_) {
			spriteInductionSP_->Draw();
		}

	} else {
		spritePlayerYellowSP_->Draw();
	}
	spriteDefaultUI_->Draw();
}
