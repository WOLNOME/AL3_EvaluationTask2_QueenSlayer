#pragma once
#include "Function.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "memory"
#include <cmath>

class GameOverCamera {
public:
	GameOverCamera();
	~GameOverCamera();

	void Initialize();
	void Update();

public: // 関数
public: // ゲッター
	const ViewProjection& GetViewProjection() { return viewProjection_; }

public: // セッター
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

private: // 変数
	// 球面の座標
	float lat_;
	float lon_;
	//初期座標
	const float kInitPosLat_ = (35.0f / 360.0f) * pi;
	const float kInitPosLon_ = 0.0f;
	// 球面速度
	const float velocityLat_ = 0.0f;
	const float velocityLon_ = (30.0f / 180.0f) * pi * (1.0f / 60.0f);
	// 球の半径
	const float length_ = 10.0f;
	// カメラの向きベクトル
	Vector3 direction_;

	// 原点(戦車中心)
	const Vector3 kCenter_ = {0.0f, 2.5f, 0.0f};
};
