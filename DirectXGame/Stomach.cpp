#include "Stomach.h"
#include "CollisionConfig.h"
#include "StageScene.h"
#include "TextureManager.h"
#include "time.h"
#include <cassert>

Stomach::Stomach() {}

Stomach::~Stomach() {
	switch (useScene_) {
	case USETITLE:
		break;
	case USESTAGE:
		// 解放
		for (EnemyBullet* bullet : bullets_) {
			delete bullet;
		}
		break;
	case USEDIRECTION:
		break;
	default:
		break;
	}

	
}

void Stomach::Initialize(Model* model, const Vector3& position, const float rad, UseScene useScene) {
	// 使用されるシーン設定
	useScene_ = useScene;

	// シーン毎の初期化処理
	switch (useScene_) {
	case USETITLE:
		// NULLポインタチェック
		assert(model);
		model_ = model;
		// ワールドトランスフォーム設定
		worldTransform_.Initialize();
		// 引数で受け取った座標をセット
		initPos = position;
		worldTransform_.translation_ = initPos;
		// 半径をセット
		radius_ = rad;
		// テクスチャ
		textureHandle_ = TextureManager::Load("enemyStomach/enemyStomach.png");
		break;
	case USESTAGE:
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

		// パーティクル生成
		particle_ = std::make_unique<Particle>();
		// テクスチャハンドル取得
		textureHandleParticle_ = TextureManager::Load("particle/ParticleSand.png");
		// パーティクル初期化
		particle_->Initialize(&worldTransform_, textureHandleParticle_, ParticleKind::POP, 40, false);
		// パーティクルタイマー
		particleTimer_ = 0;
		break;
	case USEDIRECTION:
		break;
	default:
		break;
	}

	
}

void Stomach::Update() {
	switch (useScene_) {
	case USETITLE:
		// 行列の再計算と転送
		worldTransform_.UpdateMatrix();
		break;
	case USESTAGE:

		// デスフラグの立った弾を削除
		bullets_.remove_if([](EnemyBullet* bullet) {
			if (bullet->isDead()) {
				delete bullet;
				return true;
			}
			return false;
		});

		// パーティクル更新
		if (isParticle_) {
			// タイマーインクリメント
			particleTimer_++;
			// パーティクル更新
			particle_->Update();
			// タイマー規定時間に達したら
			if (particleTimer_ >= kMaxParticleTime_) {
				particleTimer_ = 0;
				isParticle_ = false;
			}
		}

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
		break;
	case USEDIRECTION:
		break;
	default:
		break;
	}

}

void Stomach::Draw(ViewProjection& viewProjection) {
	switch (useScene_) {
	case USETITLE:
		// 腹部本体描画
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
		break;
	case USESTAGE:
		// 腹部本体描画
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
		// 弾描画
		for (EnemyBullet* bullet : bullets_) {
			bullet->Draw(viewProjection);
		}
		// パーティクル描画
		if (isParticle_) {
			particle_->Draw(viewProjection);
		}
		break;
	case USEDIRECTION:
		break;
	default:
		break;
	}

	
}

void Stomach::OnCollision() {
	// アクション中の時
	if (isAction_) {
		isHit_ = true;
		isDamageMedium_ = true;
	} else {
		isDamageSmall_ = true;
	}
}

void Stomach::OnSpecialCollision() { isDamageLarge_ = true; }

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
			isCharge_ = true;
		}
		// タイマーインクリメント
		timer_++;

		// シード値をセット
		srand((int)std::powf((float)timer_, 5));
		// 座標更新
		worldTransform_.translation_.x = prePos.x + float(rand() % 11 - 5) * (1.0f / 30.0f);
		worldTransform_.translation_.z = prePos.z + float(rand() % 11 - 5) * (1.0f / 30.0f);

		// 制限時間に達したら
		if (timer_ >= accumulatePowerTime_) {
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
			isAttack_ = true;
			isParticle_ = true;
			// パーティクルポジション初期化
			particle_->Initialize(&worldTransform_, textureHandleParticle_, ParticleKind::POP, 0, true);
		}
	}
	// 攻撃処理
	if (isShoot_) {
		// タイマーインクリメント
		timer_++;

		// 3fに1個弾を出す
		if (timer_ % 3 == 1) {
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
		}

		// 制限時間に達したら
		if (timer_ >= accumulatePowerTime_) {
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
			isCharge_ = true;
		}
		// タイマーインクリメント
		timer_++;

		// 少し縮小
		worldTransform_.scale_.y = Lerp(1.0f, kReduction_, ((float)timer_ / accumulatePowerTime_));

		// 制限時間に達したら
		if (timer_ >= accumulatePowerTime_) {
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
			// スケール基に戻す
			worldTransform_.scale_.y = 1.0f;
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

			// フラグの変更
			isJump_ = false;
			isAction_ = false;
			isAccumulatePower_ = true;
			isAttack_ = true;
			isParticle_ = true;
			//パーティクルポジション初期化
			particle_->Initialize(&worldTransform_, textureHandleParticle_, ParticleKind::POP, 0, true);
		}
	}
}

void Stomach::Stop() {
	//停止処理中
	isStop_ = true;
	//拡縮指示
	if (timer_ == 0) {
		isReduction_ = true;
		scalingTime_ = 0;
		addScale_ = goalScale_ * (1.0f / kReductionTime_);
	}
	//タイマーインクリメント
	timer_++;
	//拡大処理
	if (isExpansion_) {
		scalingTime_++;
		worldTransform_.scale_.y += addScale_;
		if (scalingTime_ >= kExpansionTime_) {
			scalingTime_ = 0;
			isExpansion_ = false;
			isReduction_ = true;
			addScale_ = goalScale_ * (1.0f / kReductionTime_);
		}
	}
	//縮小処理
	else if (isReduction_) {
		scalingTime_++;
		worldTransform_.scale_.y -= addScale_;
		if (scalingTime_ >= kReductionTime_) {
			scalingTime_ = 0;
			isExpansion_ = true;
			isReduction_ = false;
			addScale_ = goalScale_ * (1.0f / kExpansionTime_);
		}

	}
	//タイマーが規定時間に達したら終了
	if (timer_ >= stopTime_) {
		timer_ = 0;
		scalingTime_ = 0;
		isStop_ = false;
		isExpansion_ = false;
		isReduction_ = false;
		addScale_ = 0.0f;
		worldTransform_.scale_.y = 1.0f;
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
