#pragma once
#include "Audio.h"
#include "Function.h"
#include "Input.h"
#include "Sprite.h"
#include "memory"

// ゲームオーバー画面にあるボタン
enum GameOverButton {
	RETRYBUTTONGAMEOVER,
	RETIREBUTTONGAMEOVER,
};

class GameOverUI {
public:
	GameOverUI(Input* input, Audio* audio);
	~GameOverUI();

	void Initialize();
	void Update();
	void Draw();
	void DrawUI();
	void AudioPlay();

public: // 関数
public: // ゲッター
	bool GetIsRetry() { return isRetry_; }
	bool GetIsRetire() { return isRetire_; }

public: // セッター
private:
	// インプット
	Input* input_ = nullptr;
	// オーディオ
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandleRetryButton_;
	uint32_t textureHandleRetireButton_;
	uint32_t textureHandleGameOver_;
	uint32_t textureHandleOperationUI_;

	// スプライト
	std::unique_ptr<Sprite> spriteRetryButton_;
	std::unique_ptr<Sprite> spriteRetireButton_;
	std::unique_ptr<Sprite> spriteGameOver_;
	std::unique_ptr<Sprite> spriteOperationUI_;

	// サイズ
	Vector2 sizeRetryButton_;
	Vector2 sizeRetireButton_;

	// ポジション
	Vector2 positionRetryButton_;
	Vector2 positionRetireButton_;

private:
	GameOverButton selectButton_;
	bool isRetry_;
	bool isRetire_;
	const float kStrngthButtonRatio_ = 1.2f;

	const float kSlectJump_ = 2.0f;
	const float kGravity_ = -0.1f;
	Vector2 position_ = {0.0f, 0.0f};
	Vector2 velocity_ = {0.0f, 0.0f};

private:
	// BGM関連
	uint32_t soundHandleCursorMoveSE_;
	uint32_t voiceHandleCursorMoveSE_;
	bool isSoundPlayCursorMoveSE_ = false;
	const float soundVolumeCursorMoveSE_ = 0.4f;

	uint32_t soundHandleDecideSE_;
	uint32_t voiceHandleDecideSE_;
	bool isSoundPlayDecideSE_ = false;
	const float soundVolumeDecideSE_ = 0.8f;
};
