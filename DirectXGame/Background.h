#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Background {
public:
	Background();
	~Background();

	void Initialize(const Vector3& position);
	void Update();
	void Draw(ViewProjection& viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	std::unique_ptr<Model> modelWood_ = nullptr;
	std::unique_ptr<Model> modelLeaf_ = nullptr;
	
};
