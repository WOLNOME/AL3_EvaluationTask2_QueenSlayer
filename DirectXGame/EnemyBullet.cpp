#include "EnemyBullet.h"
#include "CollisionConfig.h"
#include "Function.h"
#include "StageScene.h"
#include <cassert>

EnemyBullet::EnemyBullet() {}

EnemyBullet::~EnemyBullet() {}

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity, const BulletKind bulletKind) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 速度初期化
	velocity_ = velocity;
	// 半径設定
	radius_ = 0.2f;
	// 弾の種類
	bulletKind_ = bulletKind;
	// 衝突属性を設定(自分の属性)
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定(相手の属性)
	SetCollisionMask(kCollisionAttributePlayer);
}

void EnemyBullet::Update() {
	// ワールドトランスフォームの更新←出現位置を初期値と一緒にするため
	worldTransform_.UpdateMatrix();

	// 速度加算
	switch (bulletKind_) {
	case Diffusion:
		velocity_ = Multiply(kSpeed_, Normalize(velocity_));
		worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
		break;
	case Aim:
		velocity_ = Multiply(kSpeed_, Normalize(velocity_));
		worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
		break;
	case Homing:
		if (deathTimer_ >= 150) {
			velocity_ = Multiply(kSpeed_, Normalize(Slerp(velocity_, Subtract(stageScene_->GetPlayer()->GetWorldPostion(), GetWorldPosition()), kHomingStrength_)));
		}
		worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
		break;
	default:
		break;
	}

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}

void EnemyBullet::OnCollision() { isDead_ = true; }

Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
