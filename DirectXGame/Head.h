#pragma once
#include "Function.h"
#include "Model.h"
#include "WorldTransform.h"

class Head {
public:
	Head();
	~Head();

	void Initialize(Model* model,const float rad);
	void Update();
	void Draw(ViewProjection& viewProjection);

public: // ゲッター
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

public: // セッター
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; };

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;

private:
	//半径
	float rad_;

};
