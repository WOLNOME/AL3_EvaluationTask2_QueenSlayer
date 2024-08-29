#include "Player.h"
#include "ImGuiManager.h"
#include "StageScene.h"
#include <cassert>

Player::Player() {}

Player::~Player() {
	switch (useScene_) {
	case USETITLE:
		break;
	case USESTAGE:
		// 解放
		for (PlayerBullet* bullet : bullets_) {
			delete bullet;
		}
		for (PlayerSpecialBullet* bullet : specialBullets_) {
			delete bullet;
		}
		// SE再生中止
		audio_->StopWave(voiceHandleFire_);
		audio_->StopWave(voiceHandleDamage_);
		audio_->StopWave(voiceHandleGetShineBall_);
		break;
	case USEDIRECTION:
		break;
	default:
		break;
	}
}

void Player::Initialize(const Vector3& position, Input* input, Audio* audio, UseScene useScene) {
	// 使われるシーン設定
	useScene_ = useScene;

	switch (useScene_) {
	case USETITLE:
		break;
	case USESTAGE:
		// 入力
		input_ = input;
		// オーディオ
		audio_ = audio;
		// 車両モデルの生成
		modelVehicle_.reset(Model::CreateFromOBJ("playerUnder", true));
		// 砲台モデルの生成
		modelStand_.reset(Model::CreateFromOBJ("playerAbove", true));
		// 弾モデルの生成
		modelBullet_.reset(Model::CreateFromOBJ("playerBullet", true));
		// 必殺弾モデルの生成
		modelSpecialBullet_.reset(Model::CreateFromOBJ("playerSpecialBullet", true));
		// 車両の生成
		vehicle_ = std::make_unique<Vehicle>();
		// 車両にstageSceneをセット
		vehicle_->SetStageScene(stageScene_);
		// 車両の初期化
		vehicle_->Initialize(input_, modelVehicle_.get(), position, useScene_);
		// 砲台の生成
		stand_ = std::make_unique<ShootingStand>();
		// 砲台の初期化
		stand_->Initialize(modelStand_.get(), useScene_);

		/// パラメーター
		nowHP_ = kMaxHP_;
		nowSPGauge_ = 0;
		isDead_ = false;
		nowInviTimer_ = 0;
		isInvincible_ = false;
		isDisplay_ = true;

		// サウンドハンドル
		soundHandleFire_ = audio_->LoadWave("Audio/fireNormalSE.wav");
		soundHandleDamage_ = audio->LoadWave("Audio/damageNormalSE.wav");
		soundHandleGetShineBall_ = audio_->LoadWave("Audio/getShineBallSE.wav");
		break;
	case USEDIRECTION:
		break;
	case USEGAMEOVER:
		// 車両モデルの生成
		modelVehicle_.reset(Model::CreateFromOBJ("playerUnder", true));
		// 砲台モデルの生成
		modelStand_.reset(Model::CreateFromOBJ("playerAbove", true));
		// 車両の生成
		vehicle_ = std::make_unique<Vehicle>();
		// 車両の初期化
		vehicle_->Initialize(input_, modelVehicle_.get(), position, useScene_);
		// 砲台の生成
		stand_ = std::make_unique<ShootingStand>();
		// 砲台と車両をペアレント設定
		stand_->SetParent(&vehicle_->GetWorldTransform());
		// 砲台の初期化
		stand_->Initialize(modelStand_.get(), useScene_);
		break;
	default:
		break;
	}
}

void Player::Update() {
	switch (useScene_) {
	case USETITLE:
		break;
	case USESTAGE:
		// デスフラグの立った弾を削除
		bullets_.remove_if([](PlayerBullet* bullet) {
			if (bullet->isDead()) {
				delete bullet;
				return true;
			}
			return false;
		});
		specialBullets_.remove_if([](PlayerSpecialBullet* bullet) {
			if (bullet->isDead()) {
				delete bullet;
				return true;
			}
			return false;
		});

		// 攻撃処理
		Attack();

		// 車両の更新
		vehicle_->Update();
		// 砲台の更新
		cameraDir = {stageScene_->GetTPSCamera()->GetDirectionToPlayer().x, 0.0f, stageScene_->GetTPSCamera()->GetDirectionToPlayer().z};
		stand_->Update(vehicle_->GetLocalPosition(), cameraDir);
		// 弾更新
		for (PlayerBullet* bullet : bullets_) {
			bullet->Update();
		}
		// 必殺弾更新
		for (PlayerSpecialBullet* bullet : specialBullets_) {
			bullet->Update();
		}

		// 必殺弾演出中判定
		for (PlayerSpecialBullet* bullet : specialBullets_) {
			if (bullet->GetIsDeadParticle() && !isHitStopOnce_) {
				isSpecialBulletDirection_ = true;
				isHitStopOnce_ = true;
			}
		}
		// ヒットストップ処理
		if (isSpecialBulletDirection_) {
			// タイマーインクリメント
			hitStopTimer_++;
			// 規定時間になったら
			if (hitStopTimer_ == kHitStopTime_) {
				isSpecialBulletDirection_ = false;
				hitStopTimer_ = 0;
			}
		}

		// プレイヤーのピンチ状態を伝える
		if (nowHP_ <= 3) {
			stand_->SetIsPlayerCrisis(true);
		} else {
			stand_->SetIsPlayerCrisis(false);
		}

		// SP回収処理
		if (vehicle_->GetIsGetShineBall()) {
			nowSPGauge_++;
			if (nowSPGauge_ == kMaxSPGauge_) {
				isUseSP_ = true;
			}
			// SP回収したらSE鳴らす
			isSoundPlayGetShineBall_ = true;
			vehicle_->SetIsGetShineBall(false);
		}

		// ダメージ判定
		Damage();
		// 死亡判定
		if (nowHP_ <= 0) {
			isDead_ = true;
		}

		// 無敵処理
		Invincible();

#ifdef _DEBUG
		ImGui::Begin("player");
		ImGui::Text("playerHP : %d/%d", nowHP_, kMaxHP_);
		ImGui::Text("playerSPGauge : %d/%d", nowSPGauge_, kMaxSPGauge_);
		ImGui::Text("invincible : %d", isInvincible_);
		ImGui::End();
#endif // _DEBUG
		break;
	case USEDIRECTION:
		cameraDir;
		break;
	case USEGAMEOVER:
		// 車両の更新
		vehicle_->Update();
		// 砲台の更新
		stand_->Update({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
		break;
	default:
		break;
	}
}

void Player::Draw(ViewProjection& viewProjection) {
	switch (useScene_) {
	case USETITLE:
		break;
	case USESTAGE:

		if (isDisplay_) {
			// 砲台描画
			stand_->Draw(viewProjection);
			// 車両描画
			vehicle_->Draw(viewProjection);
		}
		// 弾描画
		for (PlayerBullet* bullet : bullets_) {
			bullet->Draw(viewProjection);
		}
		// 必殺弾描画
		for (PlayerSpecialBullet* bullet : specialBullets_) {
			bullet->Draw(viewProjection);
		}
		break;
	case USEDIRECTION:
		break;
	case USEGAMEOVER:
		// 砲台描画
		stand_->Draw(viewProjection);
		// 車両描画
		vehicle_->Draw(viewProjection);
		break;
	default:
		break;
	}
}

void Player::AudioPlay() {
	// SE再生
	if (isSoundPlayFire_) {
		voiceHandleFire_ = audio_->PlayWave(soundHandleFire_, false, soundVolumeFire_);
		isSoundPlayFire_ = false;
	}
	if (isSoundPlayDamage_) {
		voiceHandleDamage_ = audio_->PlayWave(soundHandleDamage_, false, soundVolumeDamage_);
		isSoundPlayDamage_ = false;
	}
	if (isSoundPlayGetShineBall_) {
		voiceHandleGetShineBall_ = audio_->PlayWave(soundHandleGetShineBall_, false, soundVolumeGetShineBall_);
		isSoundPlayGetShineBall_ = false;
	}
}

void Player::Attack() {
	// インターバル関連処理
	if (interval_ > 0) {
		interval_--;
	}
	// 通常弾
	if (input_->PushKey(DIK_SPACE)) {

		if (interval_ == 0) {

			// 弾を生成
			PlayerBullet* newBullet = new PlayerBullet();
			// 弾の発射位置を確定
			Vector3 bulletInitPosition = Transform({0.0f, 0.0f, 3.0f}, stand_->GetWorldTransform().matWorld_);
			// 弾の速度(ベクトル)を確定
			Vector3 velocity = Normalize(Subtract(stageScene_->GetReticle()->GetWorldPosition(), bulletInitPosition));
			velocity = Multiply(kBulletSpeed_, velocity);
			// 弾を初期化
			newBullet->Initialize(modelBullet_.get(), bulletInitPosition, velocity);
			// SE再生
			isSoundPlayFire_ = true;
			// 弾を登録する
			bullets_.push_back(newBullet);
			// インターバルリセット
			interval_ = kBulletInterval_;
		}
	}
	// 必殺弾
	if (isUseSP_) {
		if (input_->TriggerKey(DIK_B)) {
			// 弾を生成
			PlayerSpecialBullet* newBullet = new PlayerSpecialBullet();
			// 弾の発射位置を確定
			Vector3 bulletInitPosition = Transform({0.0f, 0.0f, 3.0f}, stand_->GetWorldTransform().matWorld_);
			// 弾の速度(ベクトル)を確定
			Vector3 velocity = Normalize(Subtract(stageScene_->GetReticle()->GetWorldPosition(), bulletInitPosition));
			velocity = Multiply(kBulletSpeed_, velocity);
			// 弾を初期化
			newBullet->Initialize(modelSpecialBullet_.get(), bulletInitPosition, velocity);
			// SE再生
			isSoundPlayFire_ = true;
			// 弾を登録する
			specialBullets_.push_back(newBullet);
			// インターバルリセット
			interval_ = kBulletInterval_;
			// SP消費
			isUseSP_ = false;
			nowSPGauge_ = 0;
			// 弾が生成されたらヒットストップフラグを基に戻す。
			isHitStopOnce_ = false;
		}
	}
}

void Player::Damage() {
	if (vehicle_->GetIsDamage()) {
		if (!isInvincible_) {
			nowHP_ -= 1;
			isInvincible_ = true;
			// SE鳴らす
			isSoundPlayDamage_ = true;
		}

		vehicle_->SetIsDamage(false);
	}
}

void Player::Invincible() {
	// 無敵処理
	if (isInvincible_) {
		// タイマーインクリメント
		nowInviTimer_++;
		// 最初の1秒はゆっくり点滅
		if (nowInviTimer_ < 60) {
			if (nowInviTimer_ % 3 == 1) {
				if (isDisplay_) {
					isDisplay_ = false;
				} else {
					isDisplay_ = true;
				}
			}

		}
		// 最後の1秒は早く点滅
		else {
			if (nowInviTimer_ % 2 == 1) {
				if (isDisplay_) {
					isDisplay_ = false;
				} else {
					isDisplay_ = true;
				}
			}
		}

		// タイマーが規定時間に達したら
		if (nowInviTimer_ == kInviTime_) {
			nowInviTimer_ = 0;
			isInvincible_ = false;
			isDisplay_ = true;
		}
	}
}
