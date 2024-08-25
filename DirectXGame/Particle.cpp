#include "Particle.h"
#include "TextureManager.h"
#include "time.h"

Particle::Particle() {}

Particle::~Particle() {}

void Particle::Initialize(const WorldTransform* worldTransform, uint32_t textureHandle, ParticleKind particleKind, uint32_t particleNum) {
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
			particle_[i].velocity.x = float(rand() % 6 - 3) * (1.0f / 10.0f);
			srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 4));
			particle_[i].velocity.y = float(rand() % 6) * (1.0f / 10.0f);
			srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 2));
			particle_[i].velocity.z = float(rand() % 6 - 3) * (1.0f / 10.0f);
		}
		break;
	case SMOKE:
		break;
	default:
		break;
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
		break;
	default:
		break;
	}

	// 回転
	for (uint32_t i = 0; i < particleNum_; i++) {
		particle_[i].worldTransform.rotation_.y += kRotateSpeed_;
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
