#include "Enemy.h"
#include"StageScene.h"

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(const Vector3& position) {
	// 腹部モデルの生成
	modelStomach_.reset(Model::CreateFromOBJ("enemyStomach", true));
	// 胸部モデルの生成
	modelChest_.reset(Model::CreateFromOBJ("enemyChest", true));
	// 頭部モデルの生成
	modelHead_.reset(Model::CreateFromOBJ("enemyHead", true));

	// 腹部の生成
	stomach_ = std::make_unique<Stomach>();
	// 胸部の生成
	chest_ = std::make_unique<Chest>();
	// 頭部の生成
	head_ = std::make_unique<Head>();

	//腹部にステージシーンを渡す
	stomach_->SetStageScene(stageScene_);
	//腹部(親)と胸部(子)の親子関係を結ぶ
	chest_->SetParent(&stomach_->GetWorldTransform());
	// 胸部(親)と頭部(子)の親子関係を結ぶ
	head_->SetParent(&chest_->GetWorldTransform());

	// 腹部の初期化
	stomach_->Initialize(modelStomach_.get(), position,rad_);
	// 胸部の初期化
	chest_->Initialize(modelChest_.get(), rad_);
	// 頭部の初期化
	head_->Initialize(modelHead_.get(), rad_);
}

void Enemy::Update() {
	//敵の行動
	switch (action_) {
	case Stop:
		break;
	case AttackHead:
		head_->Attack();
		break;
	case AttackChest:
		chest_->Attack();
		break;
	case AttackStomach:
		stomach_->Attack();
		break;
	case Move:
		break;
	default:
		break;
	}

	//腹部更新
	stomach_->Update();
	//胸部更新
	chest_->Update();
	//頭部更新
	head_->Update();



}

void Enemy::Draw(ViewProjection& viewProjection) {
	// 腹部描画
	stomach_->Draw(viewProjection);
	// 胸部描画
	chest_->Draw(viewProjection);
	// 頭部描画
	head_->Draw(viewProjection);

}
