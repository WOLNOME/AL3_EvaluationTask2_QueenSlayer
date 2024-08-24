#pragma once
#include "Model.h"
#include "WorldTransform.h"

// パーティクルの数
const int kParticleNum_ = 5;

//パーティクルの種類
enum ParticleKind {
	DIFFUSION,
	POP,
	SMOKE,
};

class Particle {
public:
	Particle();
	~Particle();

	void Initialize(WorldTransform* worldTransform,uint32_t textureHandle,ParticleKind particleKind);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_[kParticleNum_];
	// モデル
	std::unique_ptr<Model> modelParticle_[kParticleNum_];
	//使うテクスチャハンドル
	uint32_t textureHandle_;

private:
	//パーティクルの種類
	ParticleKind particleKind_;
	//回転速度
	const float kRotateSpeed_ = 0.3f;
	//移動速度
	Vector3 velocity_[kParticleNum_];
};
