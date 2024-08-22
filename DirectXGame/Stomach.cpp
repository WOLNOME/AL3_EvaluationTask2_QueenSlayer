#include "Stomach.h"
#include "CollisionConfig.h"
#include "StageScene.h"
#include "TextureManager.h"
#include "time.h"
#include <cassert>

Stomach::Stomach() {}

Stomach::~Stomach() {
	// 解放
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Stomach::Initialize(Model* model, const Vector3& position, const float rad) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// 弾モデルの生成
	modelBullet_.reset(Model::CreateFromOBJ("enemyBullet", true));
	// ワールドトランスフォーム設定
	worldTransform_.Initialize();
	// 引数で受け取った座標をセット
	initPos = position;
	worldTransform_.translation_ = initPos;
	// 半径をセット
	radius_ = rad;
	// テクスチャ
	textureHandle_ = TextureManager::Load("enemyStomach/enemyStomach.png");
	// 衝突属性を設定(自分の属性)
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定(相手の属性)
	SetCollisionMask(kCollisionAttributePlayer);
}

void Stomach::Update() {
	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	// プレイヤーの方向に回転
	Vector3 direction_ = Subtract(stageScene_->GetPlayer()->GetWorldPostion(), GetWorldPosition());
	direction_ = Normalize({direction_.x, 0.0f, direction_.z});
	worldTransform_.rotation_.y = std::atan2(direction_.x, direction_.z);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 velocityZ = Transform(direction_, rotateYMatrix);
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	// 行列の再計算と転送
	worldTransform_.UpdateMatrix();
}

void Stomach::Draw(ViewProjection& viewProjection) {
	// 腹部本体描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Stomach::OnCollision() {}

void Stomach::Attack() {
	// 行動処理中
	isAction_ = true;
	// 色を設定
	textureHandle_ = TextureManager::Load("enemyStomach/enemyStomachRed.png");
	// 重力の設定
	Vector3 kGravity = {0.0f, -0.05f, 0.0f};

	// 力ため中処理
	if (isAccumulatePower_) {
		// 振動前の位置を保持
		if (timer_ == 0) {
			prePos = worldTransform_.translation_;
		}
		// タイマーインクリメント
		timer_++;

		// シード値をセット
		srand((int)std::powf((float)timer_, 5));
		// 座標更新
		worldTransform_.translation_.x = prePos.x + float(rand() % 11 - 5) * (1.0f / 30.0f);
		worldTransform_.translation_.z = prePos.z + float(rand() % 11 - 5) * (1.0f / 30.0f);

		// 制限時間に達したら
		if (timer_ == accumulatePowerTime_) {
			// タイマーリセット
			timer_ = 0;
			// 座標リセット
			worldTransform_.translation_ = prePos;
			// フラグの変更
			isAccumulatePower_ = false;
			isPreMove_ = true;
			// ジャンプ量
			velocity_ = {0.0f, 1.0f, 0.0f};
		}
	}
	// 予備動作処理
	if (isPreMove_) {
		// 重力処理
		velocity_.y += kGravity.y;
		// 速度加算
		worldTransform_.translation_.y += velocity_.y;

		// 制限時間に達したら
		if (worldTransform_.translation_.y <= initPos.y) {
			// タイマーリセット
			timer_ = 0;
			// 座標リセット
			worldTransform_.translation_.y = initPos.y;
			// フラグの変更
			isPreMove_ = false;
			isShoot_ = true;
		}
	}
	// 攻撃処理
	if (isShoot_) {
		// タイマーインクリメント
		timer_++;

		//  弾を生成
		EnemyBullet* newBullet = new EnemyBullet();
		// 弾の発射位置を確定
		Vector3 bulletInitPosition = GetWorldPosition();
		// シード値
		srand((int)std::powf((float)timer_, 3));
		// θ
		float theta = float(rand() % 360) / 180.0f * pi;
		// 弾の速度(ベクトル)を確定
		Vector3 velocity;
		velocity.x = std::cosf(theta);
		velocity.y = 0;
		velocity.z = std::sinf(theta);
		// 弾を初期化
		newBullet->Initialize(modelBullet_.get(), bulletInitPosition, velocity, BulletKind::Diffusion);

		// 弾を登録する
		bullets_.push_back(newBullet);

		// 制限時間に達したら
		if (timer_ == accumulatePowerTime_) {
			// タイマーリセット
			timer_ = 0;
			// 色リセット
			textureHandle_ = TextureManager::Load("enemyStomach/enemyStomach.png");
			// フラグの変更
			isShoot_ = false;
			isAction_ = false;
			isAccumulatePower_ = true;
		}
	}
}

void Stomach::Jump() {
	// 行動処理中
	isAction_ = true;
	// 色を設定
	textureHandle_ = TextureManager::Load("enemyStomach/enemyStomachRed.png");
	// 重力の設定
	Vector3 kGravity = {0.0f, -0.05f, 0.0f};

	// 力ため中処理
	if (isAccumulatePower_) {
		// ため前の位置を保持
		if (timer_ == 0) {
			prePos = worldTransform_.translation_;
		}
		// タイマーインクリメント
		timer_++;

		// 少し縮小
		worldTransform_.scale_.y = Lerp(1.0f, kReducation_, float(timer_ / accumulatePowerTime_));

		// 制限時間に達したら
		if (timer_ == accumulatePowerTime_) {
			// タイマーリセット
			timer_ = 0;
			// 座標リセット
			worldTransform_.translation_ = prePos;
			// フラグの変更
			isAccumulatePower_ = false;
			isJump_ = true;
			// ジャンプ量
			velocity_ = {0.0f, 1.0f, 0.0f};
			// 目的地までの移動速度
			velocity_.x = Multiply(1.0f / 40.0f, Subtract(stageScene_->GetPlayer()->GetWorldPostion(), GetWorldPosition())).x;
			velocity_.z = Multiply(1.0f / 40.0f, Subtract(stageScene_->GetPlayer()->GetWorldPostion(), GetWorldPosition())).z;
		}
	}
	// ジャンプ処理
	if (isJump_) {
		// 重力処理
		velocity_.y += kGravity.y;
		// 速度加算
		worldTransform_.translation_.x += velocity_.x;
		worldTransform_.translation_.y += velocity_.y;
		worldTransform_.translation_.z += velocity_.z;

		// 制限時間に達したら
		if (worldTransform_.translation_.y <= initPos.y) {
			// タイマーリセット
			timer_ = 0;
			// 色リセット
			textureHandle_ = TextureManager::Load("enemyStomach/enemyStomach.png");
			// 座標リセット
			worldTransform_.translation_.y = initPos.y;
			// スケール基に戻す
			worldTransform_.scale_.y = 1.0f;
			// フラグの変更
			isJump_ = false;
			isAction_ = false;
			isAccumulatePower_ = true;
		}
	}
}

Vector3 Stomach::GetWorldPosition() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
