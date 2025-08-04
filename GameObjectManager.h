#pragma once
#include "BaseManager.h"
#include "BaseChara.h"
#include "BaseObject.h"
#include "objects.h"
#include "Layout.h"
#include "JsonManager.h"
#include "BaseGameObjectManager.h"
#include  "UI.h"

class GameObjectManager : public BaseManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	void Create()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

private:

	std::vector<std::shared_ptr<BaseGameObjectManager>>	objectManager_ilst;

	std::vector<std::shared_ptr<BaseObject>> fieldObjects;

	std::shared_ptr<BaseObject>				map					= NULL;
	std::shared_ptr<BaseObject>				field				= NULL;
	std::shared_ptr<Camera>					camera				= NULL;
	std::shared_ptr<BaseGameObjectManager>	playerManager		= NULL;
	std::shared_ptr<BaseGameObjectManager>	coinManager			= NULL;
	std::shared_ptr<BaseGameObjectManager>  floor_sky_Manager	= NULL;
	std::shared_ptr<Layout>					layout				= NULL;
	std::shared_ptr<JsonManager>			jsonManager			= NULL;
	std::shared_ptr<Shadow>					shadow				= NULL;
	std::shared_ptr<UI>						ui					= NULL;

	std::shared_ptr<Map>			map_actual			 = NULL;
	std::shared_ptr<PlayerManager>	playerManager_actual = NULL;
	std::shared_ptr<CoinManager>	coinManager_actual   = NULL;
	
	bool isCamera;
	bool isPush;
};

