#include "ShineBall.h"
#include "CollisionConfig.h"
#include <cassert>

ShineBall::ShineBall() {}

ShineBall::~ShineBall() {}

void ShineBall::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 半径設定
	radius_ = 1.0f;
	// 速度反映
	velocity_ = velocity;
	//デスタイマー設定
	deathTimer_ = kLifeTime;
	//デスフラグ設定
	isDead_ = false;
	//静止フラグ
	isStop_ = false;
	//表示フラグ
	isDisplay_ = true;
	// 衝突属性を設定(自分の属性)
	SetCollisionAttribute(kCollisionAttributeObject);
}

void ShineBall::Update() {
	// ワールドトランスフォームの更新←出現位置を初期値と一緒にするため
	worldTransform_.UpdateMatrix();

	if (!isStop_) {
		// 重力計算
		velocity_.y += kGravity_.y;
		// 速度加算
		worldTransform_.translation_.x += velocity_.x;
		worldTransform_.translation_.y += velocity_.y;
		worldTransform_.translation_.z += velocity_.z;
	}

	//時間経過迫ったら点滅
	if (deathTimer_ < 60 * 4) {
		if (deathTimer_ % 2 == 1) {
			if (isDisplay_) {
				isDisplay_ = false;
			} else {
				isDisplay_ = true;
			}
		}
	}

	// 回転
	worldTransform_.rotation_.y += 0.03f;

	// 静止処理
	if (worldTransform_.translation_.y <= radius_) {
		worldTransform_.translation_.y = radius_;
		isStop_ = true;
	}

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void ShineBall::Draw(const ViewProjection& viewProjection) {
	// 本体描画
	if (isDisplay_) {
		model_->Draw(worldTransform_, viewProjection);
	}
}

void ShineBall::OnCollision() {}

void ShineBall::OnSpecialCollision() { isDead_ = true; }

Vector3 ShineBall::GetWorldPosition() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
