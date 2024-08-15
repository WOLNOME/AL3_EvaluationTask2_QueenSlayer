#pragma once
#include "Collider.h"
#include "Function.h"
#include "Model.h"
#include "WorldTransform.h"


class Chest : public Collider {
public:
	Chest();
	~Chest();

	void Initialize(Model* model,const float rad);
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
	float GetRadius()override { return radius_; }

public: // セッター
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; };

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;

private:
};
