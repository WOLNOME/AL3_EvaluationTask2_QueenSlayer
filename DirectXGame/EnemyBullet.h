#pragma once
#include "Collider.h"
#include "Model.h"
#include "WorldTransform.h"

enum BulletKind {
	Diffusion,
	Aim,
	Homing,
};

class StageScene;

class EnemyBullet : public Collider {
public:
	EnemyBullet();
	~EnemyBullet();

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity,const BulletKind bulletKind);
	void Update();
	void Draw(const ViewProjection& viewProjection);

public: // 関数
	bool isDead() const { return isDead_; }
	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;
	// 特殊な衝突でのみ呼び出されるコールバック関数
	void OnSpecialCollision() override;

public: // ゲッター
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	// ワールド座標を取得
	Vector3 GetWorldPosition() override;
	// 半径
	float GetRadius() override { return radius_; }

public: // セッター
	void SetStageScene(StageScene* stageScene) { stageScene_ = stageScene; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	//ステージシーン
	StageScene* stageScene_ = nullptr;

private:
	// 速度
	Vector3 velocity_;
	// 寿命
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
	//弾の種類
	BulletKind bulletKind_ = Diffusion;
	//ホーミングの強さ（0~1まで）
	const float kHomingStrength_ = 0.4f;
	//弾のスピード
	const float kSpeed_ = 0.3f;

};
