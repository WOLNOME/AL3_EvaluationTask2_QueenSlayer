#include "Particle.h"
#include "Function.h"
#include "TextureManager.h"
#include "time.h"

Particle::Particle() {}

Particle::~Particle() {}

void Particle::Initialize(const WorldTransform* worldTransform, uint32_t textureHandle, ParticleKind particleKind, uint32_t particleNum, bool positionReset) {
	// ポジションのみ初期化
	if (positionReset) {
		for (uint32_t i = 0; i < particleNum_; i++) {
			// ワールドトランスフォーム初期化
			particle_[i].worldTransform.translation_.x = 0.0f;
			particle_[i].worldTransform.translation_.y = 0.0f;
			particle_[i].worldTransform.translation_.z = 0.0f;
			//回転初期化
			particle_[i].worldTransform.rotation_.x = 0.0f;
			particle_[i].worldTransform.rotation_.y = 0.0f;
			particle_[i].worldTransform.rotation_.z = 0.0f;

		}
		//  種類による初期化処理
		switch (particleKind_) {
		case DIFFUSION:
			// 速度
			for (uint32_t i = 0; i < particleNum_; i++) {
				srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 3));
				particle_[i].velocity.x = float(rand() % 6 - 3) * (1.0f / 10.0f);
				srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 4));
				particle_[i].velocity.y = float(rand() % 6 - 3) * (1.0f / 10.0f);
				srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 2));
				particle_[i].velocity.z = float(rand() % 6 - 3) * (1.0f / 10.0f);
			}

			break;
		case POP:
			// 速度
			for (uint32_t i = 0; i < particleNum_; i++) {
				srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 3));
				particle_[i].velocity.x = float(rand() % 8 - 4) * (1.0f / 10.0f);
				srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 4));
				particle_[i].velocity.y = float(rand() % 5 + 3) * (1.0f / 10.0f);
				srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 2));
				particle_[i].velocity.z = float(rand() % 8 - 4) * (1.0f / 10.0f);
			}
			break;
		case SMOKE:
			// 速度
			for (uint32_t i = 0; i < particleNum_; i++) {
				srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 3));
				particle_[i].velocity.x = 0.25f;
				particle_[i].velocity.y = float(rand() % 6 + 1) * (1.0f / 10.0f);
				particle_[i].velocity.z = 0.2f;
			}
			break;
		default:
			break;
		}

	}
	// 1度のみの初期化処理
	else {
		// パーティクルの数を取得(最大30個まで)
		particleNum_ = particleNum;
		// 受け取ったワールドトランスフォームを親に設定する
		for (uint32_t i = 0; i < particleNum_; i++) {
			particle_[i].worldTransform.parent_ = worldTransform;
		}
		// ワールドトランスフォーム初期化
		for (uint32_t i = 0; i < particleNum_; i++) {
			particle_[i].worldTransform.Initialize();
		}
		// 使うテクスチャを取得
		for (uint32_t i = 0; i < particleNum_; i++) {
			particle_[i].textureHandle = textureHandle;
		}
		// textureHandleが0ならカラフル
		if (textureHandle == 0) {
			for (uint32_t i = 0; i < particleNum_; i++) {
				srand((uint32_t)time(nullptr) + i);
				uint32_t color = (uint32_t)rand() % 7;
				if (color == 0) {
					particle_[i].textureHandle = TextureManager::Load("particle/ParticleRed.png");
				} else if (color == 1) {
					particle_[i].textureHandle = TextureManager::Load("particle/ParticleOrange.png");
				} else if (color == 2) {
					particle_[i].textureHandle = TextureManager::Load("particle/ParticleYellow.png");
				} else if (color == 3) {
					particle_[i].textureHandle = TextureManager::Load("particle/ParticleGreen.png");
				} else if (color == 4) {
					particle_[i].textureHandle = TextureManager::Load("particle/ParticleBlue.png");
				} else if (color == 5) {
					particle_[i].textureHandle = TextureManager::Load("particle/ParticleIndigo.png");
				} else if (color == 6) {
					particle_[i].textureHandle = TextureManager::Load("particle/ParticleViolet.png");
				}
			}
		}

		// モデル
		for (uint32_t i = 0; i < particleNum_; i++) {
			particle_[i].model.reset(Model::CreateFromOBJ("particle", true));
		}
		// パーティクルの種類を取得
		particleKind_ = particleKind;
		// 回転速度設定
		for (uint32_t i = 0; i < particleNum_; i++) {
			srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 3));
			particle_[i].rotation.x = float(rand() % 7 + 3) * (1.0f / 10.0f);
			srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 4));
			particle_[i].rotation.y = float(rand() % 7 + 3) * (1.0f / 10.0f);
			srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 2));
			particle_[i].rotation.z = float(rand() % 7 + 3) * (1.0f / 10.0f);
		}
		// 種類による初期化処理
		switch (particleKind_) {
		case DIFFUSION:
			// 速度
			for (uint32_t i = 0; i < particleNum_; i++) {
				srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 3));
				particle_[i].velocity.x = float(rand() % 6 - 3) * (1.0f / 10.0f);
				srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 4));
				particle_[i].velocity.y = float(rand() % 6 - 3) * (1.0f / 10.0f);
				srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 2));
				particle_[i].velocity.z = float(rand() % 6 - 3) * (1.0f / 10.0f);
			}

			break;
		case POP:
			// 速度
			for (uint32_t i = 0; i < particleNum_; i++) {
				srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 3));
				particle_[i].velocity.x = float(rand() % 8 - 4) * (1.0f / 10.0f);
				srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 4));
				particle_[i].velocity.y = float(rand() % 5 + 3) * (1.0f / 10.0f);
				srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 2));
				particle_[i].velocity.z = float(rand() % 8 - 4) * (1.0f / 10.0f);
			}
			break;
		case SMOKE:
			// 速度
			for (uint32_t i = 0; i < particleNum_; i++) {
				srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 3));
				particle_[i].velocity.x = 0.25f;
				particle_[i].velocity.y = float(rand() % 6 + 1) * (1.0f / 10.0f);
				particle_[i].velocity.z = 0.2f;
			}
			break;
		default:
			break;
		}
	}
}

void Particle::Update() {
	// 各種類のvelocity加算処理
	switch (particleKind_) {
	case DIFFUSION:
		for (uint32_t i = 0; i < particleNum_; i++) {
			particle_[i].worldTransform.translation_.x += particle_[i].velocity.x;
			particle_[i].worldTransform.translation_.y += particle_[i].velocity.y;
			particle_[i].worldTransform.translation_.z += particle_[i].velocity.z;
		}

		break;
	case POP:
		for (uint32_t i = 0; i < particleNum_; i++) {
			// 重力処理
			particle_[i].velocity.y += kGravity_.y;

			particle_[i].worldTransform.translation_.x += particle_[i].velocity.x;
			particle_[i].worldTransform.translation_.y += particle_[i].velocity.y;
			particle_[i].worldTransform.translation_.z += particle_[i].velocity.z;
		}
		break;
	case SMOKE:
		for (uint32_t i = 0; i < particleNum_; i++) {
			// 加算処理
			particle_[i].worldTransform.translation_.x += particle_[i].velocity.x;
			particle_[i].worldTransform.translation_.y += particle_[i].velocity.y;
			particle_[i].worldTransform.translation_.z += particle_[i].velocity.z;
			// 高度上限処理
			if (particle_[i].worldTransform.translation_.y > kSmokeMaxHeight_) {
				particle_[i].worldTransform.translation_.x = 0.0f;
				particle_[i].worldTransform.translation_.y = 0.0f;
				particle_[i].worldTransform.translation_.z = 0.0f;
			}
		}
		break;
	default:
		break;
	}

	// 回転
	for (uint32_t i = 0; i < particleNum_; i++) {
		particle_[i].worldTransform.rotation_.x += particle_[i].rotation.x;
		particle_[i].worldTransform.rotation_.y += particle_[i].rotation.y;
		particle_[i].worldTransform.rotation_.z += particle_[i].rotation.z;
	}

	// ワールドトランスフォームの更新
	for (uint32_t i = 0; i < particleNum_; i++) {
		particle_[i].worldTransform.UpdateMatrix();
	}
}

void Particle::Draw(const ViewProjection& viewProjection) {
	// 複数モデル
	for (uint32_t i = 0; i < particleNum_; i++) {
		particle_[i].model->Draw(particle_[i].worldTransform, viewProjection, particle_[i].textureHandle);
	}
}
