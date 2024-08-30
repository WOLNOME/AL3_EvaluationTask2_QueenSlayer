#pragma once
#include "BaseScene.h"
#include "Input.h"
#include "Audio.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DirectionCamera.h"
#include "Ground.h"
#include "Skydome.h"
#include "Background.h"
#include "Model.h"
#include "Sprite.h"
#include "Particle.h"
#include <memory>


class DirectionScene : public BaseScene {
public:
	DirectionScene();
	~DirectionScene() override;

public:
	void Init(Input* input, Audio* audio) override;
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* commandList, DirectXCommon* dxCommon_) override;

public:
	SCENE GetNextScene() override { return NextScene; }
	bool GetIsExit() override { return isExit; }

private:
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	/// ワールドトランスフォーム
	WorldTransform worldTransformEnemyHead_;
	WorldTransform worldTransformEnemyChest_;
	WorldTransform worldTransformEnemyStomach_;
	WorldTransform worldTransformEnemyHeadInitPos_;
	WorldTransform worldTransformEnemyChestInitPos_;
	WorldTransform worldTransformEnemyStomachInitPos_;
	/// ビュープロジェクション
	ViewProjection viewProjection_;
	///モデルテクスチャ
	uint32_t textureHandleGround_;
	uint32_t textureHandleSkydome_;
	///3Dモデル
	std::unique_ptr<Model> modelEnemyHead_ = nullptr;
	std::unique_ptr<Model> modelEnemyChest_ = nullptr;
	std::unique_ptr<Model> modelEnemyStomach_ = nullptr;
	///スプライトテクスチャ
	uint32_t textureHandleSkipUI_;
	///2Dスプライト
	std::unique_ptr<Sprite> spriteSkipUI_ = nullptr;
	///インスタンス
	std::unique_ptr<DirectionCamera> directionCamera_ = nullptr;
	std::unique_ptr<Ground> ground_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;
	std::unique_ptr<Background> background_ = nullptr;
	std::unique_ptr<Particle> particleHead_ = nullptr;
	std::unique_ptr<Particle> particleChest_ = nullptr;
	std::unique_ptr<Particle> particleStomach_ = nullptr;


private://変数
	int timer_ = 0;
	const float kEnemyRadius_ = 1.5f;
	const int kCameraMoveTime_ = 180;
	const int kExplosionTime_ = 240;
	const int kMoveScene_ = 360;
	int framePerUpdate = 1;
	
	Vector3 velocityEnemyHead_;
	Vector3 velocityEnemyChest_;
	Vector3 velocityEnemyStomach_;

	const Vector3 kGravity_ = {0.0f, -0.02f, 0.0f};

	// SE関連
	uint32_t soundHandleExplosionSE_;
	uint32_t voiceHandleExplosionSE_;
	bool isSoundPlayExplosionSE_ = false;
	const float soundVolumeExplosionSE_ = 1.5f;
};
