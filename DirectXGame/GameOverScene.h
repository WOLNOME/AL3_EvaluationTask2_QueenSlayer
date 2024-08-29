#pragma once
#include "BaseScene.h"
#include "Input.h"
#include "Audio.h"
#include "Player.h"
#include "Ground.h"
#include "Skydome.h"
#include "Spotlight.h"
#include "Model.h"
#include "Sprite.h"
#include "GameOverCamera.h"
#include "ViewProjection.h"

class GameOverScene : public BaseScene {
public:
	GameOverScene();
	~GameOverScene() override;

public:
	void Init(Input* input, Audio* audio) override;
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* commandList, DirectXCommon* dxCommon_) override;

public:
	SCENE GetNextScene() override { return NextScene; }
	bool GetIsExit() override { return isExit; }

private:
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// モデルテクスチャ
	uint32_t textureHandleGround_;
	uint32_t textureHandleSkydome_;
	/// 2Dスプライト

	/// インスタンス
	std::unique_ptr<GameOverCamera> gameOverCamera_ = nullptr;
	std::unique_ptr<Ground> ground_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;
	std::unique_ptr<Spotlight> spotlight_ = nullptr;
	std::unique_ptr<Player> player_ = nullptr;

private://変数

	// BGM関連
	uint32_t soundHandleBGM_;
	uint32_t voiceHandleBGM_;
	bool isSoundPlayBGM_ = false;
	const float soundVolumeBGM_ = 0.6f;

};
