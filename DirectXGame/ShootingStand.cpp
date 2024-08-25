#include "ShootingStand.h"
#include <cassert>
#include "TextureManager.h"

ShootingStand::ShootingStand() {}

ShootingStand::~ShootingStand() {}

void ShootingStand::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	//ローカルポジション初期化
	localPosition_ = worldTransform_.translation_;

	// パーティクル生成
	particle_ = std::make_unique<Particle>();
	//テクスチャハンドル
	textureHandle_ = TextureManager::Load("particle/ParticleSmoke.png");
	// パーティクル初期化
	particle_->Initialize(&worldTransform_, textureHandle_, ParticleKind::SMOKE, 25);
	
}

void ShootingStand::Update(const Vector3 underPosition, const Vector3 cameraDir) {
	//プレイヤーがピンチ状態になったら煙演出出す
	if (isPlayerCrisis_) {
		isParticle_ = true;
	} else {
		isParticle_ = false;
	}
	//パーティクル更新処理
	if (isParticle_) {
		particle_->Update();
	}

	
	// 砲台の位置を更新(常に車両部と合わせる)
	worldTransform_.translation_ = underPosition;
	worldTransform_.translation_.y += ssPos;

	// 砲台の回転を更新(レティクルに合わせる)
	worldTransform_.rotation_.y = std::atan2(cameraDir.x, cameraDir.z);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 velocityZ = Transform(cameraDir, rotateYMatrix);
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	//ローカル座標更新
	localPosition_ = worldTransform_.translation_;
	// 行列の更新と転送
	worldTransform_.UpdateMatrix();
}

void ShootingStand::Draw(const ViewProjection& viewProjection) {
	// 砲台本体描画
	model_->Draw(worldTransform_, viewProjection);
	//パーティクル
	if (isParticle_) {
		particle_->Draw(viewProjection);
	}

}
