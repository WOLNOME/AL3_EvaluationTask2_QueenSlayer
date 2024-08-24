#include "Particle.h"
#include "TextureManager.h"
#include "time.h"

Particle::Particle() {}

Particle::~Particle() {}

void Particle::Initialize(WorldTransform* worldTransform, uint32_t textureHandle, ParticleKind particleKind) {
	// 受け取ったワールドトランスフォームを親に設定する
	for (int i = 0; i < kParticleNum_; i++) {
		worldTransform_[i].parent_ = worldTransform;
	}
	// ワールドトランスフォーム初期化
	for (int i = 0; i < kParticleNum_; i++) {
		worldTransform_[i].Initialize();
	}
	// 使うテクスチャを取得
	textureHandle_ = textureHandle;
	// モデル
	for (int i = 0; i < kParticleNum_; i++) {
		modelParticle_[i].reset(Model::CreateFromOBJ("particle", true));
	}
	// パーティクルの種類を取得
	particleKind_ = particleKind;
	// 種類による初期化処理
	switch (particleKind_) {
	case DIFFUSION:
		// 速度
		for (int i = 0; i < kParticleNum_; i++) {
			srand((uint32_t)time(nullptr) + (uint32_t)std::powf((float)i, 3));
			velocity_[i].x = float(rand() % 10 - 5) * (1.0f / 10.0f);
			srand((uint32_t)time(nullptr) - (uint32_t)std::powf((float)i, 4));
			velocity_[i].y = float(rand() % 10 - 5) * (1.0f / 10.0f);
			srand((uint32_t)time(nullptr) - (uint32_t)std::powf((float)i, 2));
			velocity_[i].z = float(rand() % 10 - 5) * (1.0f / 10.0f);
		}

		break;
	case POP:
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
		for (int i = 0; i < kParticleNum_; i++) {
			worldTransform_[i].translation_.x += velocity_[i].x;
			worldTransform_[i].translation_.y += velocity_[i].y;
			worldTransform_[i].translation_.z += velocity_[i].z;
		}

		break;
	case POP:
		break;
	case SMOKE:
		break;
	default:
		break;
	}

	// 回転
	for (int i = 0; i < kParticleNum_; i++) {
		worldTransform_[i].rotation_.y += kRotateSpeed_;
	}

	// ワールドトランスフォームの更新
	for (int i = 0; i < kParticleNum_; i++) {
		worldTransform_[i].UpdateMatrix();
	}
}

void Particle::Draw(const ViewProjection& viewProjection) {
	// 複数モデル
	for (int i = 0; i < kParticleNum_; i++) {
		modelParticle_[i]->Draw(worldTransform_[i], viewProjection, textureHandle_);
	}
}
