#pragma once
#include "Collider.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Particle.h"

class StageScene;

class ShineBall : public Collider {
public:
	ShineBall();
	~ShineBall();

	void Initialize(const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);

public: // 関数
	bool isDead() const { return isDead_; }
	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;
	// プレイヤーとの衝突でのみ呼び出されるコールバック関数
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
	std::unique_ptr<Model> model_ = nullptr;
	// ステージシーン
	StageScene* stageScene_ = nullptr;

private:
	//重力
	const Vector3 kGravity_ = {0.0f, -0.05f, 0.0f};
	// 寿命
	const int kLifeTime = 60 * 15;
	// デスタイマー
	int deathTimer_;
	// デスフラグ
	bool isDead_;
	//速度
	Vector3 velocity_;
	//静止フラグ
	bool isStop_;
	//表示されているかフラグ
	bool isDisplay_;

private: // 演出系
	// パーティクル
	std::unique_ptr<Particle> particle_ = nullptr;
	// 演出中判定
	bool isDeadParticle_;
	// パーティクル長さ
	const int kMaxParticleTime_ = 30;
	// パーティクルタイマー
	int particleTimer_;
};
