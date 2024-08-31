#pragma once
#include "Audio.h"
#include "Function.h"
#include "GamePad.h"
#include "Input.h"
#include "Sprite.h"
#include "memory"

// ゲームオーバー画面にあるボタン
enum ResultButton {
	EXITBUTTONRESULT,
	RETRYBUTTONRESULT,
};

class ResultUI {
public:
	ResultUI(Input* input, Audio* audio, GamePad* pad);
	~ResultUI();

	void Initialize();
	void Update();
	void Draw();
	void DrawUI();
	void AudioPlay();

public: // 関数
public: // ゲッター
	bool GetIsRetry() { return isRetry_; }
	bool GetIsExit() { return isExit_; }

public: // セッター
private:
	// インプット
	Input* input_ = nullptr;
	GamePad* pad_ = nullptr;
	// オーディオ
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandleExitButton_;
	uint32_t textureHandleRetryButton_;
	uint32_t textureHandleGameClear_;
	uint32_t textureHandleOperationUI_;

	// スプライト
	std::unique_ptr<Sprite> spriteExitButton_;
	std::unique_ptr<Sprite> spriteRetryButton_;
	std::unique_ptr<Sprite> spriteGameClear_;
	std::unique_ptr<Sprite> spriteOperationUI_;

	// サイズ
	Vector2 sizeExitButton_;
	Vector2 sizeRetryButton_;

	// ポジション
	Vector2 positionExitButton_;
	Vector2 positionRetryButton_;

private:
	ResultButton selectButton_;
	bool isRetry_;
	bool isExit_;
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
