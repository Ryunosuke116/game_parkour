#pragma once
#include "BaseManager.h"
#include "BaseChara.h"
#include "BaseObject.h"
#include "objects.h"
#include "Layout.h"

class ObjectManager : public BaseManager
{
public:
	ObjectManager();
	~ObjectManager();

	void Create()override;
	void Initialize()override;
	void Update()override;
	bool Draw()override;
	void Add()override;

private:
	std::shared_ptr<BaseObject>		map = NULL;
	std::shared_ptr<BaseObject>		field = NULL;
	std::shared_ptr<Camera>			camera = NULL;
	std::shared_ptr<BaseManager>	playerManager = NULL;
	std::shared_ptr<BaseObject>		fieldMesh = NULL;
	std::shared_ptr<BaseManager>	coinManager = NULL;
	std::shared_ptr<Layout>			layout = NULL;

	std::shared_ptr<Map>			map_actual = NULL;
	std::shared_ptr<PlayerManager>	playerManager_actual = NULL;
	std::shared_ptr<CoinManager>	coinManager_actual = NULL;

};

