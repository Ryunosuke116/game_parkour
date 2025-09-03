#pragma once
#include "BaseManager.h"
#include "BaseChara.h"
#include "BaseObject.h"
#include "objects.h"
#include "Layout.h"
#include "JsonManager.h"
#include "BaseGameObjectManager.h"
#include "SoundPlayer.h"
#include "ObjectMediator.h"

class GameObjectManager : public BaseManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	void Create()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

	void StartUpdate();
	void FinishUpdate();
	void tutorialDraw();

	bool GetIsGoal() { return isGoal; }
	int GetCoinCount() { return playerManager_actual->GetCoinCount(); }

private:
	std::vector<std::shared_ptr<BaseGameObjectManager>> managers;
	std::vector <std::shared_ptr<BaseObject>>				objects;

	std::shared_ptr<Layout>					layout				= NULL;
	std::shared_ptr<Tutorial>				tutorial			= NULL;
	std::shared_ptr<FinishCut>				finishCut			= NULL;

	std::shared_ptr<PlayerManager>	playerManager_actual		= NULL;
	std::shared_ptr<CoinManager>	coinManager_actual			= NULL;
	std::shared_ptr<UIManager>		uiManager_actual			= NULL;
	std::shared_ptr<GameTimer>		gameTimer_actual			= NULL;
	std::shared_ptr<Shadow>			 shadow_actual				= NULL;
	std::shared_ptr<Camera>			camera_actual				= NULL;
	std::shared_ptr<IEffectManager>  effectManager				= NULL;

	int tutorialHandle;
	int startHandle;

	bool isCamera;
	bool isPush;
	bool isGoal;
	bool isStream_startPicture;		//スタート演出を流すか
	bool isStream_finishPicture;	//フィニッシュ演出を流すか

	float stream_startPicture_timer;	//スタート画面のタイマー
	float stream_finishPicture_timer;	//フィニッシュ画面のタイマー

};

