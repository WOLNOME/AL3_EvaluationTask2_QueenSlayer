#include "Enemy.h"
#include "StageScene.h"
#include "ImGuiManager.h"
#include "time.h"

Enemy::Enemy() {}

Enemy::~Enemy() {
	//サウンド停止
	audio_->StopWave(voiceHandleAttack_);
	audio_->StopWave(voiceHandleCharge_);
	audio_->StopWave(voiceHandleMediumDamage_);
	audio_->StopWave(voiceHandleLargeDamage_);
}

void Enemy::Initialize(const Vector3& position, Audio* audio) {
	//オーディオ
	audio_ = audio;
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

	// 頭部にステージシーンを渡す
	head_->SetStageScene(stageScene_);
	// 胸部にステージシーンを渡す
	chest_->SetStageScene(stageScene_);
	// 腹部にステージシーンを渡す
	stomach_->SetStageScene(stageScene_);
	// 腹部(親)と胸部(子)の親子関係を結ぶ
	chest_->SetParent(&stomach_->GetWorldTransform());
	// 胸部(親)と頭部(子)の親子関係を結ぶ
	head_->SetParent(&chest_->GetWorldTransform());

	// 腹部の初期化
	stomach_->Initialize(modelStomach_.get(), position, rad_);
	// 胸部の初期化
	chest_->Initialize(modelChest_.get(), rad_);
	// 頭部の初期化
	head_->Initialize(modelHead_.get(), rad_);

	///パラメーター設定
	nowHP_ = kMaxHP_;
	nowSP_ = 0;
	isDead_ = false;

	// サウンドハンドル
	soundHandleAttack_ = audio_->LoadWave("Audio/attackEnemySE.wav");
	soundHandleCharge_ = audio->LoadWave("Audio/chargeEnemySE.wav");
	soundHandleMediumDamage_ = audio_->LoadWave("Audio/damageMediumSE.wav");
	soundHandleLargeDamage_ = audio->LoadWave("Audio/damageLargeSE.wav");

}

void Enemy::Update() {

	// 敵の行動
	switch (action_) {
	case Stop:
		StopAction();
		//終わったらルーレット
		if (!isAction_) {
			action_ = ActionRoulette();
		}
		break;
	case AttackHead:
		head_->Attack();
		isAction_ = head_->GetIsAction();
		//終わったらSTOP
		if (!isAction_) {
			action_ = EnemyActionPattern::Stop;
		}
		break;
	case AttackChest:
		chest_->Attack();
		isAction_ = chest_->GetIsAction();
		// 終わったらSTOP
		if (!isAction_) {
			action_ = EnemyActionPattern::Stop;
		}
		break;
	case AttackStomach:
		stomach_->Attack();
		isAction_ = stomach_->GetIsAction();
		// 終わったらSTOP
		if (!isAction_) {
			action_ = EnemyActionPattern::Stop;
		}
		break;
	case Move:
		stomach_->Jump();
		isAction_ = stomach_->GetIsAction();
		// 終わったらSTOP
		if (!isAction_) {
			action_ = EnemyActionPattern::Stop;
		}
		break;
	default:
		break;
	}

	// 腹部更新
	stomach_->Update();
	// 胸部更新
	chest_->Update();
	// 頭部更新
	head_->Update();

	//光玉蓄積処理
	if (head_->GetIsHit()) {
		nowSP_++;
		head_->SetIsHit(false);
	}
	if (chest_->GetIsHit()) {
		nowSP_++;
		chest_->SetIsHit(false);
	}
	if (stomach_->GetIsHit()) {
		nowSP_++;
		stomach_->SetIsHit(false);
	}
	//被弾処理
	Damage();

	// HPが半分以下になったら行動速度をあげる
	if (nowHP_ < kMaxHP_ / 2) {
		head_->SetAccumlatePowerTime(70);
		chest_->SetAccumlatePowerTime(70);
		stomach_->SetAccumlatePowerTime(70);
	}

	//体力0になったら死ぬ
	if (nowHP_ <= 0) {
		isDead_ = true;
	}

	//サウンド
	if (head_->GetIsCharge()) {
		isSoundPlayCharge_ = true;
		head_->SetIsCharge(false);
	}
	if (chest_->GetIsCharge()) {
		isSoundPlayCharge_ = true;
		chest_->SetIsCharge(false);
	}
	if (stomach_->GetIsCharge()) {
		isSoundPlayCharge_ = true;
		stomach_->SetIsCharge(false);
	}
	if (head_->GetIsAttack()) {
		isSoundPlayAttack_ = true;
		head_->SetIsAttack(false);
	}
	if (chest_->GetIsAttack()) {
		isSoundPlayAttack_ = true;
		chest_->SetIsAttack(false);
	}
	if (stomach_->GetIsAttack()) {
		isSoundPlayAttack_ = true;
		stomach_->SetIsAttack(false);
	}

#ifdef _DEBUG
	ImGui::Begin("enemy");
	ImGui::Text("enemyHP : %d/%d", nowHP_, kMaxHP_);
	ImGui::Text("SP accumulate value : %d/7", nowSP_);
	ImGui::End();
#endif // _DEBUG


}

void Enemy::Draw(ViewProjection& viewProjection) {
	// 腹部描画
	stomach_->Draw(viewProjection);
	// 胸部描画
	chest_->Draw(viewProjection);
	// 頭部描画
	head_->Draw(viewProjection);
}

void Enemy::AudioPlay() {
	// SE再生
	if (isSoundPlayAttack_) {
		voiceHandleAttack_ = audio_->PlayWave(soundHandleAttack_, false, soundVolumeAttack_);
		isSoundPlayAttack_ = false;
	}
	if (isSoundPlayCharge_) {
		voiceHandleCharge_ = audio_->PlayWave(soundHandleCharge_, false, soundVolumeCharge_);
		isSoundPlayCharge_ = false;
	}
	if (isSoundPlayMediumDamage_) {
		voiceHandleMediumDamage_ = audio_->PlayWave(soundHandleMediumDamage_, false, soundVolumeMediumDamage_);
		isSoundPlayMediumDamage_ = false;
	}
	if (isSoundPlayLargeDamage_) {
		voiceHandleLargeDamage_ = audio_->PlayWave(soundHandleLargeDamage_, false, soundVolumeLargeDamage_);
		isSoundPlayLargeDamage_ = false;
	}
}

void Enemy::StopAction() {
	isAction_ = true;
	//タイマーをインクリメント
	timer_++;
	//タイマーが時間に達したら
	if (timer_ == kStopTime_) {
		//タイマーリセット
		timer_ = 0;
		//フラグ
		isAction_=false;
	}

}

EnemyActionPattern Enemy::ActionRoulette() {
	EnemyActionPattern result;
	uint32_t currentTime = (int)time(nullptr);
	srand(currentTime*2);
	int num = rand() % EnemyActionPattern::kAllActionNum;
	result = (EnemyActionPattern)num;
	//もしプレイヤーと敵が離れすぎてたら強制的に移動処理
	if (Length(Subtract(stageScene_->GetPlayer()->GetWorldPostion(), stomach_->GetWorldPosition())) > 55) {
		result = Move;
	}
	return result;
}

void Enemy::Damage() {
	//関節部分狙うと2箇所にダメージが通るようになる

	if (head_->GetIsDamageSmall()) {
		nowHP_ -= kDamageSmall_;
		head_->SetIsDamageSmall(false);
	}
	if (chest_->GetIsDamageSmall()) {
		nowHP_ -= kDamageSmall_;
		chest_->SetIsDamageSmall(false);
	}
	if (stomach_->GetIsDamageSmall()) {
		nowHP_ -= kDamageSmall_;
		stomach_->SetIsDamageSmall(false);
	}

	if (head_->GetIsDamageMedium()) {
		nowHP_ -= kDamageMedium_;
		// サウンド
		isSoundPlayMediumDamage_ = true;
		head_->SetIsDamageMedium(false);
	}
	if (chest_->GetIsDamageMedium()) {
		nowHP_ -= kDamageMedium_;
		// サウンド
		isSoundPlayMediumDamage_ = true;
		chest_->SetIsDamageMedium(false);
	}
	if (stomach_->GetIsDamageMedium()) {
		nowHP_ -= kDamageMedium_;
		// サウンド
		isSoundPlayMediumDamage_ = true;
		stomach_->SetIsDamageMedium(false);
	}

	if (head_->GetIsDamageLarge()) {
		nowHP_ -= kDamageLarge_;
		// サウンド
		isSoundPlayLargeDamage_ = true;
		head_->SetIsDamageLarge(false);
	}
	if (chest_->GetIsDamageLarge()) {
		nowHP_ -= kDamageLarge_;
		// サウンド
		isSoundPlayLargeDamage_ = true;
		chest_->SetIsDamageLarge(false);
	}
	if (stomach_->GetIsDamageLarge()) {
		nowHP_ -= kDamageLarge_;
		// サウンド
		isSoundPlayLargeDamage_ = true;
		stomach_->SetIsDamageLarge(false);
	}

	//HPが0より下になったら0にする
	if (nowHP_ < 0) {
		nowHP_ = 0;
	}

}
