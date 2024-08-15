#pragma once
#include "Collider.h"
#include "Function.h"
#include "Model.h"
#include "WorldTransform.h"

class Stomach : public Collider {
public:
	Stomach();
	~Stomach();

	void Initialize(Model* model, const Vector3& position, const float rad);
	void Update();
	void Draw(ViewProjection& viewProjection);

public: // 関数
	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

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
};
