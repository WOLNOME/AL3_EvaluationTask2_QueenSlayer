#include "ShootingStand.h"
#include "TextureManager.h"
#include <cassert>

ShootingStand::ShootingStand() {}

ShootingStand::~ShootingStand() {}

void ShootingStand::Initialize(Model* model, UseScene useScene) {
	// 使用されるシーン設定
	useScene_ = useScene;

	switch (useScene_) {
	case USETITLE:
		break;
	case USESTAGE:
		// NULLポインタチェック
		assert(model);
		model_ = model;
		worldTransform_.Initialize();
		// ローカルポジション初期化
		localPosition_ = worldTransform_.translation_;

		// パーティクル生成
		particle_ = std::make_unique<Particle>();
		// テクスチャハンドル
		textureHandleParticle_ = TextureManager::Load("particle/ParticleSmoke.png");
		// パーティクル初期化
		particle_->Initialize(&worldTransform_, textureHandleParticle_, ParticleKind::SMOKE, 25, false);
		break;
	case USEDIRECTION:
		break;
	case USEGAMEOVER:
		// NULLポインタチェック
		assert(model);
		model_ = model;
		// テクスチャハンドル
		textureHandle_ = TextureManager::Load("player/player_tank_damage.png");
		worldTransform_.Initialize();
		worldTransform_.translation_.y = ssPos;

		// パーティクル生成
		particle_ = std::make_unique<Particle>();
		// テクスチャハンドル
		textureHandleParticle_ = TextureManager::Load("particle/ParticleSmoke.png");
		// パーティクル初期化
		particle_->Initialize(&worldTransform_, textureHandleParticle_, ParticleKind::SMOKE, 40, false);
		break;
	default:
		break;
	}
}

void ShootingStand::Update(const Vector3 underPosition, const Vector3 cameraDir) {
	switch (useScene_) {
	case USETITLE:
		break;
	case USESTAGE:
		// プレイヤーがピンチ状態になったら煙演出出す
		if (isPlayerCrisis_) {
			isParticle_ = true;
		} else {
			isParticle_ = false;
		}
		// パーティクル更新処理
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

		// ローカル座標更新
		localPosition_ = worldTransform_.translation_;
		// 行列の更新と転送
		worldTransform_.UpdateMatrix();
		break;
	case USEDIRECTION:
		break;
	case USEGAMEOVER:
		particle_->Update();

		// 行列の更新と転送
		worldTransform_.UpdateMatrix();
		break;
	default:
		break;
	}
}

void ShootingStand::Draw(const ViewProjection& viewProjection) {

	switch (useScene_) {
	case USETITLE:
		break;
	case USESTAGE:
		// 砲台本体描画
		model_->Draw(worldTransform_, viewProjection);
		// パーティクル
		if (isParticle_) {
			particle_->Draw(viewProjection);
		}
		break;
	case USEDIRECTION:
		break;
	case USEGAMEOVER:
		// 砲台本体描画
		model_->Draw(worldTransform_, viewProjection,textureHandle_);
		// パーティクル
		particle_->Draw(viewProjection);
		break;
	default:
		break;
	}
}
