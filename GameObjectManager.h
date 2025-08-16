#pragma once
#include "BaseManager.h"
#include "BaseChara.h"
#include "BaseObject.h"
#include "objects.h"
#include "Layout.h"
#include "JsonManager.h"
#include "BaseGameObjectManager.h"

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
	void TutorialUpdate();
	void tutorialDraw();
	void HandOver(nlohmann::json j)
	{
		jsonData = j;
	}

	bool GetIsGoal() { return isGoal; }
	int GetCoinCount() { return playerManager_actual->GetCoinCount(); }
	std::string GetTag() const { return tag; }

private:
	std::vector<std::shared_ptr<BaseObject>> fieldObjects;
	std::vector<std::shared_ptr<BaseGameObjectManager>> managers;

	std::shared_ptr<BaseObject>				map					= NULL;
	std::shared_ptr<BaseObject>				field				= NULL;
	std::shared_ptr<Camera>					camera				= NULL;
	std::shared_ptr<BaseGameObjectManager>	playerManager		= NULL;
	std::shared_ptr<BaseGameObjectManager>	coinManager			= NULL;
	std::shared_ptr<BaseGameObjectManager>  uiManager			= NULL;
	std::shared_ptr<BaseGameObjectManager>  effectManager		= NULL;
	std::shared_ptr<Layout>					layout				= NULL;
	std::shared_ptr<Shadow>					shadow				= NULL;
	std::shared_ptr<GoalArea>				goalArea			= NULL;
	std::shared_ptr<GameTimer>				gameTimer			= NULL;

	std::shared_ptr<Map>			map_actual			 = NULL;
	std::shared_ptr<PlayerManager>	playerManager_actual = NULL;
	std::shared_ptr<CoinManager>	coinManager_actual   = NULL;
	std::shared_ptr<EffectManager>  effectManager_actual = NULL;
	std::shared_ptr<UIManager>		uiManager_actual	 = NULL;

	int tutorialHandle;
	int startHandle;
	int x_tutorialGraph;
	int y_tutorialGraph;
	int x_startGraph;
	int y_startGraph;

	bool isCamera;
	bool isPush;
	bool isPush_start;
	bool isGoal;
	bool isStream_startPicture;		//スタート画面の映像を流すか

	float stream_startPicture_Timer;	//スタート画面のタイマー
	float startGraph_timer;				//スタート画像の挙動タイマー

	std::string tag;
	nlohmann::json	jsonData;

};

