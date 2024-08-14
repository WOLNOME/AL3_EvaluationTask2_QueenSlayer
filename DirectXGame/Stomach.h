#pragma once
#include "Function.h"
#include "Model.h"
#include "WorldTransform.h"

class Stomach {
public:
	Stomach();
	~Stomach();

	void Initialize(Model* model, const Vector3& position,const float rad);
	void Update();
	void Draw(ViewProjection& viewProjection);

public: // ゲッター
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

public: // セッター
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;

private:
	// 半径
	float rad_;
};
