#pragma once
#include "Input.h"
#include "Audio.h"
#include "Chest.h"
#include "Function.h"
#include "Head.h"
#include "Model.h"
#include "Stomach.h"
#include "WorldTransform.h"

// 行動パターン
enum EnemyActionPattern {
	Stop,
	AttackHead,
	AttackChest,
	AttackStomach,
	Move,
	kAllActionNum,
};


// 前方宣言
class StageScene;

class Enemy {
public:
	Enemy();
	~Enemy();

	void Initialize(const Vector3& position,Audio* audio,UseScene useScene);
	void Update();
	void Draw(ViewProjection& viewProjection);
	void AudioPlay();

public: // ゲッター
	// 腹部獲得
	const std::unique_ptr<Stomach>& GetStomach() const { return stomach_; }
	// 胸部獲得
	const std::unique_ptr<Chest>& GetChest() const { return chest_; }
	// 頭部獲得
	const std::unique_ptr<Head>& GetHead() const { return head_; }
	// HP獲得
	int GetHP() { return nowHP_; }
	// SP獲得
	int GetSP() { return nowSP_; }
	// 死亡判定
	bool GetIsDead() { return isDead_; }

public: // セッター
	void SetStageScene(StageScene* stageScene) { stageScene_ = stageScene; }
	void SetSP(int newSP) { nowSP_ = newSP; }

private:
	// 次のアクションルーレット
	EnemyActionPattern ActionRoulette();
	// 被弾処理
	void Damage();

public: // 定数
	//最大HP
	const int kMaxHP_ = 650;

private:
	// 入力
	Input* input_ = nullptr;
	//オーディオ
	Audio* audio_ = nullptr;
	// 腹部
	std::unique_ptr<Stomach> stomach_ = nullptr;
	std::unique_ptr<Model> modelStomach_ = nullptr;
	// 胸部
	std::unique_ptr<Chest> chest_ = nullptr;
	std::unique_ptr<Model> modelChest_ = nullptr;
	// 頭部
	std::unique_ptr<Head> head_ = nullptr;
	std::unique_ptr<Model> modelHead_ = nullptr;
	// ステージシーン
	StageScene* stageScene_ = nullptr;

private:
	// 各部位の半径
	const float rad_ = 1.5f;
	// 行動
	EnemyActionPattern action_ = Stop;

	// アクション中フラグ
	bool isAction_ = false;


	// 敵の体力
	int nowHP_;
	const int kDamageSmall_ = 1;
	const int kDamageMedium_ = 3;
	const int kDamageLarge_ = 120;

	// 光玉蓄積値
	int nowSP_;

	// 死亡フラグ
	bool isDead_;

	//使われるシーン
	UseScene useScene_;

private: // SE関連
	// 攻撃時効果音
	uint32_t soundHandleAttack_;
	uint32_t voiceHandleAttack_;
	bool isSoundPlayAttack_ = false;
	const float soundVolumeAttack_ = 0.85f;
	// 攻撃チャージ音
	uint32_t soundHandleCharge_;
	uint32_t voiceHandleCharge_;
	bool isSoundPlayCharge_ = false;
	const float soundVolumeCharge_ = 0.45f;
	// 被弾(中)時効果音
	uint32_t soundHandleMediumDamage_;
	uint32_t voiceHandleMediumDamage_;
	bool isSoundPlayMediumDamage_ = false;
	const float soundVolumeMediumDamage_ = 0.65f;
	// 被弾(大)時効果音
	uint32_t soundHandleLargeDamage_;
	uint32_t voiceHandleLargeDamage_;
	bool isSoundPlayLargeDamage_ = false;
	const float soundVolumeLargeDamage_ = 1.5f;
	
};
