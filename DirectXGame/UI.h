#pragma once
#include "Sprite.h"
#include "Function.h"
#include "memory"

class StageScene;

class UI {
public:
	UI();
	~UI();

	void Initialize();
	void Update();
	void Draw();

public://セッター
	void SetStageScene(StageScene* stageScene) { stageScene_ = stageScene; }

private:
	//ステージシーン
	StageScene* stageScene_ = nullptr;

	// テクスチャハンドル
	uint32_t textureEnemyGreenHP_;
	uint32_t textureEnemyFrameHP_;
	uint32_t textureEnemyName_;
	uint32_t texturePlayerGreenHP_;
	uint32_t texturePlayerFrameHP_;
	uint32_t texturePlayerYellowSP_;
	uint32_t texturePlayerFrameSP_;
	uint32_t textureDefaultUI_;

	// スプライト
	std::unique_ptr<Sprite> spriteEnemyGreenHP_ = nullptr;
	std::unique_ptr<Sprite> spriteEnemyFrameHP_ = nullptr;
	std::unique_ptr<Sprite> spriteEnemyName_ = nullptr;
	std::unique_ptr<Sprite> spritePlayerGreenHP_ = nullptr;
	std::unique_ptr<Sprite> spritePlayerFrameHP_ = nullptr;
	std::unique_ptr<Sprite> spritePlayerYellowSP_ = nullptr;
	std::unique_ptr<Sprite> spritePlayerFrameSP_ = nullptr;
	std::unique_ptr<Sprite> spriteDefaultUI_ = nullptr;
	
private:
	//敵のHPバーサイズ
	Vector2 enemyGreenHPSize_;
	//自機のHPバーサイズ
	Vector2 playerGreenHPSize_;
	//自機のSPバーサイズ
	Vector2 playerYellowSPSize_;

};
