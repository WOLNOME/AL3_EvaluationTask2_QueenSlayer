#pragma once
#include "Collider.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Particle.h"

class PlayerBullet : public Collider {
public:
	PlayerBullet();
	~PlayerBullet();

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
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
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;

private:
	// 速度
	Vector3 velocity_;
	// 寿命
	const int kLifeTime = 60 * 5;
	// デスタイマー
	int deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

private://演出系
	//パーティクル
	std::unique_ptr<Particle> particle_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandleParticle_;
	//演出中判定
	bool isDeadParticle_;
	//パーティクル長さ
	const int kMaxParticleTime_ = 20;
	//パーティクルタイマー
	int particleTimer_;

};
