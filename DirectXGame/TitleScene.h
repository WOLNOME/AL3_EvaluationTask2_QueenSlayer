#pragma once
#include "Audio.h"
#include "Background.h"
#include "BaseScene.h"
#include "Enemy.h"
#include "Ground.h"
#include "Input.h"
#include "Model.h"
#include "Skydome.h"
#include "Sprite.h"
#include "TitleCamera.h"
#include "TitleUI.h"
#include "ViewProjection.h"

class TitleScene : public BaseScene {
public:
	TitleScene();
	~TitleScene() override;

public:
	void Init(Input* input, Audio* audio) override;
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* commandList, DirectXCommon* dxCommon_) override;

public: // ゲッター
	SCENE GetNextScene() override { return NextScene; }
	bool GetIsExit() override { return isExit; }

public: // セッター
private:
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	/// 3Dモデル
	std::unique_ptr<Model> modelSkydome_ = nullptr;
	std::unique_ptr<Model> modelGround_ = nullptr;

	/// 2Dスプライト

	/// インスタンス
	std::unique_ptr<TitleCamera> titleCamera_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;
	std::unique_ptr<Ground> ground_ = nullptr;
	std::unique_ptr<Background> background_ = nullptr;
	std::unique_ptr<Enemy> enemy_ = nullptr;
	std::unique_ptr<TitleUI> titleUI_ = nullptr;


private: // 変数系
	int timer_;

	// BGM関連
	uint32_t soundHandleBGM_;
	uint32_t voiceHandleBGM_;
	bool isSoundPlayBGM_ = false;
	const float soundVolumeBGM_ = 1.0f;
};
