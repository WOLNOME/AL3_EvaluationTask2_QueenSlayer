#pragma once
#include "Function.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"

class StageScene;

class Vehicle {
public:
	Vehicle();
	~Vehicle();

	void Initialize(Input* input,Model* model,const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

public://ゲッター
	//ローカル座標
	const Vector3& GetLocalPosition() { return localPos_; }
	const Vector3 GetWorldPostion();

public://セッター
	//ステージシーンのセット
	void SetStageScene(StageScene* stageScene) { stageScene_ = stageScene; }

private:
	//入力
	Input* input_ = nullptr;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	//ステージシーン
	StageScene* stageScene_ = nullptr;

private: // 調整可能数値
	// スピード
	const float speed_ = 0.2f;
	// 移動方向の傾き
	float radian_ = 0.0f;
	// プレイヤーの向きベクトル
	Vector3 direction_ = {0.0f, 1.0f, 0.0f};
	// 移動判定変数
	bool isMove = false;
	// プレイヤーの回転速度
	const float rotateSpeed_ = (2.0f / 2.0f) * pi / 60.0f;
	//ローカル座標保存用
	Vector3 localPos_ = {0.0f, 0.0f, 0.0f};
};
