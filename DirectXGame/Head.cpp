#include "Head.h"
#include "CollisionConfig.h"
#include "StageScene.h"
#include "TextureManager.h"
#include <cassert>

Head::Head() {}

Head::~Head() {
	// 解放
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Head::Initialize(Model* model, const float rad) {
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
	textureHandle_ = TextureManager::Load("enemyHead/enemyHead.png");
	// 位置を半径に応じて変える
	worldTransform_.translation_ = {0.0f, radius_ * 2.0f, 0.0f};
	initPos = worldTransform_.translation_;
	// 衝突属性を設定(自分の属性)
	SetCollisionAttribute(kCollisionAttributeEnemy);
}

void Head::Update() {
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

void Head::Draw(ViewProjection& viewProjection) {
	// 頭部本体描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Head::OnCollision() {
	//アクション中の時
	if (isAction_) {
		isHit_ = true;
		isDamageMedium_ = true;
	}
	else {
		isDamageSmall_ = true;
	}
}

void Head::OnSpecialCollision() { isDamageLarge_ = true; }

void Head::Attack() {
	// 行動処理中
	isAction_ = true;
	// 色を設定
	textureHandle_ = TextureManager::Load("enemyHead/enemyHeadRed.png");
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
			// 弾にステージシーンを渡す
			newBullet->SetStageScene(stageScene_);
			// 弾の発射位置を確定
			Vector3 bulletInitPosition = GetWorldPosition();
			// 弾の速度(ベクトル)を確定
			Vector3 velocity = Subtract(stageScene_->GetPlayer()->GetWorldPostion(), GetWorldPosition());
			// 弾を初期化
			newBullet->Initialize(modelBullet_.get(), bulletInitPosition, velocity, BulletKind::Homing);

			// 弾を登録する
			bullets_.push_back(newBullet);
		}
		// 制限時間に達したら
		if (timer_ == accumulatePowerTime_) {
			// タイマーリセット
			timer_ = 0;
			// 色リセット
			textureHandle_ = TextureManager::Load("enemyHead/enemyHead.png");
			// フラグの変更
			isShoot_ = false;
			isAction_ = false;
			isAccumulatePower_ = true;
		}
	}
}

Vector3 Head::GetWorldPosition() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
