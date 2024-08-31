#pragma once
#include "Input.h"
#include "GamePad.h"
#include "Audio.h"
#include "Sprite.h"
#include "Function.h"
#include "memory"

// 現在の状態
enum TitleSituation {
	TITLEMODE,
	HOWTOPLAYMODE,
};
// タイトル画面にあるボタン
enum TitleButton {
	PLAYBUTTONTITLE,
	HOWTOPLAYBUTTONTITLE,
	EXITBUTTONTITLE,
};

class TitleUI {
public:
	TitleUI(Input* input,Audio* audio,GamePad* pad);
	~TitleUI();

	void Initialize();
	void Update(Device device);
	void Draw();
	void DrawUI();
	void AudioPlay();

public: // 関数
public: // ゲッター
	bool GetIsPlay() { return isPlay_; }
	bool GetIsExit() { return isExit_; }

public: // セッター
private:
	// インプット
	Input* input_ = nullptr;
	//オーディオ
	Audio* audio_ = nullptr;
	//パッド
	GamePad* pad_ = nullptr;

	Device device_;


	// テクスチャハンドル
	uint32_t textureHandlePlayButton_;
	uint32_t textureHandleHowToPlayButton_;
	uint32_t textureHandleExitButton_;
	uint32_t textureHandleBackWhite_;
	uint32_t textureHandleTitleRogo_;
	uint32_t textureHandleOperationKeyBoardUI_;
	uint32_t textureHandleOperationGamePadUI_;
	uint32_t textureHandleHTPSlide_[kHowToPlayPageNum];

	// スプライト
	std::unique_ptr<Sprite> spritePlayButton_;
	std::unique_ptr<Sprite> spriteHowToPlayButton_;
	std::unique_ptr<Sprite> spriteExitButton_;
	std::unique_ptr<Sprite> spriteBackWhite_;
	std::unique_ptr<Sprite> spriteTitleRogo_;
	std::unique_ptr<Sprite> spriteOperationKeyBoardUI_;
	std::unique_ptr<Sprite> spriteOperationGamePadUI_;
	std::unique_ptr<Sprite> spriteHTPSlide_[kHowToPlayPageNum];

	// サイズ
	Vector2 sizePlayButton_;
	Vector2 sizeHowToPlayButton_;
	Vector2 sizeExitButton_;

	// ポジション
	Vector2 positionPlayButton_;
	Vector2 positionHowToPlayButton_;
	Vector2 positionExitButton_;

	// 回転
	float rotationTitleRogo_;

private:
	TitleSituation titleSituation_;
	TitleButton selectButton_;
	bool isPlay_;
	bool isExit_;
	int slidePage_;
	const float kWhiteBackAlpha_ = 0.15f;
	const float kStrngthButtonRatio_ = 1.2f;
	const float kRotateSpeed_ = (7.0f / 180.0f) * pi * (1.0f / 150.0f);
	float rotateSpeed_;
	float rotateAmount_;

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
