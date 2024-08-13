#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Function.h"

class ShootingStand {
public:
	ShootingStand();
	~ShootingStand();

	void Initialize(Model* model);
	void Update(const Vector3 underPosition);
	void Draw(const ViewProjection& viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	//砲台の装着位置
	const float ssPos = 1.0f;
};
