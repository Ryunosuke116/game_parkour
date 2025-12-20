#pragma once
#include "BaseManager.h"
#include "BaseChara.h"
#include "BaseObject.h"
#include "GameObjects.h"
#include "Layout.h"
#include "JsonManager.h"
#include "BaseGameObjectManager.h"
#include "SoundPlayer.h"
#include "Liner8TreeManager.hpp"

class ObjectManager : public BaseManager
{
public:
	ObjectManager();
	~ObjectManager();

	void Create()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

	void StartUpdate();
	void FinishUpdate();
	void NormalUpdate();
	void WhenDebugUpdate();
	void tutorialDraw();

	void ResultCreate(int coinCount);
	void ResultInitilize();
	void ResultUpdate();
	void ResultDraw();

	void TitleCreate();
	void TitleInitilize();
	void TitleUpdate();
	void TitleDraw();

	bool GetIsGoal() { return isGoal; }
	int GetCoinCount() { return actualPlayerManager->GetCoinCount(); }

private:
	std::vector<std::shared_ptr<IObject>> objects;

	std::shared_ptr<Layout>					layout = NULL;
	std::shared_ptr<Tutorial>				tutorial = NULL;
	std::shared_ptr<FinishCut>				finishCut = NULL;

	std::shared_ptr<PlayerManager>				actualPlayerManager = NULL;
	std::shared_ptr<CoinManager>				actualCoinManager = NULL;
	std::shared_ptr<UIManager>					actualUiManager = NULL;
	std::shared_ptr<GameTimer>					actualGameTimer = NULL;
	std::shared_ptr<Shadow>						actualShadow = NULL;
	std::shared_ptr<Camera>						actualCamera = NULL;
	std::shared_ptr<SkyBox>						actualSkyBox = NULL;
	std::shared_ptr<Liner8TreeManager<Coin>>	L8TreeManager = NULL;

	bool isCamera;
	bool isPush;
	bool isPushResult;
	bool isGoal;
	bool isStreamStartPicture;		//スタート演出を流すか
	bool isStreamFinishPicture;	//フィニッシュ演出を流すか

	float streamStartPictureTimer;	//スタート画面のタイマー
	float streamFinishPictureTimer;	//フィニッシュ画面のタイマー

};

