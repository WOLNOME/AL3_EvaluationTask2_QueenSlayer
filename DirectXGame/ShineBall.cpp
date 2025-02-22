#include "ShineBall.h"
#include "CollisionConfig.h"

ShineBall::ShineBall() {}

ShineBall::~ShineBall() {}

void ShineBall::Initialize(const Vector3& position, const Vector3& velocity) {
	//モデル生成
	model_.reset(Model::CreateFromOBJ("shineBall", true));
	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 半径設定
	radius_ = 1.0f;
	// 速度反映
	velocity_ = velocity;
	// デスタイマー設定
	deathTimer_ = kLifeTime;
	// デスフラグ設定
	isDead_ = false;
	// 静止フラグ
	isStop_ = false;
	// 表示フラグ
	isDisplay_ = true;
	// 衝突属性を設定(自分の属性)
	SetCollisionAttribute(kCollisionAttributeObject);

	// パーティクル生成
	particle_ = std::make_unique<Particle>();
	// パーティクル初期化
	particle_->Initialize(&worldTransform_, 0, ParticleKind::POP, 6,false);
	// パーティクルタイマー
	particleTimer_ = 0;
}

void ShineBall::Update() {
	// ワールドトランスフォームの更新←出現位置を初期値と一緒にするため
	worldTransform_.UpdateMatrix();
	//生きてるとき
	if (!isDeadParticle_) {
		if (!isStop_) {
			// 重力計算
			velocity_.y += kGravity_.y;
			// 速度加算
			worldTransform_.translation_.x += velocity_.x;
			worldTransform_.translation_.y += velocity_.y;
			worldTransform_.translation_.z += velocity_.z;
		}

		// 時間経過迫ったら点滅
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
	//消滅演出
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

void ShineBall::Draw(const ViewProjection& viewProjection) {
	// 本体描画
	if (!isDeadParticle_) {
		if (isDisplay_ && !isDead_) {
			model_->Draw(worldTransform_, viewProjection);
		}
	}
	if (particleTimer_ > 0) {
		particle_->Draw(viewProjection);
	}
}

void ShineBall::OnCollision() {}

void ShineBall::OnSpecialCollision() {
	isDeadParticle_ = true; 
	// 敵に当たった瞬間虚無属性になる
	SetCollisionAttribute(kCollisionAttributeNothingness);
}

Vector3 ShineBall::GetWorldPosition() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
