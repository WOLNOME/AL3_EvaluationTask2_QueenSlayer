#include "ShootingStand.h"
#include <cassert>

ShootingStand::ShootingStand() {}

ShootingStand::~ShootingStand() {}

void ShootingStand::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
}

void ShootingStand::Update(const Vector3 underPosition) {
	// 砲台の位置を更新(常に車両部と合わせる)
	worldTransform_.translation_ = underPosition;
	worldTransform_.translation_.y += ssPos;

	// 砲台の回転を更新(レティクルに合わせる)
	//worldTransform_.rotation_.y += 0.03f;

	//行列の再計算
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void ShootingStand::Draw(const ViewProjection& viewProjection) {
	// 砲台本体描画
	model_->Draw(worldTransform_, viewProjection);
}
