#pragma once
#include "Function.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Head.h"
#include "Chest.h"
#include "Stomach.h"

class Enemy {
public:
	Enemy();
	~Enemy();

	void Initialize(const Vector3& position);
	void Update();
	void Draw(ViewProjection& viewProjection);

public: // ゲッター


public: // セッター
	

private:
	// 入力
	Input* input_ = nullptr;
	//腹部
	std::unique_ptr<Stomach> stomach_ = nullptr;
	std::unique_ptr<Model> modelStomach_ = nullptr;
	//胸部
	std::unique_ptr<Chest> chest_ = nullptr;
	std::unique_ptr<Model> modelChest_ = nullptr;
	//頭部
	std::unique_ptr<Head> head_ = nullptr;
	std::unique_ptr<Model> modelHead_ = nullptr;

private:
	//各部位の半径
	const float rad_ = 1.5f;

};
