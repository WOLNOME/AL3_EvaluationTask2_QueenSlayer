#include "PlayerBullet.h"
#include "CollisionConfig.h"
#include "Function.h"
#include "TextureManager.h"
#include <cassert>

PlayerBullet::PlayerBullet() {}

PlayerBullet::~PlayerBullet() {}

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
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
	// 衝突属性を設定(自分の属性)
	SetCollisionAttribute(kCollisionAttributePlayerBullet);

	// パーティクル生成
	particle_ = std::make_unique<Particle>();
	// テクスチャハンドル取得
	textureHandleParticle_ = TextureManager::Load("particle/ParticleYellow.png");
	// パーティクル初期化
	particle_->Initialize(&worldTransform_, textureHandleParticle_, ParticleKind::DIFFUSION, 4,false);
	// パーティクルタイマー
	particleTimer_ = 0;
}

void PlayerBullet::Update() {

	// ワールドトランスフォームの更新←出現位置を初期値と一緒にするため
	worldTransform_.UpdateMatrix();

	// 生きてるとき
	if (!isDeadParticle_) {
		// 速度加算
		worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

		// 時間経過でデス
		if (--deathTimer_ <= 0) {
			isDeadParticle_ = true;
		}
	}
	// 死亡パーティクル演出時
	else {
		// タイマーインクリメント
		particleTimer_++;

		// パーティクル更新処理
		particle_->Update();

		// タイマーが規定時間に達したら
		if (particleTimer_ == kMaxParticleTime_) {
			particleTimer_ = 0;
			isDeadParticle_ = false;
			isDead_ = true;
		}
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	if (!isDeadParticle_) {
		model_->Draw(worldTransform_, viewProjection);
	}
	if (particleTimer_ > 0) {
		particle_->Draw(viewProjection);
	}
}

void PlayerBullet::OnCollision() {
	isDeadParticle_ = true;
	// 敵に当たった瞬間虚無属性になる
	SetCollisionAttribute(kCollisionAttributeNothingness);
}

void PlayerBullet::OnSpecialCollision() {}

Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
