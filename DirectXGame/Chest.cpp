#include "Chest.h"
#include "CollisionConfig.h"
#include "StageScene.h"
#include "TextureManager.h"
#include <cassert>

Chest::Chest() {}

Chest::~Chest() {
	// 解放
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Chest::Initialize(Model* model, const float rad) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// 弾モデルの生成
	modelBullet_.reset(Model::CreateFromOBJ("enemyBullet", true));
	// ワールドトランスフォーム設定
	worldTransform_.Initialize();
	// 半径をセット
	radius_ = rad;
	// テクスチャ
	textureHandle_ = TextureManager::Load("enemyChest/enemyChest.png");
	// 位置を半径に応じて変える
	worldTransform_.translation_ = {0.0f, radius_ * 2.0f, 0.0f};
	initPos = worldTransform_.translation_;
	// 衝突属性を設定(自分の属性)
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定(相手の属性)
	SetCollisionMask(kCollisionAttributePlayer);
}

void Chest::Update() {
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

	// 行列の再計算と転送
	worldTransform_.UpdateMatrix();
}

void Chest::Draw(ViewProjection& viewProjection) {
	// 胸部本体描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Chest::OnCollision() {}

void Chest::Attack() {
	// 行動処理中
	isAction_ = true;
	// 色を設定
	textureHandle_ = TextureManager::Load("enemyChest/enemyChestRed.png");
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

		// 5f毎に発射
		if (timer_ % 5 == 0) {
			//  弾を生成
			EnemyBullet* newBullet = new EnemyBullet();
			// 弾の発射位置を確定
			Vector3 bulletInitPosition = GetWorldPosition();

			// 弾の速度(ベクトル)を確定
			Vector3 velocity = Subtract(stageScene_->GetPlayer()->GetWorldPostion(), GetWorldPosition());
			// 弾を初期化
			newBullet->Initialize(modelBullet_.get(), bulletInitPosition, velocity, BulletKind::Aim);

			// 弾を登録する
			bullets_.push_back(newBullet);
		}
		// 制限時間に達したら
		if (timer_ == accumulatePowerTime_) {
			// タイマーリセット
			timer_ = 0;
			// 色リセット
			textureHandle_ = TextureManager::Load("enemyChest/enemyChest.png");
			// フラグの変更
			isShoot_ = false;
			isAction_ = false;
			isAccumulatePower_ = true;
		}
	}
}

Vector3 Chest::GetWorldPosition() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
