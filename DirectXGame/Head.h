#pragma once
#include "Collider.h"
#include "Function.h"
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"

// 前方宣言
class StageScene;

class Head : public Collider {
public:
	Head();
	~Head();

	void Initialize(Model* model, const float rad, UseScene useScene);
	void Update();
	void Draw(ViewProjection& viewProjection);

public: // 関数
	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;
	// 特殊な衝突でのみ呼び出されるコールバック関数
	void OnSpecialCollision() override;
	void Attack();

public: // ゲッター
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	// ワールド座標を取得
	Vector3 GetWorldPosition() override;
	// 半径
	float GetRadius() override { return radius_; }
	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }
	// 攻撃中コールバック
	bool GetIsAction() { return isAction_; }
	//光玉蓄積許可
	bool GetIsHit() { return isHit_; }
	//被弾ゲッター
	bool GetIsDamageSmall() { return isDamageSmall_; }
	bool GetIsDamageMedium() { return isDamageMedium_; }
	bool GetIsDamageLarge() { return isDamageLarge_; }
	bool GetIsCharge() { return isCharge_; }
	bool GetIsAttack() { return isAttack_; }

public: // セッター
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; };
	void SetStageScene(StageScene* stageScene) { stageScene_ = stageScene; }
	void SetIsHit(bool isHit) { isHit_ = isHit; }
	void SetIsDamageSmall(bool isDamageSmall) { isDamageSmall_ = isDamageSmall; }
	void SetIsDamageMedium(bool isDamageMedium) { isDamageMedium_ = isDamageMedium; }
	void SetIsDamageLarge(bool isDamageLarge) { isDamageLarge_ = isDamageLarge; }
	void SetAccumlatePowerTime(int accumlatePowerTime) { accumulatePowerTime_ = accumlatePowerTime; }
	void SetIsCharge(bool isCharge) { isCharge_ = isCharge; }
	void SetIsAttack(bool isAttack) { isAttack_ = isAttack; }


private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// ステージシーン
	StageScene* stageScene_ = nullptr;
	// テクスチャ
	uint32_t textureHandle_ = 0;
	// 初期座標
	Vector3 initPos = {0.0f, 0.0f, 0.0f};
	// 弾
	std::list<EnemyBullet*> bullets_;
	std::unique_ptr<Model> modelBullet_ = nullptr;

private:
	bool isAction_ = false;
	bool isAccumulatePower_ = true;
	bool isPreMove_ = false;
	bool isShoot_ = false;

	// 各フェーズで使うタイマー
	int timer_ = 0;
	// 各フェーズの時間
	int accumulatePowerTime_ = 120;
	const int shootTime_ = 60;
	// 速度
	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};
	// 振動前の座標を保持
	Vector3 prePos;
	//光玉蓄積フラグ
	bool isHit_ = false;
	//小ダメージ蓄積フラグ
	bool isDamageSmall_ = false;
	//中ダメージ蓄積フラグ
	bool isDamageMedium_ = false;
	//大ダメージ蓄積フラグ
	bool isDamageLarge_ = false;

	//サウンドフラグ
	bool isCharge_ = false;
	bool isAttack_ = false;

	// 使われるシーン
	UseScene useScene_;

private: // 演出系
	// パーティクル
	std::unique_ptr<Particle> particle_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandleParticle_;
	// 演出中判定
	bool isParticle_;
	// パーティクル長さ
	const int kMaxParticleTime_ = 40;
	// パーティクルタイマー
	int particleTimer_;
};
