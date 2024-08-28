#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "vector"

// パーティクルの種類
enum ParticleKind {
	DIFFUSION,
	POP,
	SMOKE,
};

//パーティクルデータ
typedef struct ParticleData {
	WorldTransform worldTransform;
	std::unique_ptr<Model> model;
	Vector3 velocity;
	Vector3 rotation;
	uint32_t textureHandle;
} ParticleData;

class Particle {
public:
	Particle();
	~Particle();

	void Initialize(const WorldTransform* worldTransform, uint32_t textureHandle, ParticleKind particleKind, uint32_t particleNum,bool positionReset);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	//パーティクルデータ
	ParticleData particle_[50];

private:
	// パーティクルの数
	size_t particleNum_ = 0;
	// パーティクルの種類
	ParticleKind particleKind_;
	// 回転速度
	const float kRotateSpeed_ = 0.5f;
	//重力
	const Vector3 kGravity_ = {0.0f, -0.025f, 0.0f};
	//煙最大高度
	const float kSmokeMaxHeight_ = 2.5f;
};
