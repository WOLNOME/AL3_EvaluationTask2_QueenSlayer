#pragma once
#include "BaseScene.h"
#include "Input.h"
#include "GamePad.h"
#include "Audio.h"
#include "Player.h"
#include "Ground.h"
#include "Skydome.h"
#include "ResultUI.h"
#include "Particle.h"
#include "Model.h"
#include "Sprite.h"
#include "ResultCamera.h"
#include "ViewProjection.h"

class ResultScene : public BaseScene {
public:
	ResultScene();
	~ResultScene() override;

public:
	void Init(Input* input, Audio* audio, GamePad* pad) override;
	void Update(Device device) override;
	void Draw(ID3D12GraphicsCommandList* commandList, DirectXCommon* dxCommon_) override;

public:
	SCENE GetNextScene() override { return NextScene; }
	bool GetIsExit() override { return isExit; }

private:
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	GamePad* pad_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransformParticle_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// モデルテクスチャ
	uint32_t textureHandleGround_;
	uint32_t textureHandleSkydome_;
	/// 2Dスプライト

	/// インスタンス
	std::unique_ptr<ResultCamera> resultCamera_ = nullptr;
	std::unique_ptr<Ground> ground_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;
	std::unique_ptr<Player> player_ = nullptr;
	std::unique_ptr<ResultUI> resultUI_ = nullptr;
	std::unique_ptr<Particle> particle_ = nullptr;

private:
	int timer_ = 0;

	// BGM関連
	uint32_t soundHandleBGM_;
	uint32_t voiceHandleBGM_;
	bool isSoundPlayBGM_ = false;
	const float soundVolumeBGM_ = 0.7f;
};
