#pragma once
#include "Function.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Particle.h"

class ShootingStand {
public:
	ShootingStand();
	~ShootingStand();

	void Initialize(Model* model);
	void Update(const Vector3 underPosition, const Vector3 cameraDir);
	void Draw(const ViewProjection& viewProjection);

public: // ゲッター
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	const Vector3& GetLocalPosition() { return localPosition_; }

public: // セッター
	void SetIsPlayerCrisis(bool isPlayerCrisis) { isPlayerCrisis_ = isPlayerCrisis; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;

private:
	// 砲台の装着位置
	const float ssPos = 1.0f;
	// ローカル座標
	Vector3 localPosition_;
	//
private: // 演出系
	// パーティクル
	std::unique_ptr<Particle> particle_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_;
	// 演出中判定
	bool isParticle_ = false;
	//プレイヤーがピンチ状態かを取得するフラグ
	bool isPlayerCrisis_ = false;

};
