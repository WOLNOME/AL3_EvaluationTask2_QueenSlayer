#include "TitleCamera.h"
#include "ImGuiManager.h"

TitleCamera::TitleCamera() {}

TitleCamera::~TitleCamera() {}

void TitleCamera::Initialize() {
	//変数の初期化
	nowCource = MoveCourse::COURCEONE;
	timerOfOneCource = 0;
	//velocityの初期化
	velocityLat_ = (goalLat_[nowCource] - startLat_[nowCource]) * (1.0f / kTimeOfOneCource);
	velocityLon_ = (goalLon_[nowCource] - startLon_[nowCource]) * (1.0f / kTimeOfOneCource);
	// 座標を初期化
	lat_ = startLat_[nowCource];
	lon_ = startLon_[nowCource];

	// ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_.x = kCenter_.x + length_[nowCource] * std::cosf(lat_) * std::cosf(lon_);
	worldTransform_.translation_.y = kCenter_.y + length_[nowCource] * std::sinf(lat_);
	worldTransform_.translation_.z = kCenter_.z + length_[nowCource] * std::cosf(lat_) * std::sinf(lon_);
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

	// directionの初期化
	direction_ = Normalize(Subtract(kCenter_, worldTransform_.translation_));
}

void TitleCamera::Update() {
	//タイマーインクリメント
	timerOfOneCource++;
	//タイマーが規定時間に達したら
	if (timerOfOneCource >= kTimeOfOneCource) {
		timerOfOneCource = 0;
		//各コースごとの処理
		switch (nowCource) {
		case COURCEONE:
			//次のコースへ
			nowCource = MoveCourse::COURCETWO;
			//速度再設定
			velocityLat_ = (goalLat_[nowCource] - startLat_[nowCource]) * (1.0f / kTimeOfOneCource);
			velocityLon_ = (goalLon_[nowCource] - startLon_[nowCource]) * (1.0f / kTimeOfOneCource);
			//現在の座標を変える
			lat_ = startLat_[nowCource];
			lon_ = startLon_[nowCource];
			break;
		case COURCETWO:
			// 次のコースへ
			nowCource = MoveCourse::COURCETHREE;
			// 速度再設定
			velocityLat_ = (goalLat_[nowCource] - startLat_[nowCource]) * (1.0f / kTimeOfOneCource);
			velocityLon_ = (goalLon_[nowCource] - startLon_[nowCource]) * (1.0f / kTimeOfOneCource);
			// 現在の座標を変える
			lat_ = startLat_[nowCource];
			lon_ = startLon_[nowCource];
			break;
		case COURCETHREE:
			// 次のコースへ
			nowCource = MoveCourse::COURCEFOUR;
			// 速度再設定
			velocityLat_ = (goalLat_[nowCource] - startLat_[nowCource]) * (1.0f / kTimeOfOneCource);
			velocityLon_ = (goalLon_[nowCource] - startLon_[nowCource]) * (1.0f / kTimeOfOneCource);
			// 現在の座標を変える
			lat_ = startLat_[nowCource];
			lon_ = startLon_[nowCource];
			break;
		case COURCEFOUR:
			// 次のコースへ
			nowCource = MoveCourse::COURCEONE;
			// 速度再設定
			velocityLat_ = (goalLat_[nowCource] - startLat_[nowCource]) * (1.0f / kTimeOfOneCource);
			velocityLon_ = (goalLon_[nowCource] - startLon_[nowCource]) * (1.0f / kTimeOfOneCource);
			// 現在の座標を変える
			lat_ = startLat_[nowCource];
			lon_ = startLon_[nowCource];
			break;
		default:
			break;
		}
	}

	//速度加算処理
	lat_ += velocityLat_;
	lon_ += velocityLon_;
	
	//ローカル座標更新
	worldTransform_.translation_.x = kCenter_.x + length_[nowCource] * std::cosf(lat_) * std::cosf(lon_);
	worldTransform_.translation_.y = kCenter_.y + length_[nowCource] * std::sinf(lat_);
	worldTransform_.translation_.z = kCenter_.z + length_[nowCource] * std::cosf(lat_) * std::sinf(lon_);

	//カメラをディレクション方向に向けて回転
	direction_ = Normalize(Subtract(kCenter_, worldTransform_.translation_));
	worldTransform_.rotation_.y = std::atan2(direction_.x, direction_.z);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 velocityZ = Transform(direction_, rotateYMatrix);
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	//行列の更新と転送
	worldTransform_.UpdateMatrix();

	// カメラオブジェクトワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
}
