#include "GameScene.h"
#include "AxisIndicator.h"
#include "BaseScene.h"
#include "GameOverScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "ResultScene.h"
#include "StageScene.h"
#include "TextureManager.h"
#include "TitleScene.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// シーンの生成
	m_pScene = std::make_unique<TitleScene>();
	// シーンの初期化
	m_pScene->Init(input_, audio_);
	CurrentScene_ = TITLE;
	NextScene_ = TITLE;

	// シーン遷移生成
	gradation_ = std::make_unique<Gradation>();
	AnimationFrame_ = 0;
	isInNow_ = false;
	isOutNow_ = false;
}

void GameScene::Update() {
	// シーンの更新処理
	m_pScene->Update();

	// 各シーンからNextSceneの情報を受け取る
	if (NextScene_ != m_pScene->GetNextScene()) {
		NextScene_ = m_pScene->GetNextScene();
	}
	// シーンの切り替え処理
	ChangeScene();
	// 各シーンからアプリの終了合図を受け取る
	isExit_ = m_pScene->GetIsExit();

#ifdef _DEBUG
	ImGui::Begin("debug");
	int nowScene = CurrentScene_;
	ImGui::Text("nowScene:%d", nowScene);

	ImGui::End();
#endif // _DEBUG
}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	// シーンの描画処理
	m_pScene->Draw(commandList, dxCommon_);
	// シーン遷移描画
	gradation_->Draw(commandList);
}

void GameScene::ChangeScene() {
	// シーン切り替えアニメーションとか作るならこの関数に書く

	// もしNextSceneが入力されているならシーン遷移アニメーションに入る
	if (!isInNow_ && NextScene_ != CurrentScene_) {
		AnimationFrame_ = 0;
		isInNow_ = true;
		isOutNow_ = false;
		gradation_->Initialize(KindAni::ADMISSION);
		gradation_->SetIsDraw(true);
	}

	// インアニメーション
	if (isInNow_) {
		AnimationFrame_++;

		// アニメーション処理
		gradation_->Update(AnimationFrame_, kAnimationFrame_);
		// アニメーション終わった時の処理
		if (AnimationFrame_ == kAnimationFrame_) {
			isInNow_ = false;
			isOutNow_ = true;
			AnimationFrame_ = 0;
		}
	}

	// 切り替え
	if (!isInNow_ && isOutNow_ && NextScene_ != CurrentScene_) {
		// 現在のシーンの削除
		if (m_pScene != NULL) {
			m_pScene.reset();
		}
		// 次のシーンの挿入
		switch (NextScene_) { // 引数のシーン
		case SCENE::TITLE:
			m_pScene = std::make_unique<TitleScene>(); // タイトルシーンを現在のシーンにする
			m_pScene->Init(input_, audio_);
			CurrentScene_ = m_pScene->GetNextScene();
			;
			break;
		case SCENE::STAGE:
			m_pScene = std::make_unique<StageScene>(); // ステージシーンを現在のシーンにする
			m_pScene->Init(input_, audio_);
			CurrentScene_ = m_pScene->GetNextScene();
			;
			break;
		case SCENE::GAMEOVER:
			m_pScene = std::make_unique<GameOverScene>(); // ゲームオーバーシーンを現在のシーンにする
			m_pScene->Init(input_, audio_);
			CurrentScene_ = m_pScene->GetNextScene();
			;
			break;
		case SCENE::RESULT:
			m_pScene = std::make_unique<ResultScene>(); // リザルトシーンを現在のシーンにする
			m_pScene->Init(input_, audio_);
			CurrentScene_ = m_pScene->GetNextScene();
			;
			break;

		default:
			break;
		}
		// アニメーションの初期化
		gradation_->Initialize(KindAni::EXIT);
		AnimationFrame_ = 0;
	}

	// アウトアニメーション
	if (isOutNow_ && NextScene_ == CurrentScene_) {
		AnimationFrame_++;

		// アニメーション処理
		gradation_->Update(AnimationFrame_, kAnimationFrame_);
		// アニメーション終わった時の処理
		if (AnimationFrame_ == kAnimationFrame_) {
			isInNow_ = false;
			isOutNow_ = false;
			AnimationFrame_ = 0;
			gradation_->SetIsDraw(false);
		}
	}
}
