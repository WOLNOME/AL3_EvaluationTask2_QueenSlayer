#pragma once
#include "Function.h"
#include "Input.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <cmath>

//前方宣言
class StageScene;

class TPSCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Input* input);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public://関数
	void LockOnCameraProcess();
	void NormalCameraProcess();

public: // ゲッター
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	const Vector3 GetWorldPosition();
	const Vector3& GetDirectionToPlayer() { return direction_; }

public: // セッター
	void SetStageScene(StageScene* stageScene) { stageScene_ = stageScene; }

private:
	// 入力
	Input* input_ = nullptr;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	//ステージシーン
	StageScene* stageScene_ = nullptr;

private://調整変数
	// カメラからプレイヤーへの方向
	Vector3 direction_ = {0.0f, 0.0f, 0.0f};
	// カメラの視点座標(カメラの周る球の中心座標/本来はプレイヤーが原点)
	Vector3 toCenterDirectionLocal_ = {0.0f, 3.0f, 0.0f};
	Vector3 toCenterDirectionWorld_;
	// プレイヤーとの距離
	const float length_ = 10.0f;
	// 視点移動速度
	const float kNormalSpeed_ = (120.0f / 360.0f * pi) / 60.0f;
	float speedLat_ = kNormalSpeed_;
	float speedLon_ = kNormalSpeed_;
	// 現在の視点角度座標
	float lat = 1.0f / 18.0f * pi;//緯度
	float lon = 1.0f / 2.0f * pi;//経度
	// 緯度経度のオリジンポイント
	float oLat = 0.0f;
	float oLon = 0.0f;
	//カメラのワールド座標を保持
	Vector3 setWorldPosition;
	// ロックオン処理変数
	bool isLockOn = false;//ロックオン状態か
	bool isTransition = false;//ロックオン遷移状態か
	// ロックオン遷移フレーム
	const uint32_t kTransitionFrame = 10;
	//遷移カウントダウン
	uint32_t countTransition_ = 0;
};
